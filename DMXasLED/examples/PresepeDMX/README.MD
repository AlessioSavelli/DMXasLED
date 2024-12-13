# Sistema di Illuminazione DMX per Presepe

Questo progetto utilizza un sistema di illuminazione DMX per creare effetti dinamici di alba, giorno, tramonto e notte in un presepe. Le luci vengono controllate attraverso un microcontrollore che utilizza la libreria `DMXasLED` e un modulo RS485.

## Funzionalità principali

1. **Effetti di luce dinamici**:
   - **Alba**: Transizione dolce da notte a giorno, con simulazione del fuoco.
   - **Giorno**: Luci statiche e luminose.
   - **Tramonto**: Transizione dolce da giorno a notte, con accensione progressiva del fuoco.
   - **Notte**: Simulazione del fuoco e lampeggi casuali di una luce "stella".

2. **Simulazione realistica del fuoco**:
   - Utilizzo di variazioni casuali per creare un effetto tremolante.
   
3. **Lampeggio casuale della stella**:
   - Flash brevi e casuali durante la notte.

---

## Come funziona

### Diagramma di flusso dell'algoritmo
                 +-----------------------+
                 |       START           |
                 +-----------------------+
                          |
                          v
      +-----------------------+       +-----------------------+
      |   alba(durata)         |----->|   giorno(durata)      |
      | Transizione alba       |      | Luce fissa diurna     |
      | (notte -> giorno)      |      |                      |
      +-----------------------+       +-----------------------+
                ^                           |
                |                           v
      +-----------------------+       +-----------------------+
      |   tramonto(durata)     |<-----|   notte(durata)       |
      | Transizione tramonto   |      | Tremolio e flash      |
      | (giorno -> notte)      |      | randomico stella      |
      +-----------------------+       +-----------------------+
      


1. **Setup iniziale**:
   - Configurazione del DMX e mappatura delle luci.
   - Definizione dei colori base per giorno, notte e fuoco.

2. **Loop principale**:
   - Vengono eseguite sequenzialmente le funzioni per gli effetti (alba, giorno, tramonto e notte).
   - Ogni fase ha una durata specificata e comportamenti predefiniti.

3. **Effetti dettagliati**:
   - **Alba**:
     - Transizione graduale dal colore notturno a quello diurno.
     - Durante la seconda metà dell’alba, il fuoco si spegne progressivamente.
   - **Giorno**:
     - Colore statico con aggiornamenti regolari del DMX.
   - **Tramonto**:
     - Transizione graduale dal colore diurno a quello notturno.
     - Durante la seconda metà del tramonto, il fuoco si accende progressivamente.
   - **Notte**:
     - Fuoco tremolante continuo.
     - Flash casuali della stella con periodi di esclusione.

---

## Configurazione hardware

1. **Pin collegati al modulo RS485**:
   - TX: Pin 3
   - RX: Pin 4
   - EN: Pin 5

2. **Indirizzi delle luci DMX**:
   - **LUCE_MAIN_ADDR**: Luce principale per gli effetti giorno/notte (indirizzo 8).
   - **LUCE_FUOCO_ADDR**: Simulazione del fuoco (indirizzo 16).
   - **LUCE_STELLA_ADDR**: Lampeggio della stella (indirizzo 1).

3. **Mappatura dei canali DMX**:
   - Ogni luce utilizza 4 canali: Dimmer, Rosso, Verde, Blu (opzionale: Bianco).

---

## Struttura del codice

### Librerie utilizzate
- `DMXasLED`: Gestione del protocollo DMX.
- `esp_system`: Generazione di numeri casuali (HRNG).

### Funzioni principali
- `setup()`: Configura i pin, la libreria DMX e i colori base.
- `loop()`: Esegue sequenzialmente gli effetti di alba, giorno, tramonto e notte.
- `alba()`, `giorno()`, `tramonto()`, `notte()`: Implementano gli effetti di luce.
- `fadeColorPWM()`: Transizione graduale tra due colori.
- `ColorWithFlicker()`: Simula un tremolio realistico.

---

## Diagramma temporale del funzionamento

| Tempo        | LUCE_MAIN_ADDR                      | LUCE_FUOCO_ADDR            | LUCE_STELLA_ADDR     |
|--------------|-------------------------------------|----------------------------|----------------------|
| **Alba**     | Crescita (NightColor → DayColor)    | Decrescita tremolante      | Spenta               |
| **Giorno**   | Stabile (DayColor)                  | Spenta                     | Spenta               |
| **Tramonto** | Decrescita (DayColor → NightColor)  | Crescita tremolante        | Spenta               |
| **Notte**    | Stabile (NightColor)                | Tremolio continuo          | Flash casuali        |

---

## Requisiti

- Microcontrollore con supporto DMX (ad esempio ESP32).
- Modulo RS485 per la comunicazione DMX.
- Luci DMX compatibili con gli effetti desiderati.

---

## Come utilizzare

1. Configurare il modulo DMX e collegarlo alle luci secondo gli indirizzi specificati.
2. Caricare il codice sul microcontrollore utilizzando l’IDE Arduino.
3. Regolare i tempi e i colori direttamente nel codice se necessario.
4. Alimentare il sistema e osservare gli effetti!

---

## Contributi

Questo progetto è aperto ai contributi. Sentiti libero di migliorare il codice e aggiungere nuove funzionalità!
