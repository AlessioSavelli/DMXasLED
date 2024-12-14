#ifndef DMXasLED_h
#define DMXasLED_h

#include <Arduino.h>
#include <esp_dmx.h>          // Inclusione della libreria per la gestione del protocollo DMX su ESP32
#include "DMXProfiles.h"      // File che definisce i profili DMX
#include "SPIFFSStorage.h"    // Libreria per la gestione del filesystem SPIFFS
#include "SerialInterface.h"  // Classe per la gestione della comunicazione seriale

// Definizione costante per la gestione di overflow dei profili
#define PROFILES_OVERFLOW  (256 + 1)  // Indica che il massimo numero di profili supportati è 256 (più uno di sicurezza)

// Struttura per il colore di defualt accesso
struct DMXDefualtColor {
    unsigned int Dimmer;  // valore per il dimmer
    unsigned int R;       // valore per Rosso
    unsigned int G;       // valore per Verde
    unsigned int B;       // valore per Blu
    unsigned int W;       // valore per Bianco (opzionale)
};


// Classe principale per la gestione delle luci DMX
class DMXasLED {
public:
    unsigned long serialTimeout = 10000; // Timeout di default per la seriale, impostato a 10 secondi

    // Costruttore della classe
    // Parametri:
    // - `TX_PIN`: Pin per la trasmissione DMX
    // - `RX_PIN`: Pin per la ricezione DMX
    // - `EN_PIN`: Pin per abilitare il driver DMX
    // - `useSerialWithSPIFFS`: Abilita l'uso combinato di protocollo seriale e filesystem SPIFFS
    // - `LED_INDICATOR`: Pin di un LED usato come indicatore (se 0, non viene usato)
    DMXasLED(int TX_PIN, int RX_PIN, int EN_PIN, bool useSerialWithSPIFFS = false, int LED_INDICATOR = 0);

    // Metodo di inizializzazione
    // Parametri:
    // - `dmx_port`: Porta DMX da configurare
    // - `dmx_config`: Configurazione DMX (usa il valore predefinito se non specificato)
    // - `personalities`: Array di personalità DMX (opzionale)
    // - `personality_count`: Numero di personalità DMX definite
    void begin(
        dmx_port_t dmx_port, 
        dmx_config_t dmx_config = DMX_CONFIG_DEFAULT, 
        dmx_personality_t* personalities = nullptr, 
        int personality_count = 0
    );

    // Metodo per aggiungere un profilo luce al sistema
    // Parametri:
    // - `LightProfile`: Struttura che descrive il profilo DMX di una luce
    void attachLight(const DMXLightMapping& LightProfile);

	// Metodo per definire un colore base di tutte le luci
	// Parametri :
	//  - `R`		Componente Rossa  0-255
	//  - `G`		Componente Verde  0-255
	//  - `B`		Componente Blu    0-255
	//  - `W`		Componente Bianca 0-255
	//  - `Dimmer`  Dimmer della luce 0-255
	void attachDefColor(byte R, byte G, byte B, byte W, byte Dimmer=255);

    // Metodi per impostare manualmente uno o più byte al pacchetto DMX
    // `setFixedByte`: Imposta un valore byte su un indice specifico
    void setFixedByte(int index, byte value);

    // `setFixedByte`: Imposta un array di byte a partire da un indice specifico
    // Parametri:
    // - `stIndex`: Indice di partenza
    // - `value`: Puntatore all'array dei valori
    // - `length`: Lunghezza dell'array
    void setFixedByte(int stIndex, byte* value, int length);

    // Metodo per scrivere valori RGBW e Dimmer per una luce
    // Parametri:
    // - `lightAddr`: Indirizzo DMX della luce
    // - `R`, `G`, `B`, `W`: Valori dei canali rosso, verde, blu, e bianco
    // - `Dimmer`: Intensità luminosa complessiva (default 255)
    void digitalDmxWrite(unsigned int lightAddr, byte R, byte G, byte B, byte W, byte Dimmer = 255);
	// Fa la stessa cosa del DMX Write ma per usnado i profili di riferimento
	void digitalDmxWriteP(unsigned int profileAddr, byte R, byte G, byte B, byte W, byte Dimmer = 255);

    // Metodo per accendere e spegnere la luce
    // Parametri:
    // - `lightAddr`: Indirizzo DMX della luce
    // - `value`: Indica se accendere o spegnere la luce con il colore di defualt
    void digitalDmxWrite(unsigned int lightAddr, bool value);
	// Fa la stessa cosa del DMX Write ma per usnado i profili di riferimento
	void digitalDmxWriteP(unsigned int profileAddr, bool value);
	// Metodo per ottenere i profili caricati in memoria
	// Ritorna il numero di profilo caricati
	unsigned int getProfilesCount();

    // Metodo per inviare il pacchetto DMX
    void update();

    // Metodo per aggiornare lo stato della comunicazione seriale
    void updateSerial();

private:
    // Oggetto per la gestione della comunicazione seriale
    SerialInterface _SerialUSB;

    // Oggetto per la gestione del filesystem SPIFFS
    SPIFFSStorage _SpiffssManager;

    int _TX_PIN;                      // Pin di trasmissione DMX
    int _RX_PIN;                      // Pin di ricezione DMX
    int _EN_PIN;                      // Pin di abilitazione del driver DMX
    int _LED_INDICATOR;               // Pin LED indicatore (0 se non usato)

    bool _useSerialWithSPIFFS;        // Flag per abilitare l'uso della seriale e SPIFFS per la gestione profili

    unsigned long _SERIAL_TIMEOUT = 10 * 1000; // Timeout interno per la seriale


	DMXDefualtColor _defOnColor; // Colore di accensione base della luce DMX

    // Configurazione della porta DMX
    dmx_port_t _dmx_port;             // Porta DMX utilizzata
    dmx_config_t _dmx_config;         // Configurazione DMX
    dmx_personality_t* _personalities;// Array di personalità DMX
    int _dmx_personality_count;       // Numero di personalità DMX configurate

    // Array per i profili di luce DMX (fino a 256 profili)
    DMXLightMapping _lightProfiles[256];

    // Contatore per il numero di profili attualmente salvati
    int _lightProfiles_Counter = 0;

    // Pacchetto DMX (512 canali + 1 byte di start code)
    byte _dmx_Data[513] = { 0 };      // Inizializza tutto a zero

    // Array per velocizzare il mapping tra indirizzi e profili luce
    unsigned int _fast_addressing_dmx[513] = {0};

    // Metodo per creare un mapping tra indirizzi DMX e profili di luce
    // Funzione ausiliaria per garantire una gestione veloce e accurata delle luci
    void mappingDmxProfile();
	
	void addMappingProfile(const DMXLightMapping& profile, unsigned int index);

	void changeColorLight(DMXLightMapping Light, byte R, byte G, byte B, byte W, byte Dimmer);
};

#endif
