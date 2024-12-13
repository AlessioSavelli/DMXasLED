/*
    Esempio Base per gestire le luci DMX ON/OFF

    N.B: Per giochi luminosi dove viene iterato molte volte la funzione di cambio colore non serve dmx.update();
*/
#include <DMXasLED.h>  // Include la libreria DMXasLED

// Definisci i pin per il modulo RS485 del DMX
#define TX_PIN 3
#define RX_PIN 4
#define EN_PIN 5
// Definisci la portra DMX che varia tra 0 e 1
#define DMX_PORT 1

/*
dmx_personality_t personalities[] = {};
int dmx_personality_count = 0;
dmx_config_t dmx_config = DMX_CONFIG_DEFAULT;  // Configurazione DMX
*/

DMXasLED dmx(TX_PIN, RX_PIN, EN_PIN);  // Crea l'oggetto dmx

unsigned int lightAddress = 1; // L'Address della luce DMX è scritta sul suo display

void setup() {
  Serial.begin(115200);
  // Inizializza la libreria DMXasLED
  dmx.begin(DMX_PORT);  // Configurazione la porta DMX desiderata
  // Alternativa qualora si volossero usare più dettagli per il canale DMX
  // dmx.begin(DMX_PORT, dmx_config, &personalities, dmx_personality_count);

  // Imposta il colore di base delle luci accesse, R,G,B,W,Dimmer
  dmx.attachDefColor(255,127,50,0,255);

  // Aggiungi un profilo luce esempio che usa R G B W senza dimmer, i canali non usati si impostano a 0
  DMXLightMapping lightProfile = { 0, 1, 2, 3, 4 };  // Indirizzi per Dimmer, R, G, B, W
  dmx.attachLight(lightProfile);

}

void loop() { 
  // Esempio di scrittura colori
  dmx.digitalDmxWrite(lightAddress, HIGH);  // Accende la luce con il colore di defualt
  delay(1000);
  // Esempio di scrittura colori
  dmx.digitalDmxWrite(lightAddress, LOW);  // Spegne la luce
  delay(1000);
}
