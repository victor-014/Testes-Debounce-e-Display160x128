#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735

// Pinos do Display LCD 16x02
#define CS  PB12
#define DC  PB10
#define RST PB11
#define SDA PB15  //MOSI
#define SCK PB13 
// SPI2 hardware, Obs: Pode ser usado o mesmo SPI do Lora, mas o RST e o CS devem ser diferntes
SPIClass SPI_2(SDA, PB14, SCK);
Adafruit_ST7735 tft(&SPI_2, CS, DC, RST);


// Botões
#define botaoUp     PA10 // aumenta
//#define CountDown   PA11 // diminui
//#define Acionamento PA12 // aciona

int contagem = 0;
bool acionado = false;
unsigned long tempo = 0;
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
    if (digitalRead(botaoUp) == HIGH && bordaCountUp){
        countUp++;
        if (countUp == 20){
            IncrementaContagem();
            bordaCountUp = false;
        }
    }
}

void bordaBotaoUp(){
    countUp = 0;
    bordaCountUp = true;
}

void setup() {

    Serial.begin(115200);
    SPI_2.begin();

    Serial.println("Display inicializado!");

    tft.initR(INITR_BLACKTAB);
    tft.setRotation(3);
    
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setTextSize(3);
    pinMode(botaoUp, INPUT);
    //pinMode(CountDown, INPUT);
    //pinMode(Acionamento, INPUT);

    Timer.setOverflow(1000,MICROSEC_FORMAT);
    Timer.attachInterrupt(debounceCountUp);
    Timer.resume();

    attachInterrupt(digitalPinToInterrupt(botaoUp), bordaBotaoUp, RISING);
}

void loop() {
}