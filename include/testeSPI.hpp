#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

// Pinos padrão do Display LCD 16x02
// #define CS  PB12
// #define DC  PB13
// #define RST PB14
// #define SDA PA7    // MOSI
// #define SCK PA5

#define CS  PB12
#define DC  PB10
#define RST PB11
#define SDA PB15  //MOSI
#define SCK PB13 

// Usa o SPI2 hardware
SPIClass SPI_2(SDA, PB14, SCK);
Adafruit_ST7735 tft(&SPI_2, CS, DC, RST);
//Adafruit_ST7735 tft = Adafruit_ST7735(CS,  DC, RST);

int contagem = 0;
bool acionado = false;
unsigned long tempo = 0;
bool clicadoCountUp = false; //primeiro o botão é pressionado
bool soltadoCountUp = false; //em seguida é solto e é incrementada a contagem

void setup() {
  Serial.begin(115200);
  SPI_2.begin();
  delay(5000);

  Serial.println("Display inicializado!");

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  
  tft.fillScreen(ST77XX_BLACK);
  //tft.setTextColor(ST77XX_WHITE);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK); // texto branco, fundo preto
  tft.setTextSize(4);
}

void display() {
  // tft.setCursor(0, 0);
  // tft.print("    "); // reseta contagem de até 4 digitos
  tft.fillRect(0, 0, 80, 20, ST77XX_BLACK);  // limpa região
  tft.setCursor(0, 0);
  tft.print(contagem);
  tft.print("s");
}

void IncrementaContagem(){
  contagem++;
  clicadoCountUp = false;
  soltadoCountUp = false;
  display();
}

void loop() {
  if ((millis()-tempo)>1000){
    tempo=millis();
    Serial.println(contagem);
    IncrementaContagem();
  }
}
