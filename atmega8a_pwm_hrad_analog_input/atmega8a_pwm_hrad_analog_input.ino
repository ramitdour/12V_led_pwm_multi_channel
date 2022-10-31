


void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);

}

void loop() {
  // put your main code here, to run repeatedly:

  uint8_t a0 = analogRead(A0);
  uint8_t a1 = analogRead(A1);
  
  analogWrite(10,a0);
  analogWrite(11,a1);

  Serial.print(a0);
  Serial.print(" \t");
  Serial.print(a1);
  Serial.println();
  

}
