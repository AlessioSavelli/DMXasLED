#ifndef DMXProfiles_h
#define DMXProfiles_h

// Definisci il versionamento della struttura
#define DMX_PROFILE_VERSION 1

// Definisco la dimensione massima di un pacchetto DMX
#define DMX_PACKET_SIZE	513

// Struttura per mappare i canali DMX di una luce
struct DMXLightMapping {
    unsigned int DimmerChannel;  // Canale per il dimmer
    unsigned int rChannel;       // Canale per Rosso
    unsigned int gChannel;       // Canale per Verde
    unsigned int bChannel;       // Canale per Blu
    unsigned int wChannel;       // Canale per Bianco (opzionale)
};


#endif
