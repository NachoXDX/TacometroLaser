#include <LiquidCrystal.h>
const uint8_t rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
uint8_t pinEm = A1;
uint8_t pinRec = 2;
uint8_t pinBtn = A4;

unsigned long interruptTimer = 0;
unsigned long lastTimeMeasure = 0;
double rpm = 0;
bool enabled = false;
int measures[400];
unsigned int measureCount = 0;
float sum = 0;
float mean = 0;
float sqSum = 0;
float cv = 0;

unsigned int measuresSum = 0;
unsigned long sampleTimer = 0;

void setup() {
//  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(pinEm,OUTPUT);
  pinMode(pinRec,INPUT_PULLUP);
  pinMode(pinBtn,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinRec),recISR,RISING);
  lcd.setCursor(0,0);
  lcd.print(" Bienvenido  :)");
  delay(1000);
  digitalWrite(pinEm,HIGH);
  lcd.setCursor(0,0);
  lcd.print("Presionar Btn");
}

void loop() {
 if(digitalRead(pinBtn)){
    delay(100);
    enabled = true;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Midiendo");
    sampleTimer = millis();
 }
 
 if(enabled && millis() - sampleTimer >= 2000){
  enabled = false;
//  for(int i = 0; i<measureCount;i++){
//    Serial.println(measures[i]);
//  }
  if(measureCount != 0){
    sum = 0;
    mean = 0;
    sqSum = 0;
    cv = 0;
    for(int i = 1; i < measureCount; i++){
        sum += measures[i];
    }
    mean = (sum)/(measureCount-1);

    for(int i = 1; i < measureCount; i++){
        sqSum += (measures[i] - mean)*(measures[i] - mean);
    }
    
    cv = sqrt(sqSum/(measureCount-1))/mean;
//    debug
//    Serial.println("-----------------------");
//    Serial.print("Media: ");
//    Serial.println(mean);
//    Serial.print("CV: ");
//    Serial.println(cv);
//    Serial.print("Valor: ");
//    Serial.println(mean);
//    Serial.println("-----------------------");

  
// fin debug
    if(cv <= 0.15){
        refreshScreen(mean);
    }
    else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Mala Medicion");
    lcd.setCursor(0,1);
    lcd.print("Tocar Btn");
    }
    
  }
  else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sin Mediciones");
    lcd.setCursor(0,1);
    lcd.print("Tocar Btn");
  }
  measureCount = 0;
  for(int i = 0; i<400;i++){
    measures[i] = 0;
  }
 }
}


void recISR(void){
  if (enabled){
    if(micros() - interruptTimer >= 100 && measureCount <= 399){
    interruptTimer = micros();
    measures[measureCount] = 60000000.0/(interruptTimer - lastTimeMeasure);
    lastTimeMeasure = interruptTimer;
    measureCount++;  
 }  
  }  
}


void refreshScreen(long rpm){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   Velocidad:  ");
  lcd.setCursor(0, 1);
  lcd.print(rpm);
  lcd.print(" rpm");
}
