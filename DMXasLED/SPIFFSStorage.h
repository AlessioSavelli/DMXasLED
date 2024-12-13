#ifndef SPIFFSStorage_h
#define SPIFFSStorage_h

#include <FS.h>
#include <SPIFFS.h>
#include "Macro.h"
#include "DMXProfiles.h"  // Inclusione del file dei profili luce DMX

class SPIFFSStorage {
public:
    // Costruttore
    SPIFFSStorage();

    /**
     * Funzione per inizializzare SPIFFS.
     * Restituisce true se SPIFFS è stato inizializzato correttamente, false altrimenti.
     * La funzione monta il filesystem SPIFFS per permettere la lettura/scrittura di file.
     */
    bool begin();

    /**
     * Funzione per caricare i profili di luce da un file.
     * riceve un puntatore a un array di DMXLightMapping dove verranno caricati i profili.
     * Questo metodo carica il file contenente i profili di luce e li carica nell'array passato come argomento.
     */
    bool loadLightProfiles(DMXLightMapping* profiles, int* _profileIndex);

    /**
     * Funzione per salvare i profili di luce in un file.
     * riceve un puntatore a un array di DMXLightMapping che contiene i profili da salvare.
     * Questa funzione salva i profili di luce nel filesystem SPIFFS.
     */
    bool saveLightProfiles(const DMXLightMapping* profiles);

    /**
     * Funzione per salvare i profili di luce a partire da un array di byte.
     * dmxData: il puntatore all'array di byte che contiene i dati DMX.
     * bufferSize: la dimensione del buffer.
     * stIndex: l'indice di partenza per la scrittura (default 0).
     * endIndex: l'indice finale per la scrittura (default 0, scrive fino alla fine).
     * isCircular: indica se il buffer deve essere considerato circolare (default false).
     * Questo metodo permette di salvare un array di byte contenente i dati DMX in un file.
     */
    bool saveLightProfilesFromBytes(const byte* dmxData, int bufferSize, int stIndex = 0, int endIndex = 0, bool isCircular = false);

    /**
     * Funzione per caricare i dati DMX da un file.
     * dmxData: il puntatore all'array dove verranno caricati i dati DMX.
     * Questa funzione carica i dati DMX da un file nel filesystem SPIFFS e li copia nel buffer fornito.
     */
    bool loadDMXData(byte* dmxData);

    /**
     * Funzione per salvare i dati DMX in un file.
     * dmxData: il puntatore all'array di byte che contiene i dati DMX da salvare.
     * bufferSize: la dimensione del buffer.
     * stIndex: l'indice di partenza per la scrittura (default 0).
     * endIndex: l'indice finale per la scrittura (default 0, scrive fino alla fine).
     * isCircular: indica se il buffer deve essere considerato circolare (default false).
     * Questa funzione salva i dati DMX nel filesystem SPIFFS a partire da un array di byte.
     */
    bool saveDMXData(const byte* dmxData, int bufferSize, int stIndex = 0, int endIndex = 0, bool isCircular = false);

    /**
     * Funzione per ottenere una lista di tutti i file presenti nel filesystem SPIFFS.
     * Restituisce una stringa contenente i nomi dei file separati da una nuova linea.
     * Questo metodo permette di visualizzare tutti i file attualmente presenti nel filesystem SPIFFS.
     */
    String listFiles();

    /**
     * Funzione per ottenere informazioni sull'uso dello spazio di memoria in SPIFFS.
     * Restituisce una stringa con informazioni sull'uso dello spazio di memoria.
     */
    String usage();

    /**
     * Funzione per cancellare tutti i file presenti nel filesystem SPIFFS.
     * Restituisce true se l'operazione è andata a buon fine, false altrimenti.
     * Questa funzione cancella tutti i file nel filesystem SPIFFS.
     */
    bool clear();

    /**
     * Funzione per aggiornare il filesystem SPIFFS.
     * Rende effettive le modifiche e può essere utilizzato per fare il refresh della memoria.
     * Questa funzione consente di eseguire eventuali operazioni necessarie per aggiornare il filesystem.
     */
    void refreshFileSystem();

private:
    // Puoi aggiungere variabili private se necessario
};

#endif
