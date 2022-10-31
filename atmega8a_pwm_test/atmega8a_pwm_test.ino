

uint8_t pin_nos[] = {3, 5, 6, 9, 10, 11};

uint8_t temp_level = 0;

uint8_t current_led = 0;


void analogWrite_pwm_led()
{
  analogWrite(pin_nos[current_led], temp_level);
  temp_level++;

  if (temp_level == 255)
  {
    temp_level = 0;
    current_led++;
    delay(100);

    if (current_led == 6)
    {
      current_led = 0;


      digitalWrite(12, HIGH);
      delay(2000);

      digitalWrite(12, LOW);
      // delay(1000);

      analogWrite(pin_nos[0], 0);
      analogWrite(pin_nos[1], 0);
      analogWrite(pin_nos[2], 0);
      analogWrite(pin_nos[3], 0);
      analogWrite(pin_nos[4], 0);
      analogWrite(pin_nos[5], 0);

      delay(2000);

      
      
    }
  }
}


void pwm_test_analog(){
    for(uint8_t i=0 ; i< 6; i++){
//      analogWrite(pin_nos[i], 255);
//      delay(500);
//
//      analogWrite(pin_nos[i], 128);
//      delay(500);
//
//      analogWrite(pin_nos[i], 64);
//      delay(500);
//
//      analogWrite(pin_nos[i], 14);
//      delay(500);

      
      for(uint8_t j=0 ;j <255 ;j++){
          analogWrite(pin_nos[i], j);
          delayMicroseconds(1500);
//          delay(1);
        }

      }
  }

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite_pwm_led();
pwm_test_analog();

}
