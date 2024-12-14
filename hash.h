#ifndef HASH_H
#define HASH_H

#include <Arduino.h>

class HASH {
public:
	// Costruttore
	// Questo è il costruttore della classe `HASH`.
	// Scopo:
	// - Inizializza l'oggetto `HASH`, compreso il valore del CRC8 utilizzato per calcoli successivi.
	// - Il valore iniziale del CRC8 è tipicamente impostato a zero.
	HASH();
	
	// Metodo `calculateCRC8`
	// Calcola il valore CRC8 per un singolo byte di dati.
	// Parametri:
	// - `data`: il byte per il quale calcolare il CRC8.
	// Ritorna:
	// - Il valore CRC8 calcolato per il byte fornito.
	// Scopo:
	// - Fornisce un metodo diretto per calcolare il CRC8 di un byte senza modificare lo stato interno dell'oggetto.
	uint8_t calculateCRC8(uint8_t data);

	// Metodo `updateCRC8`
	// Aggiorna il valore corrente del CRC8 basandosi su un nuovo byte di dati.
	// Parametri:
	// - `data`: il byte che verrà utilizzato per aggiornare il valore CRC8 corrente.
	// Scopo:
	// - Modifica il valore interno di `crc8` accumulando i calcoli CRC per una sequenza di byte.
	// - Utile per calcolare il CRC di dati sequenziali.
	void updateCRC8(uint8_t data);
	
	// Metodo `resetCRC8`
	// Resetta il valore interno del CRC8 a uno stato iniziale.
	// Scopo:
	// - Riporta il valore `crc8` interno a zero, pronto per una nuova serie di calcoli CRC.
	// - Deve essere chiamato prima di iniziare un nuovo calcolo CRC per garantire risultati corretti.
	void resetCRC8();

	// Metodo `getCRC8`
	// Ritorna il valore corrente del CRC8 calcolato.
	// Ritorna:
	// - Il valore del CRC8 attualmente accumulato.
	// Scopo:
	// - Fornisce un metodo per ottenere il risultato del calcolo CRC accumulato fino a quel punto.
	// - Tipicamente viene chiamato dopo aver elaborato tutti i dati per ottenere il risultato finale.
	uint8_t getCRC8() const;

private:
	// Costante `CRC8_POLYNOMIAL`
	// Definisce il polinomio utilizzato per i calcoli CRC8.
	// Valore predefinito:
	// - `0x31` è il polinomio standard per il CRC8 (x^8 + x^5 + x^4 + 1).
	// Scopo:
	// - Questo polinomio viene usato come base per i calcoli CRC8.
	static const uint8_t CRC8_POLYNOMIAL = 0x31;

	// Variabile membro `crc8`
	// Memorizza il valore corrente del CRC8 durante i calcoli.
	// Scopo:
	// - Contiene il risultato parziale o completo del calcolo CRC8 mentre i byte vengono elaborati.
	// - Questo valore viene modificato dal metodo `updateCRC8` e restituito da `getCRC8`.
	uint8_t crc8;
};

#endif
