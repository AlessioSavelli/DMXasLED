/*
    Esempio Base per gestire le luci ON OFF
    N.B: I profili dovranno essere creati in accordo con il manuale d'uso della luce DMX da pilotare
*/
#include <DMXasLED.h>  // Include la libreria DMXasLED

// Definisci i pin per il modulo RS485 del DMX
#define TX_PIN 3
#define RX_PIN 4
#define EN_PIN 5
// Definisci la portra DMX che varia tra 0 e 1
#define DMX_PORT 1

DMXasLED dmx(TX_PIN, RX_PIN, EN_PIN);  // Crea l'oggetto dmx

unsigned int adress_light = 1;  // Indica l'indirizzo della luce DMX che si trova sul display

void setup() {
  // Inizializza la libreria DMXasLED
  dmx.begin(DMX_PORT);  // Configurazione la porta DMX desiderata
  // Alternativa qualora si volossero usare più dettagli per il canale DMX
  // dmx.begin(DMX_PORT, dmx_config, &personalities, dmx_personality_count);

  // Imposta il colore di base delle luci accesse, R,G,B,W,Dimmer
  dmx.attachDefColor(255, 255, 0, 0, 255);

  // Aggiungo il profilo della luce principale
  DMXLightMapping lightProfileMain = { 0, adress_light, adress_light + 1, adress_light + 2, adress_light + 3 };  // Indirizzi per Dimmer, R, G, B, W
  dmx.attachLight(lightProfileMain);
  // indexMain = dmx.getProfilesCount(); // Ottengo l'indirizzo profilo della luce main

  //Aggiungo eventuali byte fissi per gestire le luci - in accordo con il manuale d'uso
  //dmx.setFixedByte(LUCE_MAIN_ADDR + IndexAddress, value);
}

void loop() {
  //Fa un blink con la luce DMX
  dmx.digitalDmxWrite(adress_light, HIGH);
  delay(500);
  dmx.digitalDmxWrite(adress_light, LOW);
  delay(500);
}
