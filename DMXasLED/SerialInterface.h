#ifndef SERIAL_INTERFACE_H
#define SERIAL_INTERFACE_H

#include <Arduino.h>
#include "SerialHeader.h"  // Include le definizioni degli header seriali
#include "SPIFFSStorage.h"  // Include la gestione del filesystem SPIFFS
#include "HASH.h"  // Includi la libreria per l'hashing
#include "Macro.h" // Includo la libreria per la gestione delle macro

// Enum per la gestione degli stati di lettura seriale
enum Serial_Reading_State {
  SERIAL_WAITING_FOR_HEADER,
  SERIAL_WAITING_FOR_CRC,
  SERIAL_WAITING_FOR_LENGTH_HSB,
  SERIAL_WAITING_FOR_LENGTH_LSB,
  SERIAL_WAITING_FOR_PAYLOAD_HEADER,
  SERIAL_WAITING_FOR_PAYLOAD_PROCESSING,
  SERIAL_WAITING_FOR_END_PACKET
};

#define SERIAL_BUFFER_SIZE 3 * KByte  // Dimensione del buffer circolare
#define SERIAL_VERSION 0x01 // Versione del protocollo Seriale
#define FW_VERSION 0x01
#define EXTRA_INFOS "Protocolo seriale : Attivo"

// Classe che gestisce l'interfaccia seriale
class SerialInterface {
  private:
    bool _isOK = true;
  
    byte* circBuffer;
    unsigned int bufferSize;
    int bufferStart;
    int bufferEnd;
    Serial_Reading_State state;  // Stato della macchina a stati seriale

    SPIFFSStorage* spiffsStorageInstance;  // Puntatore a un oggetto (può essere SPIFFSStorage o altro)

    HASH hashInstance;  // Istanza dell'oggetto per l'hashing
	
	DMXLightMapping* _lightProfiles;  // Puntatore all'array dei profili luce
    byte* _dmx_Data;  // Puntatore all'array dei profili luce
    int* _lightProfiles_Counter;  // Puntatore al contatore dei profili
    
	// Metodo privato `processHeader`
	// Questo metodo è responsabile della gestione di un pacchetto ricevuto.
	// Parametri:
	// - `header`: il byte di header che identifica il tipo di pacchetto ricevuto.
	// - `stTail`: indice iniziale della coda del buffer circolare per il pacchetto corrente.
	// - `endTail`: indice finale della coda del buffer circolare per il pacchetto corrente.
	// Scopo:
	// - Analizza l'header del pacchetto e decide quale azione intraprendere.
	// - La logica del metodo sarà specifica per il protocollo seriale implementato.
	// Questo metodo è privato e viene chiamato internamente durante l'elaborazione dei dati seriali.
	void processHeader(byte header, int stTail, int endTail);

  public:
	int LED_INDICATOR=0;               // Pin LED indicatore (0 se non usato)
  

	// Costruttore
	// Questo è il costruttore della classe `SerialInterface`. 
	// Viene utilizzato per inizializzare un'istanza della classe con un buffer seriale di dimensione specificata.
	// - `serialBufferSize` (predefinito a `SERIAL_BUFFER_SIZE`): specifica la dimensione del buffer circolare per la comunicazione seriale.
	// Se non viene passato alcun valore, utilizza la dimensione predefinita `SERIAL_BUFFER_SIZE`.
	SerialInterface(unsigned int serialBufferSize = SERIAL_BUFFER_SIZE);

	// Metodo `setupSerial`
	// Questo metodo è responsabile della configurazione iniziale del sistema seriale.
	// Deve essere chiamato prima di utilizzare altre funzioni della classe.
	// Parametri:
	// - `storageInstance`: puntatore a un'istanza della classe `SPIFFSStorage` utilizzata per gestire il filesystem SPIFFS.
	// - `lightProfiles`: puntatore a un array contenente i profili DMX (mappature delle luci).
	// - `dmx_Data`: puntatore a un array di byte che contiene i dati DMX attivi.
	// - `lightProfiles_Counter`: puntatore a un intero che tiene traccia del numero di profili DMX caricati.
	// Scopo: 
	// Inizializza i puntatori delle risorse necessarie per il corretto funzionamento del sistema seriale.
	void setupSerial(SPIFFSStorage* storageInstance, DMXLightMapping* lightProfiles, byte* dmx_Data, int* lightProfiles_Counter);

	// Metodo `runSerial`
	// Questo metodo implementa il ciclo principale di gestione della comunicazione seriale.
	// Deve essere chiamato frequentemente (ad esempio, nel loop principale di Arduino) per elaborare i dati in arrivo.
	// Parametri:
	// - `serialTimeout` (predefinito a 100000): specifica il timeout massimo per la ricezione dei dati seriali in microsecondi.
	// Scopo:
	// - Legge i dati dalla porta seriale.
	// - Analizza i dati ricevuti in base a un protocollo seriale definito.
	// - Passa attraverso uno stato seriale basato sull'enumerazione `Serial_Reading_State` per garantire la corretta sequenza di elaborazione.
	// - Effettua azioni specifiche in base ai dati ricevuti e al loro header.
	void runSerial(unsigned long serialTimeout = 100000);
};

#endif // SERIAL_INTERFACE_H
