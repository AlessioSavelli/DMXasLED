#include "DMXasLED.h"

// Costruttore della classe DMXasLED
DMXasLED::DMXasLED(int TX_PIN, int RX_PIN, int EN_PIN, bool useSerialWithSPIFFS, int LED_INDICATOR) {
    _TX_PIN = TX_PIN;
    _RX_PIN = RX_PIN;
    _EN_PIN = EN_PIN;
    _useSerialWithSPIFFS = useSerialWithSPIFFS;  // Salva il valore dell'uso seriale
    _LED_INDICATOR = LED_INDICATOR;  // Salva il valore del LED o 0 se non specificato
}

void DMXasLED::begin(
    dmx_port_t dmx_port, 
    dmx_config_t dmx_config, 
    dmx_personality_t* personalities, 
    int personality_count
) {
    _dmx_port = dmx_port;
    _dmx_config = dmx_config;
    _personalities = personalities;
    _dmx_personality_count = personality_count;

    // Configura i pin
    pinMode(_TX_PIN, OUTPUT);
    pinMode(_RX_PIN, INPUT);
    pinMode(_EN_PIN, OUTPUT);

	//Definisco il colore bianco di base
	attachDefColor(255,255,255,255,255);
	
    // Configura il LED indicatore (se presente)
    if (_LED_INDICATOR > 0) {
        pinMode(_LED_INDICATOR, OUTPUT);
        digitalWrite(_LED_INDICATOR, LOW);
    }

    // Inizializza il driver DMX
    dmx_driver_install(_dmx_port, &_dmx_config, _personalities, _dmx_personality_count); 
    // Configura i pin per DMX
    dmx_set_pin(_dmx_port, _TX_PIN, _RX_PIN, _EN_PIN);
	
	
    // Inizializzazione SPIFFS se richiesto
    if (_useSerialWithSPIFFS) {
        if (!_SpiffssManager.begin()) {
            return;
        }

		// Carica i profili luce dal filesystem SPIFFS se esistono
		_SpiffssManager.loadLightProfiles(_lightProfiles, &_lightProfiles_Counter);

        
        // Carica il layout dei fixed byte dal filesystem SPIFFS
        _SpiffssManager.loadDMXData(_dmx_Data);
		
		    // Ora passa i dati alla seriale usando setupSerial
        _SerialUSB.setupSerial(
            &_SpiffssManager,  // Oggetto che gestisce SPIFFS
            _lightProfiles,    // I profili luce
            _dmx_Data,         // I dati DMX
            &_lightProfiles_Counter  // Il contatore dei profili luce
        );
		
		_SerialUSB.LED_INDICATOR = _LED_INDICATOR; // Associo il led di segnalazione all'interfaccia seriale
    }
}

void DMXasLED::attachLight(const DMXLightMapping& LightProfile) {
    if (_lightProfiles_Counter < 256) {
        // Aggiungi il profilo luce all'array
        _lightProfiles[_lightProfiles_Counter++] = LightProfile;
		// Mappa la luce per il fast searching
		addMappingProfile(LightProfile,(_lightProfiles_Counter-1));
    }
}

void DMXasLED::setFixedByte(int index, byte value) {
    if (index >= 0 && index <= 512) {
        _dmx_Data[index] = value;
    }
	update(); // Aggiorna i dati sul canale DMX
}
void DMXasLED::setFixedByte(int stIndex, byte* value, int length) {
    for (int i = 0; i < length; i++) {
        if (stIndex + i <= 512) {
            _dmx_Data[stIndex + i] = value[i];
        }
    }
	update(); // Aggiorna i dati sul canale DMX
}

void DMXasLED::attachDefColor(byte R, byte G, byte B, byte W, byte Dimmer){
	_defOnColor.R = R;
	_defOnColor.G = G;
	_defOnColor.B = B;
	_defOnColor.W = W;
	_defOnColor.Dimmer = Dimmer;
}
// Implementazione del metodo update
void DMXasLED::update() {
  // Aggiorna i dati da inviare sul protocollo DMX
  dmx_write(_dmx_port, _dmx_Data, DMX_PACKET_SIZE);

  //Una volta aggiornati i dati da inviare sul protocollo e' pronto ad inviarli
  dmx_send_num(_dmx_port, DMX_PACKET_SIZE);

  //Blocca il thread del DMX in attesa che tutti i dati DMX siano inviati
  dmx_wait_sent(_dmx_port, DMX_TIMEOUT_TICK);
}
void DMXasLED::updateSerial(){
	_SerialUSB.runSerial(serialTimeout);
}

