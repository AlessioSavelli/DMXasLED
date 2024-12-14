# Collegamento Hardware per la Gestione delle Luci DMX

Questo progetto utilizza un modulo RS485 per controllare una luce DMX tramite Arduino. Di seguito viene illustrato il collegamento hardware necessario e il funzionamento del sistema.
## Componenti Necessari

1. **Modulo RS485** per la comunicazione DMX
2. **Luce DMX** compatibile con il protocollo DMX512
3. **ESP32** (o microcontrollore freeRTOS compatibile)
4. **Resistenze di terminazione** (se necessarie, per lunghe tratte DMX)
5. **Cavi di collegamento**

## Descrizione del Collegamento

### 1. Connessioni del Modulo RS485

Il modulo RS485 è essenziale per la comunicazione tra Arduino e il sistema DMX. I collegamenti principali sono:

- **TX (Trasmissione)**: collegato al pin 3 di Arduino
- **RX (Ricezione)**: collegato al pin 4 di Arduino
- **EN (Abilitazione)**: collegato al pin 5 di Arduino
- **VCC**: alimentazione (5V o 3.3V, a seconda del modulo)
- **GND**: massa comune

### 2. Uscita DMX dal Modulo RS485

- **D+ e D-**: questi pin sono collegati ai cavi di ingresso del sistema DMX.
- **GND**: massa comune con il sistema DMX.

### 3. Collegamento alla Luce DMX

Ogni luce DMX deve essere configurata con un indirizzo unico utilizzando i DIP switch o il pannello di configurazione integrato. Segui il manuale della luce per configurare correttamente l'indirizzo.

### 4. Alimentazione

Assicurati che tutti i dispositivi condividano una massa comune (GND) per un funzionamento corretto. Se necessario, utilizza un alimentatore esterno per il modulo RS485 o la luce DMX.

## Schema di Collegamento

Il diagramma del collegamento hardware è illustrato nella seguente immagine:

![Schema Collegamento Hardware](https://github.com/AlessioSavelli/DMXasLED/blob/main/docs/Schema.jpg?raw=true)

## Configurazione del Sistema DMX

1. Configura il modulo RS485 come descritto sopra.
2. Collega le luci DMX e assegna gli indirizzi corretti.
3. Carica il codice Arduino (presente in questo repository) per testare il sistema.
4. Verifica il corretto funzionamento controllando il lampeggio delle luci.

## Nota Importante

- Segui il manuale della luce DMX per configurare correttamente i canali e i parametri richiesti.
- Per connessioni lunghe, considera l'uso di resistenze di terminazione (120 ohm) tra D+ e D- per evitare riflessioni del segnale.

## Conclusione

Questo schema e la guida aiuteranno a impostare correttamente il sistema per gestire le luci DMX con Arduino. In caso di dubbi, consulta la documentazione della libreria [DMXasLED](https://github.com/AlessioSavelli/DMXasLED/tree/main/DMXasLED).
