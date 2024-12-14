# **DMXasLED**

**DMXasLED** è una libreria progettata per controllare luci e dispositivi DMX tramite microcontrollori ESP32. Semplifica la gestione di sistemi di illuminazione professionale utilizzando il protocollo DMX.

---

## 🌟 **Caratteristiche principali**
- ✅ Supporto completo per ESP32 tramite DMX.
- 🎨 Gestione colori RGB, RGBW e dimmer.
- 📂 Salvataggio e caricamento di profili tramite SPIFFS *(in fase di realizzazione)*.
- 🛠️ Comunicazione seriale per monitoraggio e debugging *(in fase di realizzazione)*.


---

## 🚀 **Installazione**

### Metodo 1: Installazione manuale
1. Scarica o clona questo repository.
2. Copia la cartella `DMXasLED` nella directory `libraries` del tuo ambiente Arduino:
   - **Windows**: `Documents\Arduino\libraries`
   - **macOS/Linux**: `~/Arduino/libraries`
3. Riavvia Arduino IDE per caricare la libreria.

### Metodo 2: Library Manager
1. Apri Arduino IDE.
2. Vai su **Sketch** > **Include Library** > **Manage Libraries...**.
3. Cerca "DMXasLED" e clicca su **Install**.

---

## ⚙️ **Funzioni principali**

### 🖌️ Configurazione e controllo delle luci

#### 1. **`begin(dmx_port, dmx_config)`**
Inizializza la libreria DMX sulla porta specificata.

- **Parametri**:
  - `dmx_port`: Porta DMX su cui si avvia la comunicazione (es. `DMX_PORT`).
  - `dmx_config`: Configurazione DMX da utilizzare (usa il valore predefinito se non specificato).

- **Esempio**:
  ```cpp
  DMXasLED dmx(TX_PIN, RX_PIN, EN_PIN); // Crea un'istanza di DMXasLED
  dmx.begin(DMX_PORT);                  // Inizializza il DMX sulla porta specificata
  ```

---

#### 2. **`attachLight(const DMXLightMapping& LightProfile)`**
Aggiunge un profilo luce al sistema DMX, specificando i canali da utilizzare.

- **Parametri**:
  - `LightProfile`: Struttura contenente la mappatura dell'indirizzo DMX e dei canali corrispondenti.

- **Esempio**:
  ```cpp
  DMXLightMapping lightProfile = {0, 1, 2, 3, 4}; // Canali: Dimmer, R, G, B, W
  dmx.attachLight(lightProfile);                 // Associa il profilo al DMX
  ```

---

#### 3. **`attachDefColor(byte R, byte G, byte B, byte W, byte Dimmer=255)`**
Imposta il colore di accensione predefinito per tutte le luci gestite dal sistema DMX.

- **Parametri**:
  - `R`, `G`, `B`, `W`: Valori dei canali colore (0-255).
  - `Dimmer`: Intensità luminosa complessiva (0-255).

- **Esempio**:
  ```cpp
  dmx.attachDefColor(255, 127, 50, 0, 255); // Rosso acceso, Verde medio, Blu minimo
  ```

---

### 💡 Gestione diretta

#### 4. **`digitalDmxWrite(unsigned int lightAddr, byte R, byte G, byte B, byte W, byte Dimmer=255)`**
Scrive i valori RGBW e Dimmer per una luce all'indirizzo DMX specificato.

- **Parametri**:
  - `lightAddr`: Indirizzo DMX della luce.
  - `R`, `G`, `B`, `W`: Componenti colore (0-255).
  - `Dimmer`: Intensità luminosa complessiva (default 255).

- **Esempio**:
  ```cpp
  dmx.digitalDmxWrite(1, 255, 0, 0, 0, 255); // Luce rossa accesa al massimo
  ```

---

#### 5. **`digitalDmxWriteP(unsigned int profileAddr, byte R, byte G, byte B, byte W, byte Dimmer=255)`**
Scrive i valori RGBW e Dimmer per una luce utilizzando un profilo di luce predefinito.

- **Parametri**:
  - `profileAddr`: Indirizzo DMX del profilo luce.
  - `R`, `G`, `B`, `W`: Componenti colore (0-255).
  - `Dimmer`: Intensità luminosa complessiva (default 255).

- **Esempio**:
  ```cpp
  dmx.digitalDmxWriteP(profileIndex, 0, 255, 0, 0, 255); // Luce verde accesa al massimo
  ```

---

### 🔄 Aggiornamenti e mapping

#### 6. **`update()`**
Invia i dati DMX aggiornati a tutte le luci e dispositivi configurati. Deve essere chiamato periodicamente per mantenere sincronizzati gli stati.

- **Esempio**:
  ```cpp
  dmx.update(); // Aggiorna tutti i dati DMX
  ```

---

#### 7. **`getProfilesCount()`**
Restituisce il numero di profili caricati nel sistema. Utile per gestire dinamicamente i profili.

- **Esempio**:
  ```cpp
  unsigned int profileCount = dmx.getProfilesCount();
  Serial.print("Numero di profili: ");
  Serial.println(profileCount);
  ```

---
---

## 📂 **Esempi inclusi**

### 1. [**BlinkDMX**](https://github.com/AlessioSavelli/DMXasLED/tree/main/examples/BlinkDMX)
- Esempio base per accendere e spegnere una luce DMX.
- Configura un indirizzo DMX e invia comandi ON/OFF.
- Ideale per principianti.

### 2. [**BlinkDMX2**](https://github.com/AlessioSavelli/DMXasLED/tree/main/examples/BlinkDMX2)
- Utilizza **profili DMX** per gestire luci RGBW.
- Cambia i colori (Rosso, Verde, Blu, Bianco) in sequenza.
- Perfetto per chi necessita di configurazioni modulari.

### 3. [**PresepeDMX**](https://github.com/AlessioSavelli/DMXasLED/tree/main/examples/PresepeDMX)
- Esempio complesso con effetti dinamici.
- Simula transizioni tra alba, giorno, tramonto e notte.
- Include tremolio del fuoco e flash casuali di una luce "stella".

### 3. [**Configurazione Hardware**](https://github.com/AlessioSavelli/DMXasLED/tree/main/docs/)

---

## 📜 **Versionamento**

- **Versione attuale**: 1.0.0
- **Changelog**:
  - **1.0.0**: Rilascio iniziale con supporto per profili DMX, controllo RGBW e gestione di transizioni.

---

## 📝 **TODO**
- 📂 **Supporto SPIFFS**: Implementazione completa per salvataggio e caricamento di profili.
- 🛠️ **Debugging seriale avanzato**: Miglioramento della comunicazione seriale per monitoraggio in tempo reale.
- 💻 **Software di gestione profili**: Prossimamente, sarà disponibile un software per Windows che, insieme alla seriale, consentirà di:
  - Caricare nuovi profili direttamente da PC.
  - Modificare i profili esistenti in modo semplice e rapido.
---

## 📧 **Contatti**

- **Autore**: Alessio Savelli

💡 Se hai domande o suggerimenti, sentiti libero di aprire una issue o inviare una pull request. 

---

📜 Distribuito sotto la licenza **MIT**. Vedi il file `LICENSE` per i dettagli.
