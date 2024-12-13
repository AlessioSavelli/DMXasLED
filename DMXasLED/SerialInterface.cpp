#include "SerialInterface.h"

// Costruttore
SerialInterface::SerialInterface(unsigned int serialBufferSize) {
    // Imposta la dimensione del buffer seriale
    bufferSize = serialBufferSize;
    circBuffer = new byte[bufferSize];  // Allocazione dinamica del buffer circolare
    bufferStart = 0;                    // Inizializza il puntatore di inizio del buffer
    bufferEnd = 0;                      // Inizializza il puntatore di fine del buffer
    state = SERIAL_WAITING_FOR_HEADER;  // Stato iniziale

    spiffsStorageInstance = nullptr;    // Nessun oggetto di storage SPIFFS associato inizialmente
    _lightProfiles = nullptr;           // Nessun profilo luce inizialmente
    _dmx_Data = nullptr;                // Nessun dato DMX inizialmente
    _lightProfiles_Counter = nullptr;   // Nessun contatore di profili luce inizialmente
}

// Metodo per inizializzare lo spiffs usage
void SerialInterface::setupSerial(SPIFFSStorage* storageInstance, DMXLightMapping* lightProfiles, byte* dmx_Data, int* lightProfiles_Counter) {
    // Inizializza SPIFFS (oppure altro sistema di storage passato)
    spiffsStorageInstance = storageInstance;

    // Associa i profili luce e i dati DMX
    _lightProfiles = lightProfiles;
    _dmx_Data = dmx_Data;
    _lightProfiles_Counter = lightProfiles_Counter;

    // Eventuali altre inizializzazioni necessarie
    // Esegui operazioni specifiche di setup, come il montaggio del filesystem SPIFFS
    if (spiffsStorageInstance != nullptr) {
        _isOK = false;
    }
}

