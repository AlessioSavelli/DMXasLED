#include "SPIFFSStorage.h"

// Costruttore
SPIFFSStorage::SPIFFSStorage() {
    // Puoi aggiungere parametri se necessario per la configurazione
}

// Funzione di setup per inizializzare SPIFFS
bool SPIFFSStorage::begin() {
  if (!SPIFFS.begin(true)) {
    return false;  // Restituisce false se SPIFFS non è stato inizializzato correttamente
  }
  return true;  // Restituisce true se SPIFFS è stato inizializzato correttamente
}

bool SPIFFSStorage::loadLightProfiles(DMXLightMapping* profiles, int* _profileIndex) {
  // Apriamo il file in modalità di lettura binaria
  File file = SPIFFS.open("/light_profiles.bin", "rb");
  if (!file) {
    return false;  // Restituisce se il file non è stato aperto
  }

  int profileIndex = 0;  // Contatore per i profili caricati
  byte buffer[10];  // Buffer per un singolo profilo da 10 byte

  // Legge i profili di luce a blocchi di 10 byte
  while (file.available() && (profileIndex < 256)) {
    if (file.read(buffer, 10) != 10) {
      break;  // Termina se i dati letti sono incompleti
    }

    // Verifica che non siano i due byte di fine lettura dati
    if ((buffer[0] == 0x0A) && (buffer[1] == 0x00)) {
      break;  // Se troviamo i byte di fine lettura, fermiamo la lettura
    }

    // Decodifica e assegna i valori ai canali del profilo
    profiles[profileIndex].DimmerChannel = (buffer[0] << 8) | buffer[1];
    profiles[profileIndex].rChannel = (buffer[2] << 8) | buffer[3];
    profiles[profileIndex].gChannel = (buffer[4] << 8) | buffer[5];
    profiles[profileIndex].bChannel = (buffer[6] << 8) | buffer[7];
    profiles[profileIndex].wChannel = (buffer[8] << 8) | buffer[9];

    profileIndex++;  // Incrementa l'indice del profilo
  }
	//Converte il risultato in 0 based
	profileIndex --;

  // Imposta il valore del contatore di profili letti
  *_profileIndex = profileIndex;

  file.close();  // Chiudi il file
  return true;
}

// Funzione per salvare i profili di luce nel file
bool SPIFFSStorage::saveLightProfiles(const DMXLightMapping* profiles) {
  File file = SPIFFS.open("/light_profiles.bin", "wb");
  if (!file) {
    return false;  // Restituisce se il file non è stato aperto
  }

  // Scrive ogni canale dei profili di luce nel file
  for (int i = 0; i < 256; i++) {
    file.write(profiles[i].DimmerChannel >> 8);    // Scrive il byte alto (MSB)
    file.write(profiles[i].DimmerChannel & 0xFF);  // Scrive il byte basso (LSB)

    file.write(profiles[i].rChannel >> 8);    // Scrive il byte alto (MSB)
    file.write(profiles[i].rChannel & 0xFF);  // Scrive il byte basso (LSB)

    file.write(profiles[i].gChannel >> 8);    // Scrive il byte alto (MSB)
    file.write(profiles[i].gChannel & 0xFF);  // Scrive il byte basso (LSB)

    file.write(profiles[i].bChannel >> 8);    // Scrive il byte alto (MSB)
    file.write(profiles[i].bChannel & 0xFF);  // Scrive il byte basso (LSB)

    file.write(profiles[i].wChannel >> 8);    // Scrive il byte alto (MSB)
    file.write(profiles[i].wChannel & 0xFF);  // Scrive il byte basso (LSB)
  }

  file.close();
  
  return true;
}

// Funzione per salvare i profili di luce da un array di byte
bool SPIFFSStorage::saveLightProfilesFromBytes(const byte* dmxData, int bufferSize, int stIndex, int endIndex, bool isCircular) {
  File file = SPIFFS.open("/light_profiles.bin", "wb");
  if (!file) {
    return false;  // Restituisce se il file non è stato aperto
  }

  unsigned int reading_index = stIndex;
  while (true) {
    if (reading_index == endIndex) break;

    // Scrivi il byte corrente nel file
    file.write(dmxData[reading_index]);

    if (isCircular) {
      CIRC_BUFFER_ONESCROLL(reading_index, bufferSize);  // Avanza l'indice usando la modalità circolare
    } else {
      reading_index++;
    }
  }

  file.close();
  return true;
}

