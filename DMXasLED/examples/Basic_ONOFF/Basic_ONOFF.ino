/*
    Esempio Base per presepe
    Gestisce 4 luci DMX per fare i vari effetti

    Effetto Alba, con la simulazione fuoco tremolante
    Effetto Giorno, con le luci tutte accese
    Effetto Tramonto, con la simulazione fuoco tremolante
    Effetto Notte, con la simulazione fuoco tremolante e il flash randomico della luce indicata come stella

    Funzionamento:
    Ogni effetto è temporizzato e controlla le 4 luci come segue:

    - **LUCE_MAIN_ADDR**: Rappresenta la luce principale (notte/giorno).
    - **Fuoco**: Simula un tremolio realistico durante alba, tramonto e notte.
    - **Stella**: Flash casuali durante la notte.

    Grafico temporale di funzionamento delle luci:

    Tempo    | LUCE_MAIN_ADDR                     | Fuoco                  | Stella              
    --------|-------------------------------------|------------------------|---------------------
    Alba    | Crescita (NightColor -> DayColor)   | Decrescita tremolante  | Spenta              
    Giorno  | Stabile (DayColor)                  | Spenta                 | Spenta              
    Tramonto| Decrescita (DayColor -> NightColor) | Crescita tremolante    | Spenta              
    Notte   | Stabile (NightColor)                | Tremolio continuo      | Flash casuali       

    N.B: I profili dovranno essere creati in accordo con il manuale d'uso della luce DMX da pilotare
*/
#include <DMXasLED.h>  // Include la libreria DMXasLED

//----Definisco un Delay da usare per il freertos---
#define TASKDELAY(MS) vTaskDelay(MS / portTICK_RATE_MS);
//------

// Definisci i pin per il modulo RS485 del DMX
#define TX_PIN 3
#define RX_PIN 4
#define EN_PIN 5
// Definisci la portra DMX che varia tra 0 e 1
#define DMX_PORT 1

#define LUCE_MAIN_ADDR 1  //L'indirizzo della luce principale

#define LUCE_FUOCO_ADDR 8  //L'indirizzo della luce principale

#define LUCE_STELLA_ADDR 16  //L'indirizzo della luce principale

DMXasLED dmx(TX_PIN, RX_PIN, EN_PIN);  // Crea l'oggetto dmx

struct Color {
  unsigned int R;  // Canale per Rosso
  unsigned int G;  // Canale per Verde
  unsigned int B;  // Canale per Blu
  unsigned int W;  // Canale per Bianco (opzionale)
};

Color DayColor;
Color NightColor;
Color FireColor;

void setup() {
  Serial.begin(115200);
  // Inizializza la libreria DMXasLED
  dmx.begin(DMX_PORT);  // Configurazione la porta DMX desiderata
  // Alternativa qualora si volossero usare più dettagli per il canale DMX
  // dmx.begin(DMX_PORT, dmx_config, &personalities, dmx_personality_count);

  // Imposta il colore di base delle luci accesse, R,G,B,W,Dimmer
  dmx.attachDefColor(255, 127, 50, 0, 255);

  // Aggiungo il profilo della luce principale
  DMXLightMapping lightProfileMain = { 0, LUCE_MAIN_ADDR, LUCE_MAIN_ADDR + 1, LUCE_MAIN_ADDR + 2, LUCE_MAIN_ADDR + 3 };  // Indirizzi per Dimmer, R, G, B, W
  dmx.attachLight(lightProfileMain);
  // indexMain = dmx.getProfilesCount(); // Ottengo l'indirizzo profilo della luce main

  // Aggiungo il profilo della luce Fuoco
  DMXLightMapping lightProfileFire = { 0, LUCE_FUOCO_ADDR, LUCE_FUOCO_ADDR + 1, LUCE_FUOCO_ADDR + 2, LUCE_FUOCO_ADDR + 3 };  // Indirizzi per Dimmer, R, G, B, W
  dmx.attachLight(lightProfileFire);
  // indexFire = dmx.getProfilesCount(); // Ottengo l'indirizzo profilo della luce fuoco

  // Aggiungo il profilo della luce Stella
  DMXLightMapping lightProfileStella = { 0, LUCE_STELLA_ADDR, LUCE_STELLA_ADDR + 1, LUCE_STELLA_ADDR + 2, LUCE_STELLA_ADDR + 3 };  // Indirizzi per Dimmer, R, G, B, W
  dmx.attachLight(lightProfileStella);
  // indexStella = dmx.getProfilesCount(); // Ottengo l'indirizzo profilo della luce fuoco

  //Aggiungo eventuali byte fissi per gestire le luci - in accordo con il manuale d'uso
  //dmx.setFixedByte(LUCE_MAIN_ADDR + IndexAddress, value);


  //Imposto il colore di defualt per le luci che userò per le stelle, se non si imposta di base è tutto bianco
  dmx.attachDefColor(170, 200, 252, 0);

  //Imposto il colore di giorno
  DayColor.R = 245;
  DayColor.G = 255;
  DayColor.B = 35;
  DayColor.W = 0;

  //Imposto il colore della notta
  NightColor.R = 20;
  NightColor.G = 50;
  NightColor.B = 255;
  NightColor.W = 0;

  //Imposto il colore del fuoco
  FireColor.R = 255;
  FireColor.G = 100;
  FireColor.B = 0;
  FireColor.W = 0;
}
// Funzione per gestire il cambiamento di colore basato su un valore PWM
void fadeColorPWM(unsigned int addrLight, Color Start, Color End, unsigned int pwmValue) {
  // Assicurati che il valore PWM sia nel range corretto
  pwmValue = constrain(pwmValue, 0, 1024);

  // Calcola la percentuale di completamento basata sul valore PWM
  float progress = pwmValue / 1024.0;

  // Calcola i valori interpolati di R, G e B
  byte r = Start.R + (End.R - Start.R) * progress;
  byte g = Start.G + (End.G - Start.G) * progress;
  byte b = Start.B + (End.B - Start.B) * progress;

  // Scrivi il valore interpolato tramite DMX
  dmx.digitalDmxWrite(addrLight, r, g, b, 0);
}

