# DMXasLED

La libreria **DMXasLED** è progettata per controllare luci e dispositivi DMX tramite microcontrollori ESP32. Utilizzando questa libreria, è possibile gestire facilmente la trasmissione e la ricezione di segnali DMX per il controllo di luci RGB, dimmer e altri dispositivi compatibili. La libreria semplifica l'implementazione di sistemi di illuminazione professionale basati su DMX in applicazioni come scenografie, spettacoli dal vivo e controllo luci domestico.

## Caratteristiche principali

- Supporto per ESP32 tramite protocollo DMX.
- Gestione dei colori RGB, RGBW e dimmer.
- Funzioni per l'aggiornamento e la gestione dei profili DMX.
- Supporto per il salvataggio e il caricamento di profili tramite SPIFFS.
- Comunicazione seriale per monitoraggio e debugging.

## Installazione

### Metodo 1: Installazione tramite Arduino IDE

1. Scarica o clona questo repository.
2. Copia la cartella `DMXasLED` nella cartella `libraries` del tuo ambiente di sviluppo Arduino. Se stai utilizzando l'IDE di Arduino, la cartella si trova in:
   - **Windows**: `Documents\Arduino\libraries`
   - **macOS/Linux**: `~/Arduino/libraries`
3. Riavvia l'IDE di Arduino per caricare correttamente la libreria.

### Metodo 2: Installazione tramite Library Manager

1. Apri l'IDE di Arduino.
2. Vai su **Sketch** > **Include Library** > **Manage Libraries...**
3. Cerca "DMXasLED" nella barra di ricerca e clicca su **Install**.

## Funzioni principali

### 1. **`begin(dmx_port_t dmx_port, dmx_config_t dmx_config)`**

Inizializza la libreria DMX sulla porta specificata.

**Parametri:**
- `dmx_port`: Porta DMX su cui si avvia la comunicazione (e.g., `DMX_PORT`).
- `dmx_config`: Configurazione DMX da utilizzare (usa il valore predefinito se non specificato).

### 2. **`attachLight(const DMXLightMapping& LightProfile)`**

Aggiunge un profilo luce al sistema DMX. Un profilo luce definisce l'indirizzo DMX e i canali per ogni luce. Questo metodo consente di mappare le luci fisiche agli indirizzi DMX.

**Parametri:**
- `LightProfile`: Struttura contenente la mappatura dell'indirizzo DMX e i canali corrispondenti.

### 3. **`attachDefColor(byte R, byte G, byte B, byte W, byte Dimmer=255)`**

Imposta il colore di accensione predefinito per tutte le luci gestite dal sistema DMX.

**Parametri:**
- `R`: Componente rossa (0-255).
- `G`: Componente verde (0-255).
- `B`: Componente blu (0-255).
- `W`: Componente bianca (0-255, opzionale).
- `Dimmer`: Valore del dimmer (intensità luminosa) della luce (0-255).

### 4. **`setFixedByte(int index, byte value)`**

Imposta un byte fisso in un determinato indice del pacchetto DMX. Utile per configurare specifici canali DMX.

**Parametri:**
- `index`: Indice del canale DMX da configurare.
- `value`: Valore del byte da impostare (0-255).

### 5. **`setFixedByte(int stIndex, byte* value, int length)`**

Imposta una sequenza di byte su più canali DMX a partire da un indice specifico. Utile per inviare più valori DMX in una sola operazione.

**Parametri:**
- `stIndex`: Indice di partenza del canale DMX.
- `value`: Array contenente i valori da scrivere nei canali DMX.
- `length`: Lunghezza dell'array `value`.

### 6. **`digitalDmxWrite(unsigned int lightAddr, byte R, byte G, byte B, byte W, byte Dimmer=255)`**

Scrive i valori RGBW e Dimmer per una luce all'indirizzo DMX specificato.

**Parametri:**
- `lightAddr`: Indirizzo DMX della luce.
- `R`: Componente rossa (0-255).
- `G`: Componente verde (0-255).
- `B`: Componente blu (0-255).
- `W`: Componente bianca (0-255).
- `Dimmer`: Intensità luminosa complessiva (0-255, default 255).

### 7. **`digitalDmxWriteP(unsigned int profileAddr, byte R, byte G, byte B, byte W, byte Dimmer=255)`**

Scrive i valori RGBW e Dimmer per una luce utilizzando un profilo di luce predefinito.

**Parametri:**
- `profileAddr`: Indirizzo DMX del profilo luce.
- `R`: Componente rossa (0-255).
- `G`: Componente verde (0-255).
- `B`: Componente blu (0-255).
- `W`: Componente bianca (0-255).
- `Dimmer`: Intensità luminosa complessiva (0-255).

### 8. **`digitalDmxWrite(unsigned int lightAddr, bool value)`**

Accende o spegne una luce specificata all'indirizzo DMX.

**Parametri:**
- `lightAddr`: Indirizzo DMX della luce.
- `value`: `HIGH` per accendere la luce, `LOW` per spegnerla.

### 9. **`digitalDmxWriteP(unsigned int profileAddr, bool value)`**

Accende o spegne una luce utilizzando un profilo predefinito.

**Parametri:**
- `profileAddr`: Indirizzo DMX del profilo luce.
- `value`: `HIGH` per accendere la luce, `LOW` per spegnerla.

### 10. **`getProfilesCount()`**

Restituisce il numero di profili caricati nel sistema. È utile per gestire dinamicamente i profili in un'applicazione che richiede l'uso di più dispositivi DMX.

### 11. **`update()`**

Invia i dati DMX aggiornati a tutte le luci e dispositivi configurati. Questo metodo deve essere chiamato periodicamente per aggiornare lo stato delle luci.

### 12. **`updateSerial()`**

Invia i dati seriali per il monitoraggio e il debugging. Utilizza la comunicazione seriale per inviare informazioni sullo stato del sistema.

### 13. **`mappingDmxProfile()`**

Crea un mapping tra gli indirizzi DMX e i profili luce per una gestione efficiente delle luci e dei dispositivi.

### 14. **`addMappingProfile(const DMXLightMapping& profile, unsigned int index)`**

Aggiunge un profilo luce specificato all'array dei profili. Questo metodo è utilizzato internamente per mappare le luci agli indirizzi DMX.

### 15. **`changeColorLight(DMXLightMapping Light, byte R, byte G, byte B, byte W, byte Dimmer)`**

Cambia il colore e l'intensità di una luce specificata dal suo profilo DMX.

## Profili DMX

La libreria supporta la gestione di **profili DMX**, che definiscono le impostazioni per luci specifiche. È possibile definire profili personalizzati e assegnare i colori a ciascun dispositivo, facilitando la gestione di impianti di illuminazione complessi.

## Licenza

Distribuito sotto la licenza MIT. Vedi `LICENSE` per i dettagli.

## Contatti

- **Autore**: Alessio Savelli

---

Se hai domande o suggerimenti, non esitare a contattarmi o a contribuire al progetto!
