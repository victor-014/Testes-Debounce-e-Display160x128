#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <STM32FreeRTOS.h>

// Pinos do Display LCD 16x02
#define CS  PB12
#define DC  PB10
#define RST PB11
#define SDA PB15  //MOSI
#define SCK PB13 
// SPI2 hardware, Obs: Pode ser usado o mesmo SPI do Lora, mas o RST e o CS devem ser diferntes
SPIClass SPI_2(SDA, PB14, SCK);
Adafruit_ST7735 tft(&SPI_2, CS, DC, RST);


// Botões para contagem
#define botaoUp     PA10 // aumenta
#define botaoDown   PA11 // diminui
#define botaoAciona PA12 // aciona
#define botaoAborta PA15 // aborta

int botao[4] = {botaoUp, botaoDown, botaoAciona, botaoAborta};

bool leitura;
int contagem = 0;
int contagemSelecionada = 0;
bool acionado = false;
unsigned long tempo = 0;
bool borda[4] = {0, 0, 0, 0};
int count[4] = {0, 0, 0, 0};
bool contagemAcionada = false;
uint8_t text_size = 2;

HardwareTimer Timer(TIM2);

void display() {
    tft.setCursor(8*7*text_size+1, 7*text_size+1); //são usados 8x8 pixels da tela para um caractere (pixels 0-7 para 1° coluna/linha)
    tft.print("    "); // reseta contagem de até 4 digitos
    tft.setCursor(8*7*text_size+1, 7*text_size+1);
    tft.print(contagem);
    tft.print("s");
}

void incrementaContagem(){
    contagem++;
    display();
}

void decrementaContagem(){
    contagem--;
    display();
}

static void acionaContagem(void *pvParameters){
    for(;;){
        if(contagemAcionada){
            contagemSelecionada = contagem;
            while(contagem>0){
                vTaskDelay(1);
                decrementaContagem();
            }
            contagemAcionada = false;
        }
    }
}

static void abortaContagem(void *pvParameters){
    for(;;){
        if(abortaContagem){
            contagemAcionada = false;
            contagem = contagemSelecionada;
        }
    }
}

void debounce(){
    for (int i=0; i<4; i++){
        leitura = digitalRead(botao[i]);
        if (leitura == HIGH && borda[i]){
            count[i]++;
            if (count[i] == 20){
                switch(i){
                    case 0:
                        incrementaContagem();
                        break;
                    case 1:
                        decrementaContagem();
                        break;
                    case 2:
                        contagemAcionada = true;
                        break;
                    case 3:
                        contagemAcionada = false;
                        contagem = contagemSelecionada;
                        break;
                }
                borda[i] = false;
            }
        }
    }
}

void BordaBotaoUp()     { borda[0] = true; count[0] = 0; }
void BordaBotaoDown()   { borda[1] = true; count[1] = 0; }
void BordaBotaoAciona() { borda[2] = true; count[2] = 0; }
void BordaBotaoAborta() { borda[3] = true; count[3] = 0; }

void setup() {

    Serial.begin(115200);
    SPI_2.begin();

    Serial.println("Display inicializado!");

    tft.initR(INITR_BLACKTAB);
    tft.setRotation(3);
    
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setTextSize(text_size);
    
    tft.setCursor(0, 0);
    tft.print("Escolha a\nContagem:");

    for (int i=0; i<4; i++){
        pinMode(botao[i], INPUT);
    }

    Timer.setOverflow(1000,MICROSEC_FORMAT);
    Timer.attachInterrupt(debounce);
    Timer.resume();

    attachInterrupt(digitalPinToInterrupt(botaoUp), BordaBotaoUp, RISING);
    attachInterrupt(digitalPinToInterrupt(botaoDown), BordaBotaoDown, RISING);
    attachInterrupt(digitalPinToInterrupt(botaoAciona), BordaBotaoAciona, RISING);
    attachInterrupt(digitalPinToInterrupt(botaoAborta), BordaBotaoAborta, RISING);

    xTaskCreate(acionaContagem, "Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(abortaContagem, "Task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);

    vTaskStartScheduler();
}

void loop() {
}