# Esempio Base per Gestione Luci DMX con Profili

Questo esempio dimostra come utilizzare profili DMX per gestire le luci con colori specifici. Utilizza la libreria **DMXasLED** per configurare un profilo e permette di inviare comandi ai canali corrispondenti in modo strutturato.

---

## Funzionamento

L'applicazione mostra come creare un profilo DMX per una luce con canali RGBW. Ogni ciclo del loop principale cambia il colore della luce passando tra rosso, verde, blu e bianco.

### Differenze rispetto agli esempi precedenti

1. **Utilizzo dei profili DMX**:
   - Invece di inviare comandi direttamente agli indirizzi dei canali, si utilizza un profilo che raggruppa i canali (Dimmer, R, G, B, W).
   - La funzione `dmx.digitalDmxWriteP()` consente di impostare i valori utilizzando l'indice del profilo.

2. **Gestione avanzata**:
   - Questo approccio rende il codice più modulare e facilmente estensibile per luci con configurazioni complesse.

---

## Struttura del Codice

1. **Configurazione iniziale**:
   - I pin hardware per la comunicazione RS485 (DMX) vengono definiti come segue:
     ```cpp
     #define TX_PIN 3  // Pin trasmissione
     #define RX_PIN 4  // Pin ricezione
     #define EN_PIN 5  // Pin abilitazione
     #define DMX_PORT 1 // Porta DMX da usare
     ```
   - La libreria **DMXasLED** viene inizializzata con la funzione `dmx.begin(DMX_PORT)`.

2. **Creazione del profilo luce**:
   - Un profilo luce viene creato specificando i canali per Dimmer, R, G, B e W:
     ```cpp
     DMXLightMapping lightProfile = { 0, 1, 2, 3, 4 }; 
     dmx.attachLight(lightProfile);
     index_profilo = dmx.getProfilesCount(); // Ottengo l'indice del profilo
     ```
     Questo profilo consente di controllare tutti i canali associati utilizzando l'indice `index_profilo`.

3. **Comandi ON/OFF con profili**:
   - Nel loop principale, il colore della luce viene impostato ciclicamente tra rosso, verde, blu e bianco utilizzando:
     ```cpp
     dmx.digitalDmxWriteP(index_profilo, 255, 0, 0, 0); // Colore rosso
     delay(1000);
     ```
     Qui, i valori rappresentano le intensità dei canali RGBW (Rosso, Verde, Blu, Bianco).

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
|  - Colore Rosso           |
|  - Attendi 1000ms         |
|  - Colore Verde           |
|  - Attendi 1000ms         |
|  - Colore Blu             |
|  - Attendi 1000ms         |
|  - Colore Bianco          |
|  - Attendi 1000ms         |
+---------------------------+
            |
            v
+---------------------------+
|       Ripeti il ciclo     |
+---------------------------+
