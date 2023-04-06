String val = "";
void setup() {
  Serial.begin(9600);
  pinMode(6,OUTPUT);
}

void loop() {
  if(Serial.available() > 0){
    val = Serial.readString();
    analogWrite(6,((val.toInt())*2.55));
  }

}
