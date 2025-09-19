#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
//#include "Adafruit_miniTFTWing.h"

// Pinos do Display LCD 16x02
#define CS  PB12
#define DC  PB10
#define RST PB11
#define SDA PB15  //MOSI
#define SCK PB13 
// Usa o SPI2 hardware
SPIClass SPI_2(SDA, PB14, SCK);
Adafruit_ST7735 tft(&SPI_2, CS, DC, RST);


// Botões
#define botaoUp     PA10 // aumenta
//#define CountDown   PA11 // diminui
//#define Acionamento PA12 // aciona


int contagem = 0;
bool acionado = false;
unsigned long tempo = 0;
bool estadoAnterior = LOW;
bool bordaCountUp = false;
int countUp = 0;

HardwareTimer Timer(TIM2);

void display() {
    tft.setCursor(0, 0);
    tft.print("    "); // reseta contagem de até 4 digitos
    tft.setCursor(0, 0);
    tft.print(contagem);
    tft.print("s");
}

void IncrementaContagem(){
    contagem++;
    display();
}

void debounceCountUp(){
    bool leituraCountUp = digitalRead(botaoUp);
    if (leituraCountUp != estadoAnterior){
        estadoAnterior = leituraCountUp;
        bordaCountUp = true;
        countUp = 0;
    } else if (leituraCountUp == HIGH && bordaCountUp){
        countUp++;
        if (countUp == 20){
            IncrementaContagem();
            bordaCountUp = false;
        }
    }
}

void setup() {

    Serial.begin(115200);
    SPI_2.begin();
    //delay(5000);

    Serial.println("Display inicializado!");

    tft.initR(INITR_BLACKTAB);
    tft.setRotation(3);
    
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setTextSize(3);
    pinMode(botaoUp, INPUT);
    //pinMode(CountDown, INPUT_PULLUP);
    //pinMode(Acionamento, INPUT_PULLUP);

    Timer.setOverflow(1000,MICROSEC_FORMAT);
    Timer.attachInterrupt(debounceCountUp);
    Timer.resume();
}

void loop() {
}