// Funzione per caricare i dati DMX nel buffer
bool SPIFFSStorage::loadDMXData(byte* dmxData) {
  memset(dmxData, 0, DMX_PACKET_SIZE);  // Imposta tutti i byte a zero se il file non è disponibile
  File file = SPIFFS.open("/dmx_data.bin", "rb");
  if (!file) {
    return false;  // Restituisce se il file non è stato aperto
  }

  // Inizializza il primo byte a 0, come richiesto dal protocollo DMX

  size_t fileSize = file.size();
  size_t max_lengt = DMX_PACKET_SIZE * 2;
  if (max_lengt > fileSize) return false;

  for (size_t index = 0; index < DMX_PACKET_SIZE; index++) {
    byte HSB = file.read();
    byte LSB = file.read();
    dmxData[index] = ((HSB << 8) | LSB);
  }

  file.close();
  return true;
}

// Funzione per salvare i dati DMX nel file
bool SPIFFSStorage::saveDMXData(const byte* dmxData, int bufferSize, int stIndex, int endIndex, bool isCircular) {
  File file = SPIFFS.open("/dmx_data.bin", "wb");
  if (!file) {
    return false;  // Restituisce se il file non è stato aperto
  }

  int expectedSize = 512;
  int actualBufferSize = (bufferSize <= 0) ? 0 : bufferSize;  // Verifica la dimensione del buffer

  if (actualBufferSize == 0) {
    file.close();
    return false;
  }

  endIndex = min(endIndex, actualBufferSize - 1);

  if (!isCircular) {
    file.write(dmxData, actualBufferSize);
    int remainingBytes = expectedSize - actualBufferSize;
    if (remainingBytes > 0) {
      byte zero = 0;
      for (int i = 0; i < remainingBytes; i++) {
        file.write(zero);
      }
    }
  } else {
    int wrIndex = stIndex;
    do {
      file.write(dmxData[wrIndex]);
      CIRC_BUFFER_ONESCROLL(wrIndex, actualBufferSize);
    } while (wrIndex != endIndex);

    int remainingBytes = expectedSize - (endIndex - stIndex + 1);
    if (remainingBytes > 0) {
      byte zero = 0;
      for (int i = 0; i < remainingBytes; i++) {
        file.write(zero);
      }
    }
  }

  file.close();
  return true;
}

// Funzione per visualizzare l'elenco dei file nel filesystem SPIFFS
String SPIFFSStorage::listFiles() {
  String fileList = "";
  File root = SPIFFS.open("/");
  if (!root) {
    return fileList;
  }

  if (!root.isDirectory()) {
    return fileList;
  }

  File file = root.openNextFile();
  while (file) {
    String fileName = file.name();
    uint32_t fileSize = file.size();
    String fileSizeStr = String(fileSize) + " Byte";
    float kb = fileSize / 1024;
    float mb = kb / 1024;
    if (kb >= 1) {
      fileSizeStr = (mb >= 1) ? String(mb, 2) + " MB" : String(kb, 2) + " KB";
    }
    fileList += "FILE: " + fileName + "\tSIZE: " + fileSizeStr + "\n";
    file.close();
    file = root.openNextFile();
  }

  root.close();
  return fileList;
}

// Funzione per ottenere l'utilizzo dello spazio in SPIFFS
String SPIFFSStorage::usage() {
  uint32_t totalBytes = SPIFFS.totalBytes();
  uint32_t usedBytes = SPIFFS.usedBytes();
  float totalMB = totalBytes / (1024.0 * 1024.0);
  float usedMB = usedBytes / (1024.0 * 1024.0);
  float usedPercent = (usedBytes * 100) / totalBytes;

  String usageInfo = "TOT " + String(totalMB, 3) + "MB/" + String(usedMB, 3) + "MB Usati (" + String(usedPercent, 3) + "% Usati)";
  return usageInfo;
}

// Funzione per cancellare tutti i file nel file system SPIFFS
bool SPIFFSStorage::clear() {
  if (!SPIFFS.begin(true)) {
    return false;
  }

  File root = SPIFFS.open("/");
  if (!root) {
    return false;
  }

  File file = root.openNextFile();
  while (file) {
    SPIFFS.remove("/" + String(file.name()));
    file.close();
    file = root.openNextFile();
  }

  root.close();
  return true;
}

// Funzione per forzare il refresh del file system SPIFFS
void SPIFFSStorage::refreshFileSystem() {
  SPIFFS.end();
  TASKDELAY(100);
  SPIFFS.begin(true);
}
