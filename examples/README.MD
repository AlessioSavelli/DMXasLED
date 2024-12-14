# Esempi per DMXasLED

Questa directory contiene tre esempi pratici per la gestione di luci DMX utilizzando la libreria **DMXasLED**. Gli esempi sono progettati per illustrare diversi livelli di complessità e funzionalità.

## Indice degli Esempi

### [1. BlinkDMX](https://github.com/AlessioSavelli/DMXasLED/tree/main/examples/BlinkDMX)
Un esempio base per controllare una luce DMX con un semplice effetto ON/OFF. Questo esempio dimostra come:
- Configurare un microcontrollore per la comunicazione DMX.
- Accendere e spegnere una luce DMX con un colore predefinito.

> Ideale per chi vuole iniziare con il controllo delle luci DMX senza configurazioni avanzate.

---

### [2. BlinkDMX2](https://github.com/AlessioSavelli/DMXasLED/tree/main/examples/BlinkDMX2)
Un esempio avanzato per gestire luci DMX utilizzando profili personalizzati. Questo esempio include:
- Creazione di un profilo DMX per controllare più canali (Dimmer, RGBW).
- Impostazione di colori diversi in sequenza (Rosso, Verde, Blu, Bianco).

> Perfetto per scenari che richiedono la gestione di configurazioni multi-canale in modo modulare e flessibile.

---

### [3. PresepeDMX](https://github.com/AlessioSavelli/DMXasLED/tree/main/examples/PresepeDMX)
Un esempio complesso progettato per simulare le luci di un presepe con effetti dinamici. Funzionalità incluse:
- Transizioni tra alba, giorno, tramonto e notte.
- Simulazione del tremolio del fuoco.
- Flash casuali di una luce "stella" durante la notte.

> Ideale per chi vuole creare giochi di luce dinamici e personalizzati.

---

## Come Utilizzare

1. **Clona il Repository**:
   ```bash
   git clone https://github.com/AlessioSavelli/DMXasLED.git