unsigned int DMXasLED::getProfilesCount(){
	return _lightProfiles_Counter;
}
void DMXasLED::digitalDmxWriteP(unsigned int lightAddr, byte R, byte G, byte B, byte W, byte Dimmer){
	if (lightAddr> 256) return; // indice non valido
	// Ottiene il profilo della luce
	DMXLightMapping Light = _lightProfiles[lightAddr];
	changeColorLight(Light,R,G,B,W,Dimmer); // Cambio il colore della luce
}
void DMXasLED::digitalDmxWriteP(unsigned int lightAddr, bool value){
	if (lightAddr> 256) return; // indice non valido
	// Ottiene il profilo della luce
	DMXLightMapping Light = _lightProfiles[lightAddr];
	if(value){
		changeColorLight(Light,_defOnColor.R,_defOnColor.G,_defOnColor.B,_defOnColor.W,_defOnColor.Dimmer);
	}else{
		changeColorLight(Light,0,0,0,0,255);
	}
}
void DMXasLED::digitalDmxWrite(unsigned int lightAddr, byte R, byte G, byte B, byte W, byte Dimmer){
	// Converto l'indice in base 0
	unsigned int _lightAddr = lightAddr;
	if (_lightAddr> 512) return; // indice non valido
	
	// Ottiene il profilo della luce
	DMXLightMapping Light = _lightProfiles[_fast_addressing_dmx[_lightAddr]];
	changeColorLight(Light,R,G,B,W,Dimmer); // Cambio il colore della luce
}
void DMXasLED::digitalDmxWrite(unsigned int lightAddr, bool value){
	if (value){
		//Imposto il colore di defualt della luce
		digitalDmxWrite(lightAddr,_defOnColor.R,_defOnColor.G,_defOnColor.B,_defOnColor.W,_defOnColor.Dimmer);
	}else{
		//Spengo la luce
		digitalDmxWrite(lightAddr,0,0,0,0,255);
	}
}

void DMXasLED::changeColorLight(DMXLightMapping Light, byte R, byte G, byte B, byte W, byte Dimmer){
		// Imposto la colorazione solo dei canali usati
		if(Light.rChannel > 0) _dmx_Data[Light.rChannel] = R;
		if(Light.gChannel > 0) _dmx_Data[Light.gChannel] = G;
		if(Light.bChannel > 0)_dmx_Data[Light.bChannel] = B;
		if(Light.wChannel > 0)_dmx_Data[Light.wChannel] = W;
		if(Light.DimmerChannel > 0)_dmx_Data[Light.DimmerChannel] = Dimmer;

		update(); // Aggiorna i dati sul canale DMX
}

void DMXasLED::addMappingProfile(const DMXLightMapping& profile, unsigned int index){
	   
        // Mappa ogni canale (Dimmer, RGB, W) al profilo
        // Utilizziamo l'indirizzo di canale come indice nell'array _fast_addressing_dmx
        if (profile.DimmerChannel > 0 && profile.DimmerChannel <= 512) {
            _fast_addressing_dmx[profile.DimmerChannel - 1] = index;  // Mappa il profilo al canale del dimmer
        }
        if (profile.rChannel > 0 && profile.rChannel <= 512) {
            _fast_addressing_dmx[profile.rChannel] = index;  // Mappa il profilo al canale del rosso
        }
        if (profile.gChannel > 0 && profile.gChannel <= 512) {
            _fast_addressing_dmx[profile.gChannel] = index;  // Mappa il profilo al canale del verde
        }
        if (profile.bChannel > 0 && profile.bChannel <= 512) {
            _fast_addressing_dmx[profile.bChannel] = index;  // Mappa il profilo al canale del blu
        }
        if (profile.wChannel > 0 && profile.wChannel <= 512) {
            _fast_addressing_dmx[profile.wChannel] = index;  // Mappa il profilo al canale del bianco
        }
}

    // Inizializza l'array di mappatura degli indirizzi DMX
    // Supponiamo che ci siano al massimo 512 canali DMX (1-512)
    // L'array sarà di dimensione 512 per mappare ogni canale DMX con un profilo associato
    // Profilo del canale: qui possiamo mettere il riferimento al profilo
    // ad esempio: il tipo di struttura può essere DMXLightMapping
void DMXasLED::mappingDmxProfile() {
    // Inizializziamo _fast_addressing_dmx a 0 (nessun profilo associato)
    for (int i = 0; i < 512; i++) {
        _fast_addressing_dmx[i] = PROFILES_OVERFLOW;  // Nessun profilo associato, indica l'overflow del dato
    }

    // Scorriamo tutti i profili luce caricati
    for (int i = 0; i < _lightProfiles_Counter; i++) {
		addMappingProfile(_lightProfiles[i],i);
    }
}

	