#include "DHT.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DHTTYPE DHT11   // Tipo do Sensor DHT 11
// DHT Sensor (Pino 4)
uint8_t DHTPin = 4; 
               
// Inicialização do sensor DHT.
DHT dht(DHTPin, DHTTYPE);                

float Temperature;
float Humidity;
float temp1;
float temp2;
float temp3;
int i=0;
int count=0;

const int pinButton = 2; //pino do botão1
const int pinButton2 = 15; //pino do botão2

//Timer
hw_timer_t * timer0 = NULL;
hw_timer_t * timer1 = NULL;
portMUX_TYPE timerMux0 = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timerMux1 = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer0(){
  // Critical Code here
  portENTER_CRITICAL_ISR(&timerMux0);
  
  Serial.println("30s - TEMPORIZADOR LEITURA TEMPERATURA: ON");
  i = 1;
//  Serial.println(i);
  
  portEXIT_CRITICAL_ISR(&timerMux0);
}

void IRAM_ATTR onTimer1(){
  // Critical Code here
  portENTER_CRITICAL_ISR(&timerMux1);

  Serial.println("10s - TEMPORIZADOR BACKLIGHT: OFF");
  count = 1;
//  Serial.println(count);
  
  portEXIT_CRITICAL_ISR(&timerMux1);
}

//Botao 1
void botao1(){
  lcd.backlight();
  lcd.clear();// clear previous values from screen
  lcd.setCursor(0,0);
  lcd.print("BOTAO1 LER AGORA!");
  temp3 = dht.readTemperature();
  temp2 = 0.00;
  temp1 = 0.00;
  lcd.setCursor(0,1);
  lcd.print(temp3);
  lcd.setCursor(4,1);
  lcd.print("  ");
  lcd.setCursor(6,1);
  lcd.print(temp2);
  lcd.setCursor(10,1);
  lcd.print("  ");
  lcd.setCursor(12,1);
  lcd.print(temp1);
  timerWrite(timer0, 0); //reseta o temporizador0  
  timerWrite(timer1, 0); //reseta o temporizador1  
//  delay(3000);
}

//Botao2
void botao2() {
  lcd.backlight();
  lcd.clear();
  temp1=0;
  temp2=0;
  temp3=0;
  lcd.setCursor(0,0);
  lcd.print("Painel RESETADO!");
  lcd.setCursor(0,1);
  lcd.print(temp3);
  lcd.setCursor(4,1);
  lcd.print("  ");
  lcd.setCursor(6,1);
  lcd.print(temp2);
  lcd.setCursor(10,1);
  lcd.print("  ");
  lcd.setCursor(12,1);
  lcd.print(temp1);
  timerWrite(timer0, 0); //reseta o temporizador0  
  timerWrite(timer1, 0); //reseta o temporizador1  
}
 
void setup() {
  Serial.begin(115200);
  //Define os botoes como entrada
  pinMode(pinButton, INPUT_PULLUP);
  pinMode(pinButton2, INPUT_PULLUP);

//Inicializa os timers
  Serial.println("start timer 1");
  timer1 = timerBegin(1, 80, true);  // timer 1, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
  timerAttachInterrupt(timer1, &onTimer1, true); // edge (not level) triggered 
  timerAlarmWrite(timer1, 10000100, true); // 250000 * 1 us = 250 ms, autoreload true

  Serial.println("start timer 0");
  timer0 = timerBegin(0, 80, true);  // timer 0, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
  timerAttachInterrupt(timer0, &onTimer0, true); // edge (not level) triggered 
  timerAlarmWrite(timer0, 30000000, true); // 2000000 * 1 us = 2 s, autoreload true

  timerAlarmEnable(timer0); // enable
  timerAlarmEnable(timer1); // enable

  delay(100);

//Define o Sensor DHT como entrada  
  pinMode(DHTPin, INPUT);
  dht.begin();              

//Inicializa o LCD
  lcd.begin();  
  lcd.backlight();
  lcd.print("Projeto ESP32");
  lcd.setCursor(0,1);
  lcd.print("Sensor Temp. C");   
}

void loop() {
//  server.handleClient();

      if (count == 1) { //quando o timer10s é ativado, executa esta função
        lcd.noBacklight();
        lcd.clear();
        count = 0;
    }

    if (i == 1) { //quando o timer30s é ativado, executa esta função
        timerWrite(timer1, 0); //reseta o temporizador1  
        lcd.backlight(); 
        lcd.clear();
        lcd.print("Temp. Atual");
        lcd.setCursor(12,0);
        lcd.print(dht.readTemperature());
        printTemp();
        lcd.setCursor(0,1);
        lcd.print(temp3);
        lcd.setCursor(4,1);
        lcd.print("  ");
        lcd.setCursor(6,1);
        lcd.print(temp2);
        lcd.setCursor(10,1);
        lcd.print("  ");
        lcd.setCursor(12,1);
        lcd.print(temp1);
        i = 0;
    }
  
  //Botao1
      while(digitalRead(pinButton))
    {
        Serial.println("botão 1 pressionado = LER AGORA!");
        botao1();
    }
          while(digitalRead(pinButton2))
    {
        Serial.println("botão 2 pressionado = BOTÃO RESET!");
        botao2();
    }
}

void printTemp() {
temp1 = temp2;
temp2 = temp3;
temp3 = dht.readTemperature();
}
