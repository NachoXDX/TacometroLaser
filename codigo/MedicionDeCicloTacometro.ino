
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

uint8_t pinEm = A1;
uint8_t pinRec = 2;
uint8_t pinBtn = A4;
unsigned long duration[50];
unsigned long inTime = 0;

void setup() {
Serial.begin(9600);
pinMode(pinEm,OUTPUT);
  pinMode(pinRec,INPUT_PULLUP);
  pinMode(pinBtn,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinRec),recISR,RISING);
  digitalWrite(pinEm,HIGH);
}

void loop() {
 if(digitalRead(pinBtn)){
    delay(100);
    enabled = true;
    sampleTimer = millis();
 }
 
 if(enabled && millis() - sampleTimer >= 2000){
  enabled = false;
  
  for(int i = 0; i<measureCount;i++){
    Serial.println(duration[i]);
  }
  if(measureCount != 0){
    sum = 0;
    mean = 0;
    sqSum = 0;
    cv = 0;
    for(int i = 0; i < measureCount; i++){
        sum += duration[i];
    }
    mean = (sum)/(measureCount);

    for(int i = 1; i < measureCount; i++){
        sqSum += (measures[i] - mean)*(measures[i] - mean);
    }
    
    cv = sqrt(sqSum/(measureCount))/mean;

    Serial.println("------------------");
    Serial.print("Media: ");
    Serial.println(mean);
    Serial.print("CV: ");
    Serial.println(cv);
    Serial.println("------------------");
    
  }

  measureCount = 0;
  for(int i = 0; i<400;i++){
    measures[i] = 0;
  }
 }
}


void recISR(void){
  
  if (enabled){
    inTime = micros();
    if(micros() - interruptTimer >= 100 && measureCount <= 399){
    interruptTimer = micros();
    measures[measureCount] = 60000000.0/(interruptTimer - lastTimeMeasure);
    lastTimeMeasure = interruptTimer;
    measureCount++;  
    duration[measureCount] = micros() - inTime;  
    }
  }  
}