// Funzione per elaborare il buffer seriale
void SerialInterface::runSerial(unsigned long serialTimeout) {
	
	if (!_isOK) return; // Se lo spiffss da errore non usa nemmeno la seriale
	
	// Calcola l'indice di fine lettura del buffer circolare
#define GET_END_TAIL ((stDataIndex + payloadLength) % SERIAL_BUFFER_SIZE)
#define GET_ST_TAIL_WITHNOHEADER (stDataIndex + 1)


  static unsigned int Serial_circ_head = 0;  // Indice di scrittura
  static unsigned int Serial_circ_tail = 0;  // Indice di lettura
  static unsigned long Serial_timeout = MACRO_TIMER_IS_NULL;
  // Controlla se ci sono dati disponibili sulla seriale
  if (Serial.available()) {
    // Leggi tutti i dati disponibili finché ci sono
    while (Serial.available()) {
      // Leggi un byte dalla seriale e scrivilo nel buffer circolare
      circBuffer[Serial_circ_head] = Serial.read();
      CIRC_BUFFER_ONESCROLL(Serial_circ_head, (unsigned int)(SERIAL_BUFFER_SIZE));  // Avanza l'indice di testa
    }
    // Resetta e avvia il timer del timeout
    START_TIMER(Serial_timeout);

  } else if (ISALIVE_TIMER(Serial_timeout)) {
    // Verifica se il timer è scaduto senza ricevere nuovi dati
    if (IS_DELAYED(Serial_timeout, serialTimeout)) {
      //if (state != SERIAL_WAITING_FOR_HEADER) Serial.println("Serial reading TIMEOUT!");  // Messaggio di errore
      // Se è passato il timeout senza lettura, resettare lo stato della macchina
      state = SERIAL_WAITING_FOR_HEADER;
      STOP_TIMER(Serial_timeout);  // Ferma il timer
    }
  }
  if (Serial_circ_tail == Serial_circ_head) return;       // Se non ci sono byte da leggere, esci
#define BYTE_READED circBuffer[Serial_circ_tail]  // Definizione per il byte attuale

  static unsigned int payloadLength = 0;  // Lunghezza del payload
  static unsigned int stDataIndex = 0;    // Indice di partenza dei dati
  static unsigned int payloadReaded = 0;  // Indice per contare i byte letti
  static byte Serial_header = 0x00;       // Header del messaggio
  static byte Serial_CRC8 = 0x00;         // CRC8 del messaggio
  static byte CRC8 = 0x00;                // Variabile per il calcolo del CRC
  switch (state) {
    case SERIAL_WAITING_FOR_HEADER:
      // Indica che e' in attesa di un payload valido
      if(LED_INDICATOR != 0) digitalWrite(LED_INDICATOR, LOW);
      if (BYTE_READED == SERIAL_HEADER) {  // Se si riceve l'header corretto
      // Indica di aver ottenuto un payload valido
        if(LED_INDICATOR != 0)digitalWrite(LED_INDICATOR, HIGH);
        state = SERIAL_WAITING_FOR_CRC;  // Passa al prossimo stato
      }
      break;

    case SERIAL_WAITING_FOR_CRC:                    // Legge il CRC8 del messaggio
      Serial_CRC8 = BYTE_READED;                    // Memorizza il CRC8 ricevuto
      state = SERIAL_WAITING_FOR_LENGTH_HSB;  // Passa a leggere la lunghezza
      break;

    case SERIAL_WAITING_FOR_LENGTH_HSB:             // Legge l'HSB della lunghezza del messaggio
      payloadLength = (BYTE_READED << 8);           // Memorizza l'HSB
      state = SERIAL_WAITING_FOR_LENGTH_LSB;  // Passa a leggere il LSB
      break;

    case SERIAL_WAITING_FOR_LENGTH_LSB:                 // Legge l'LSB della lunghezza del messaggio
      payloadLength |= BYTE_READED;                     // Completa la lunghezza del payload
      state = SERIAL_WAITING_FOR_PAYLOAD_HEADER;  // Passa a leggere l'header del payload
      break;

    case SERIAL_WAITING_FOR_PAYLOAD_HEADER:  // Legge l'header del payload
      Serial_header = BYTE_READED;           // Memorizza l'header
      stDataIndex = Serial_circ_tail;        // Salva dove iniziano i byte del payload
      payloadReaded = 1;                     // Salva di aver letto un byte
      CRC8 = 0x00;                           // Inizializza il CRC8 a zero
	  hashInstance.resetCRC8();
                                             //Inizia a calcoalre il CRC dall'header
      hashInstance.updateCRC8(BYTE_READED);

      /*
        N.B Gestione custom per i payload di lunghezza uguale a 1
      */

      if (payloadLength <= 1) {                       // Se la lunghezza del payload è 1, processa subito
        state = SERIAL_WAITING_FOR_END_PACKET;  // A questo punto ha già processato il comando e attende la chiusura pacchetto
      } else {
        state = SERIAL_WAITING_FOR_PAYLOAD_PROCESSING;  // Passa a processare il payload
      }

      break;

    case SERIAL_WAITING_FOR_PAYLOAD_PROCESSING:
      //Calcolo dinamico del CRC8
      hashInstance.updateCRC8(BYTE_READED);
      payloadReaded++;  //Salva di aver letto un byte
      if (payloadReaded > payloadLength) {
        state = SERIAL_WAITING_FOR_END_PACKET;  // Il payload è completo
      }
      break;
    case SERIAL_WAITING_FOR_END_PACKET:
      if (BYTE_READED == SERIAL_END_PACKET) state = SERIAL_WAITING_FOR_HEADER;  // Resetta la macchina a stati

      // Verifica del CRC: confronta il CRC calcolato con quello ricevuto
	  CRC8 = hashInstance.getCRC8();
      if (CRC8 == Serial_CRC8) {
        if (payloadLength <= 1) {
          processHeader(Serial_header, stDataIndex, stDataIndex);  // processa il comando di payload 1
        } else {
          processHeader(Serial_header, GET_ST_TAIL_WITHNOHEADER, GET_END_TAIL);  // Legge i dati
        }
      }
      if(LED_INDICATOR != 0)digitalWrite(LED_INDICATOR, LOW);
      break;
  }

  // Porta avanti il buffer circolare
  CIRC_BUFFER_ONESCROLL(Serial_circ_tail, (unsigned int)(SERIAL_BUFFER_SIZE));
}

 // Metodo per processare un pacchetto ricevuto, dato un header