void fadeColorWithFlicker(unsigned int addrLight, Color BaseColor, unsigned int intensity) {
  // Genera una variazione casuale per simulare il tremolio
  int flicker = random(-20, 20);

  // Applica il tremolio mantenendo i valori RGB nel range valido
  byte r = constrain(BaseColor.R + flicker, 0, 255);
  byte g = constrain(BaseColor.G + flicker, 0, 255);
  byte b = constrain(BaseColor.B + flicker, 0, 255);

  // Scrivi i valori con tremolio al dispositivo tramite DMX
  dmx.digitalDmxWrite(addrLight, r, g, b, intensity);
}


//Gestione alba con la transizione da notte a giorno
void alba(unsigned long durata) {
  unsigned long startTime = millis();  // Tempo di inizio

  while ((millis() - startTime) < durata) {
    // Calcola il valore PWM per la transizione giorno/notte
    unsigned int pwmValue = map(millis() - startTime, 0, durata, 0, 1024);

    // Aggiorna i colori dal colore notturno a quello diurno
    fadeColorPWM(LUCE_MAIN_ADDR, NightColor, DayColor, pwmValue);

    // Se siamo a metà transizione, inizia la transizione del "fuoco"
    if (millis() >= (startTime + (durata / 2))) {
      unsigned int pwmValueFire = map(millis() - (startTime + (durata / 2)), 0, durata / 2, 0, 1024);

      // Aggiorna i colori del fuoco al colore spento con tremolio
      Color fireTransitionColor = {
        FireColor.R * (1024 - pwmValueFire) / 1024,
        FireColor.G * (1024 - pwmValueFire) / 1024,
        FireColor.B * (1024 - pwmValueFire) / 1024
      };
      fadeColorWithFlicker(LUCE_MAIN_ADDR, fireTransitionColor, pwmValueFire);
    }
  }
}

void giorno(unsigned long durata) {
  dmx.digitalDmxWrite(addrLinght, DayColor.R, DayColor.G, DayColor.B, 0);
  //Qui siccome il colore rimarrà statico occorre forzare il dmx.update()
  unsigned long startDaty = millis();
  while ((millis() - startDaty) < durata)){
      dmx.update();
      TASKDELAY(50);  // Ritardo 50ms tra un update e l'altro
    }
}

void tramonto(unsigned long durata) {
  unsigned long startTime = millis();  // Tempo di inizio

  while ((millis() - startTime) < durata) {
    // Calcola il valore PWM per la transizione giorno/notte
    unsigned int pwmValue = map(millis() - startTime, 0, durata, 0, 1024);

    // Aggiorna i colori dal colore diurno a quello notturno
    fadeColorPWM(LUCE_MAIN_ADDR, DayColor, NightColor, pwmValue);

    // Se siamo a metà transizione, inizia la transizione del "fuoco"
    if (millis() >= (startTime + (durata / 2))) {
      unsigned int pwmValueFire = map(millis() - (startTime + (durata / 2)), 0, durata / 2, 0, 1024);

      // Aggiorna i colori dal colore spento al colore del fuoco con tremolio
      Color fireTransitionColor = {
        FireColor.R * pwmValueFire / 1024,
        FireColor.G * pwmValueFire / 1024,
        FireColor.B * pwmValueFire / 1024
      };
      fadeColorWithFlicker(LUCE_MAIN_ADDR, fireTransitionColor, pwmValueFire);
    }
  }
}


void notte(unsigned long durata) {
  unsigned long startDaty = millis();
  // Applica il colore statico notturno
  dmx.digitalDmxWrite(LUCE_MAIN_ADDR, NightColor.R, NightColor.G, NightColor.B, 0);
  while ((millis() - startDaty) < durata) {

    // Simula il tremolio del fuoco durante la notte
    fadeColorWithFlicker(LUCE_MAIN_ADDR, FireColor, 255);

    // Randomizza l'accensione/spegnimento della luce che simula il flash di una stella
    if (random(0, 100) < 35) {                      // 35% di probabilità ogni ciclo
      dmx.digitalDmxWrite(LUCE_STELLA_ADDR, HIGH);  // Accende la luce stella con il colore impostato di defualt
      TASKDELAY(random(50, 350));                  // Ritardo casuale tra 50 e 350ms
      dmx.digitalDmxWrite(LUCE_STELLA_ADDR, LOW);
    } else {
      // Ritardo per controllare la frequenza dell'aggiornamento
      TASKDELAY(50);
    }
  }
}

void loop() {
  alba(5000);      //5S di alba
  giorno(15000);   // 15s di giorno
  tramonto(5000);  //5S di tramonto
  notte(15000);    //15S di notte
}
