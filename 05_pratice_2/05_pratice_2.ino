#define PIN_LED 7


void setup() {
  pinMode(PIN_LED, OUTPUT);

}

void loop() {
  digitalWrite(PIN_LED,0);
  delay(1000);

  for(int i=0; i<5;i++){
    digitalWrite(PIN_LED,LOW);
    delay(200);
    digitalWrite(PIN_LED,HIGH);
    delay(200);
  }
  digitalWrite(PIN_LED,1);
  while(1){}
}
