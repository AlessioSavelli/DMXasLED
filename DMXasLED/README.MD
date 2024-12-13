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
- **`begin(dmx_port, dmx_config)`**: Inizializza la libreria.
- **`attachLight()`**: Configura un profilo luce con canali RGBW.
- **`attachDefColor()`**: Imposta il colore predefinito (R, G, B, W, Dimmer).

### 💡 Gestione diretta
- **`digitalDmxWrite()`**: Accende o spegne una luce DMX.
- **`digitalDmxWriteP()`**: Gestisce luci utilizzando profili DMX.

### 🔄 Aggiornamenti e mapping
- **`update()`**: Invia i dati DMX aggiornati.
- **`getProfilesCount()`**: Restituisce il numero di profili caricati.

---

## 📂 **Esempi inclusi**

### 1. [**BlinkDMX**](https://github.com/AlessioSavelli/DMXasLED/tree/main/DMXasLED/examples/BlinkDMX)
- Esempio base per accendere e spegnere una luce DMX.
- Configura un indirizzo DMX e invia comandi ON/OFF.
- Ideale per principianti.

### 2. [**BlinkDMX2**](https://github.com/AlessioSavelli/DMXasLED/tree/main/DMXasLED/examples/BlinkDMX2)
- Utilizza **profili DMX** per gestire luci RGBW.
- Cambia i colori (Rosso, Verde, Blu, Bianco) in sequenza.
- Perfetto per chi necessita di configurazioni modulari.

### 3. [**PresepeDMX**](https://github.com/AlessioSavelli/DMXasLED/tree/main/DMXasLED/examples/PresepeDMX)
- Esempio complesso con effetti dinamici.
- Simula transizioni tra alba, giorno, tramonto e notte.
- Include tremolio del fuoco e flash casuali di una luce "stella".

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
- **Email**: [alessioneplus@gmail.com](mailto:alessioneplus@gmail.com)

💡 Se hai domande o suggerimenti, sentiti libero di aprire una issue o inviare una pull request. 

---

📜 Distribuito sotto la licenza **MIT**. Vedi il file `LICENSE` per i dettagli.
