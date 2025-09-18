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
#define CountUp     PA10 // aumenta
//#define CountDown   PA11 // diminui
//#define Acionamento PA12 // aciona


int contagem = 0;
bool acionado = false;
unsigned long tempo = 0;
bool CountUpAnterior = LOW;
bool CountUpAtual = LOW; //verifica se o botão foi clicado e liberado (Anterior = HIGH e Atual = LOW)

void setup() {
  Serial.begin(115200);
  SPI_2.begin();
  delay(5000);

  Serial.println("Display inicializado!");

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setTextSize(2);
  pinMode(CountUp, INPUT);
  //pinMode(CountDown, INPUT_PULLUP);
  //pinMode(Acionamento, INPUT_PULLUP);

  tempo = millis();
}

void display() {
  tft.setCursor(0, 0);
  tft.print("    "); // reseta contagem de até 4 digitos
  tft.setCursor(0, 0);
  tft.print(contagem);
  tft.print("s");
}

void IncrementaContagem(){
  contagem++;
  CountUpAnterior = false;
  CountUpAtual = false;
  display();
}

void loop() {
  CountUpAtual = digitalRead(CountUp);
  delay(20);

  // detecta a borda de descida (HIGH → LOW)
  if (CountUpAnterior == HIGH && CountUpAtual == LOW) {
    IncrementaContagem();
  }
  
  CountUpAnterior = CountUpAtual;

}
