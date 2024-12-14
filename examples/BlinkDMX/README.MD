# Esempio Base per Gestire Luci DMX (ON/OFF)

Questo esempio dimostra come controllare luci DMX con un semplice comando ON/OFF utilizzando la libreria **DMXasLED**. L'implementazione include anche la configurazione dei profili DMX per adattarsi alle specifiche della luce utilizzata, in accordo con il relativo manuale.

---

## Funzionamento

L'applicazione è progettata per creare un profilo luce personalizzato e far lampeggiare (blink) una luce DMX con un indirizzo configurabile.

### Struttura del Codice

1. **Configurazione iniziale**:
   - I pin hardware per la comunicazione RS485 (DMX) vengono definiti come segue:
     ```cpp
     #define TX_PIN 3  // Pin trasmissione
     #define RX_PIN 4  // Pin ricezione
     #define EN_PIN 5  // Pin abilitazione
     #define DMX_PORT 1 // Porta DMX da usare
     ```
   - La libreria **DMXasLED** viene inizializzata con la funzione `dmx.begin(DMX_PORT)`.

2. **Settaggio del colore di base**:
   - È possibile impostare il colore predefinito (RGBW + Dimmer) con:
     ```cpp
     dmx.attachDefColor(255, 255, 0, 0, 255);
     ```

3. **Creazione del profilo luce**:
   - I canali DMX della luce principale vengono mappati in un profilo personalizzato:
     ```cpp
     DMXLightMapping lightProfileMain = { 0, adress_light, adress_light + 1, adress_light + 2, adress_light + 3 };
     dmx.attachLight(lightProfileMain);
     ```
     I canali sono configurati come segue:
     - Dimmer: Canale 0
     - Rosso (R): `adress_light`
     - Verde (G): `adress_light + 1`
     - Blu (B): `adress_light + 2`
     - Bianco (W): `adress_light + 3`

4. **Comando ON/OFF**:
   - Nel loop principale, la luce lampeggia (ON/OFF) con un ritardo di 500 millisecondi:
     ```cpp
     dmx.digitalDmxWrite(adress_light, HIGH);
     delay(500);
     dmx.digitalDmxWrite(adress_light, LOW);
     delay(500);
     ```

---

## Requisiti

### Hardware
- Un microcontrollore compatibile (es. Arduino o ESP32).
- Modulo RS485 per la comunicazione DMX.
- Luci compatibili con il protocollo DMX.

### Software
- Libreria **DMXasLED** per la gestione del protocollo DMX.
- IDE Arduino o simile per la compilazione e caricamento del codice.

---

## Schema del Flusso di Lavoro

```plaintext
+---------------------------+
| Inizializzazione Hardware |
+---------------------------+
            |
            v
+---------------------------+
| Configurazione Colori e   |
| Profili DMX               |
+---------------------------+
            |
            v
+---------------------------+
|     Loop Principale       |
|  - Accendi la luce (ON)   |
|  - Attendi 500ms          |
|  - Spegni la luce (OFF)   |
|  - Attendi 500ms          |
+---------------------------+
            |
            v
+---------------------------+
|       Ripeti il ciclo     |
+---------------------------+
