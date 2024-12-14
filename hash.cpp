#include "HASH.h"
// Costruttore
// Il costruttore `HASH` inizializza l'oggetto all'atto della sua creazione.
// Azioni eseguite:
// - Chiama il metodo `resetCRC8` per impostare il valore iniziale di `crc8` a zero.
// - Questo assicura che l'oggetto inizi con uno stato CRC valido.
HASH::HASH() {
    resetCRC8();  // Inizializza il CRC8 a 0
}

// Metodo `calculateCRC8`
// Calcola il CRC8 per un singolo byte di dati passato come parametro.
// Parametri:
// - `data`: il byte per cui calcolare il CRC8.
// Funzionamento:
// - Inizializza il valore di `crc` con il byte fornito.
// - Esegue 8 iterazioni (una per ciascun bit del byte).
// - Controlla il bit più significativo (`MSB`) di `crc`:
//   - Se è 1, effettua uno shift a sinistra e applica il polinomio CRC8 con XOR.
//   - Se è 0, effettua solo uno shift a sinistra.
// - Ritorna il valore finale di `crc`, che è il CRC8 calcolato per quel byte.
// Nota:
// - Questo metodo non altera lo stato interno della classe (`crc8`).
uint8_t HASH::calculateCRC8(uint8_t data) {
    uint8_t crc = 0;
    crc = data;
    for (int i = 8; i > 0; i--) {
        if (crc & 0x80) {
            crc = (crc << 1) ^ CRC8_POLYNOMIAL;
        } else {
            crc = crc << 1;
        }
    }
    return crc;
}

// Metodo `updateCRC8`
// Aggiorna progressivamente il valore interno del CRC8 (`crc8`) basandosi su un nuovo byte.
// Parametri:
// - `data`: il byte utilizzato per aggiornare il CRC8 corrente.
// Funzionamento:
// - Effettua un XOR tra il valore di `crc8` e il byte fornito.
// - Esegue 8 iterazioni per aggiornare il CRC8:
//   - Se il bit più significativo di `crc8` è 1, esegue uno shift a sinistra e un XOR con il polinomio CRC8.
//   - Altrimenti, esegue solo lo shift a sinistra.
// Azioni sullo stato:
// - Aggiorna il valore interno di `crc8`, accumulando i risultati dei calcoli CRC.
void HASH::updateCRC8(uint8_t data) {
    crc8 ^= data;  // Inizializza con il valore del CRC corrente
    for (int i = 8; i; i--) {
        if (crc8 & 0x80) {
            crc8 = (crc8 << 1) ^ CRC8_POLYNOMIAL;
        } else {
            crc8 = crc8 << 1;
        }
    }
}

// Metodo `resetCRC8`
// Resetta il valore interno di `crc8` a zero.
// Scopo:
// - Riporta lo stato interno dell'oggetto a un valore iniziale, pronto per un nuovo calcolo CRC.
// Azioni:
// - Imposta il valore di `crc8` a 0.
// - Questo metodo deve essere chiamato all'inizio di una nuova operazione CRC.
void HASH::resetCRC8() {
    crc8 = 0;  // Inizializza CRC8 a zero
}

// Metodo `getCRC8`
// Ritorna il valore corrente del CRC8 calcolato.
// Funzionamento:
// - Non altera lo stato interno dell'oggetto.
// - Ritorna il valore accumulato in `crc8`, che rappresenta il risultato del calcolo CRC fino a quel momento.
// Utilizzo:
// - Chiamato tipicamente alla fine di un calcolo CRC per ottenere il valore finale.
uint8_t HASH::getCRC8() const {
    return crc8;
}
