const uint8_t pul = 8;
const uint8_t dir = 7;

int vel = "";
void setup() {
  pinMode(pul,OUTPUT);
  pinMode(dir,OUTPUT);
  Serial.begin(9600);
  while(Serial.available() == 0){
    
  }
  vel = (Serial.readString()).toInt();
  Serial.print("Girando a vel: ");
  Serial.print(vel);
  Serial.println(" RPM");
  digitalWrite(dir,HIGH);
  vel = 2500/(vel/60);
}

void loop() {

  digitalWrite(pul,HIGH);
  delayMicroseconds(vel);
  digitalWrite(pul,LOW);
  delayMicroseconds(vel);

}