void SerialInterface::processHeader(byte header, int stTail, int endTail){		
  static byte CRC8 = 0;
  unsigned int payloadLength;

  // Crea un'istanza della classe HASH
  HASH hashInstance;
  
  // In questa fase blocca il thread all'interno di questa funzione fino a quando non processa tutti i byte
  switch (header) {
    case SERIAL_HEADER_DMXLEYOUT:
        Serial.println("We pass from here");
        if (spiffsStorageInstance) {
            // Usa il metodo di SPIFFSStorage per salvare i dati DMX
            spiffsStorageInstance->saveDMXData(circBuffer, SERIAL_BUFFER_SIZE, stTail, endTail, true);
            // Carica il layout dei dati DMX
            spiffsStorageInstance->loadDMXData(_dmx_Data);  // Carica i dati DMX
        }
        break;

    case SERIAL_HEADER_DMXPROFILES:
        if (spiffsStorageInstance) {
            // Usa il metodo di SPIFFSStorage per salvare i profili luce
            spiffsStorageInstance->saveLightProfilesFromBytes(circBuffer, SERIAL_BUFFER_SIZE, stTail, endTail, true);
            // Carica i profili delle luci
            spiffsStorageInstance->loadLightProfiles(_lightProfiles,_lightProfiles_Counter);  // Carica i profili di luce
        }
        break;

    case SERIAL_HEADER_GETDMXLEYOUT_ASCII:
        // Invia il layout dei dati DMX
        Serial.println("DMX FIXED BYTE:");
        for (int i = 0; i < 513; i++) {
            Serial.print(_dmx_Data[i]);  // Utilizza il puntatore ai dati DMX
            Serial.print("\t");
        }
        Serial.print("\n");
        break;

    case SERIAL_HEADER_GETDMXLEYOUT_BYTE:
        Serial.write(SERIAL_HEADER);
        CRC8 = 0x00;
        hashInstance.resetCRC8();

        // Usa l'oggetto hashInstance per calcolare il CRC8 dei dati DMX
        for (int i = 0; i < 513; i++) {
            hashInstance.updateCRC8(_dmx_Data[i]);  // Utilizza i dati DMX dal puntatore
        }

        CRC8 = hashInstance.getCRC8();
        Serial.write(CRC8);
        Serial.write((513 >> 8) & 0xFF);  // HSB
        Serial.write((513) & 0xFF);       // LSB

        // Invio dei dati DMX
        for (int i = 0; i < 513; i++) {
            Serial.write(_dmx_Data[i]);  // Utilizza il puntatore ai dati DMX
        }
        Serial.write(SERIAL_END_PACKET);
        break;

    case SERIAL_HEADER_GETDMXPROFILES_ASCII:
        // Stampa i profili aggiornati
        Serial.println("DMX Profiles :");
        for (int i = 0; i < 256; i++) {
            Serial.print("[+] Profilo Nr");
            Serial.println(i);
            Serial.print("\t DimmerCH :");
            Serial.println(_lightProfiles[i].DimmerChannel);
            Serial.print("\t RedCH :");
            Serial.println(_lightProfiles[i].rChannel);
            Serial.print("\t GreenCH :");
            Serial.println(_lightProfiles[i].gChannel);
            Serial.print("\t BlueCH :");
            Serial.println(_lightProfiles[i].bChannel);
            Serial.print("\t WhiteCH :");
            Serial.println(_lightProfiles[i].wChannel);
        }
        break;

    case SERIAL_HEADER_GETDMXPROFILES_BYTE:
        Serial.write(SERIAL_HEADER);
        CRC8 = 0x00;
        hashInstance.resetCRC8();
        // Calcolo del CRC per l'header di comando
        hashInstance.updateCRC8(SERIAL_HEADER_GETDMXPROFILES_BYTE);

        // Calcolo CRC e lunghezza payload per i profili
        payloadLength = 256 * 5 * 2;  // 256 profili, 5 canali ciascuno, 2 byte per canale
        for (int i = 0; i < 256; i++) {
            hashInstance.updateCRC8((_lightProfiles[i].DimmerChannel >> 8) & 0xFF);  // HSB
            hashInstance.updateCRC8(_lightProfiles[i].DimmerChannel & 0xFF);         // LSB

            hashInstance.updateCRC8((_lightProfiles[i].rChannel >> 8) & 0xFF);  // HSB
            hashInstance.updateCRC8(_lightProfiles[i].rChannel & 0xFF);         // LSB

            hashInstance.updateCRC8((_lightProfiles[i].gChannel >> 8) & 0xFF);  // HSB
            hashInstance.updateCRC8(_lightProfiles[i].gChannel & 0xFF);         // LSB

            hashInstance.updateCRC8((_lightProfiles[i].bChannel >> 8) & 0xFF);  // HSB
            hashInstance.updateCRC8(_lightProfiles[i].bChannel & 0xFF);         // LSB

            hashInstance.updateCRC8((_lightProfiles[i].wChannel >> 8) & 0xFF);  // HSB
            hashInstance.updateCRC8(_lightProfiles[i].wChannel & 0xFF);         // LSB

            CRC8 = hashInstance.getCRC8();
        }

        // Invio del CRC calcolato
        Serial.write(CRC8);

        // Invio della lunghezza del payload (1280 byte)
        Serial.write((payloadLength >> 8) & 0xFF);  // HSB
        Serial.write(payloadLength & 0xFF);         // LSB

        // Invio del comando
        Serial.write(SERIAL_HEADER_GETDMXPROFILES_BYTE);

        // Invio dei profili di luce (256 profili, ciascun canale in HSB e LSB)
        for (int i = 0; i < 256; i++) {
            Serial.write((_lightProfiles[i].DimmerChannel >> 8) & 0xFF);  // HSB
            Serial.write(_lightProfiles[i].DimmerChannel & 0xFF);         // LSB

            Serial.write((_lightProfiles[i].rChannel >> 8) & 0xFF);  // HSB
            Serial.write(_lightProfiles[i].rChannel & 0xFF);         // LSB

            Serial.write((_lightProfiles[i].gChannel >> 8) & 0xFF);  // HSB
            Serial.write(_lightProfiles[i].gChannel & 0xFF);         // LSB

            Serial.write((_lightProfiles[i].bChannel >> 8) & 0xFF);  // HSB
            Serial.write(_lightProfiles[i].bChannel & 0xFF);         // LSB

            Serial.write((_lightProfiles[i].wChannel >> 8) & 0xFF);  // HSB
            Serial.write(_lightProfiles[i].wChannel & 0xFF);         // LSB
        }

        // Invio del terminatore di pacchetto
        Serial.write(SERIAL_END_PACKET);
        break;

    case SERIAL_HEADER_GETFWVERS:
        Serial.write(SERIAL_HEADER);
        CRC8 = 0x00;
        hashInstance.resetCRC8();
        hashInstance.updateCRC8(SERIAL_HEADER_GETFWVERS);
        hashInstance.updateCRC8(FW_VERSION);
        CRC8 = hashInstance.getCRC8();
        Serial.write(CRC8);
        // Invio lunghezza payload di 1 byte
        Serial.write(0x00);
        Serial.write(0x02);
        Serial.write(SERIAL_HEADER_GETFWVERS);
        Serial.write(FW_VERSION);
        Serial.write(SERIAL_END_PACKET);
        break;

    case SERIAL_HEADER_GETFWVERS_ASCII:
        Serial.print("Control Unit Version : \t\t");
        Serial.println(FW_VERSION, DEC);
        Serial.print("Serial Protocol Version : \t");
        Serial.println(SERIAL_VERSION, DEC);
        Serial.println(EXTRA_INFOS);
        break;

    case SERIAL_HEADER_GETSPIFFS_LIST_ASCII:
        Serial.println("SPIFFS Explorer : ");
        Serial.println(spiffsStorageInstance->listFiles());
        break;

    case SERIAL_HEADER_GETSPIFFSUSAGE_ASCII:
        Serial.println(spiffsStorageInstance->usage());
        break;

    case SERIAL_HEADER_CLEARSPIFFS:
        if (spiffsStorageInstance->clear()) {
            Serial.println("Memoria SPIFFS PULITA!!!!!!!!\n\n");
            Serial.println(spiffsStorageInstance->usage());
        }
        break;

    case SERIAL_HEADER_REFRESH_SPIFFS:
        spiffsStorageInstance->refreshFileSystem();
        Serial.println("Memoria SPIFFS Aggiornata, mediante riavvio fs!!!!!!!!\n\n");
        break;

    case SERIAL_HEADER_DISCOVERY:  // Risponde al discovery inviando un payload che contiene due volte l'header di discovery
        Serial.write(SERIAL_HEADER);
        Serial.write(0xF4);
        Serial.write(0x00);
        Serial.write(0x02);
        Serial.write(SERIAL_HEADER_DISCOVERY);
        Serial.write(SERIAL_HEADER_DISCOVERY);
        Serial.write(SERIAL_END_PACKET);
        break;

    default:
        break;
  }
}

