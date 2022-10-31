#include "aSoftPWM.h"

//#define DEBUG_AMOR 1

#define ANALOG_READ_PIN A0

#define PWM_LEVELS 254

//#define ANALOG_READ_FUNC 1
#ifdef ANALOG_READ_FUNC
volatile uint32_t FRAME_RATE = 1;
volatile uint32_t FRAME_RATE_DATA = 1;
#else
//  #define FRAME_RATE 3286 //3150
//  #define FRAME_RATE_DATA 1100 //3150

volatile uint32_t FRAME_RATE = 3286;
volatile uint32_t FRAME_RATE_DATA = 1100;
#endif

#define CORSERNESS 1
#define CORSERNESS_IN 1
#define CORSERNESS_OUT 1

#define PWM_FREQUNCY 120

#define wave_array_len 10
//////uint8_t wave_array[] = {8,12,22,34,44,56,66,76,88,100};  // liner
uint8_t wave_array[] = {8, 22, 44, 66, 88, 100, 76, 56, 34, 10}; // inverted V

//uint8_t wave_array[] = {22,66,100,66,22,22,66,100,66,22}; // inverted V
//uint8_t wave_array[] = {12,56,100,56,12,12,56,100,56,12}; // inverted V

//uint8_t wave_array[] = {12,56,100,56,12,56,100,56,12,6}; // inverted V

//#define wave_array_len 10
//uint8_t wave_array[] = {0,0,0,10,50,70,90,100,0,0};

SOFTPWM_DEFINE_CHANNEL(0, DDRD, PORTD, PORTD0); //Arduino pin 0
SOFTPWM_DEFINE_CHANNEL(1, DDRD, PORTD, PORTD1); //Arduino pin 1

SOFTPWM_DEFINE_CHANNEL(2, DDRD, PORTD, PORTD2);  //Arduino pin 2
SOFTPWM_DEFINE_CHANNEL(3, DDRD, PORTD, PORTD3);  //Arduino pin 3
SOFTPWM_DEFINE_CHANNEL(4, DDRD, PORTD, PORTD4);  //Arduino pin 4
SOFTPWM_DEFINE_CHANNEL(5, DDRD, PORTD, PORTD5);  //Arduino pin 5
SOFTPWM_DEFINE_CHANNEL(6, DDRD, PORTD, PORTD6);  //Arduino pin 6
SOFTPWM_DEFINE_CHANNEL(7, DDRD, PORTD, PORTD7);  //Arduino pin 7
SOFTPWM_DEFINE_CHANNEL(8, DDRB, PORTB, PORTB0);  //Arduino pin 8
SOFTPWM_DEFINE_CHANNEL(9, DDRB, PORTB, PORTB1);  //Arduino pin 9
SOFTPWM_DEFINE_CHANNEL(10, DDRB, PORTB, PORTB2); //Arduino pin 10
SOFTPWM_DEFINE_CHANNEL(11, DDRB, PORTB, PORTB3); //Arduino pin 11

SOFTPWM_DEFINE_CHANNEL(12, DDRB, PORTB, PORTB4); //Arduino pin 12
SOFTPWM_DEFINE_CHANNEL(13, DDRB, PORTB, PORTB5); //Arduino pin 13

SOFTPWM_DEFINE_OBJECT_WITH_PWM_LEVELS(14, PWM_LEVELS);

SOFTPWM_DEFINE_EXTERN_OBJECT_WITH_PWM_LEVELS(14, PWM_LEVELS);

static volatile uint8_t temp_v_array_current[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static volatile uint8_t temp_v_array_desired[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int8_t wave_array_counter = 0;
int8_t displacer_couter = 0;

int8_t led_counter = 0;
int8_t place_counter = 0;

void update_all_leds_simultanesouly();

#ifdef DEBUG_AMOR
void array_status()
{
  for (int j = 0; j < 14; j++)
  {
    Serial.print(temp_v_array_desired[j]);
    Serial.print("\t");
  }
  Serial.print("\n");
}
#endif

void update_one_level_of_all_leds()
{
  for (uint8_t i = 2; i < 12; i++)
  {

    if (temp_v_array_current[i] < temp_v_array_desired[i])
    {
      temp_v_array_current[i] += (CORSERNESS_IN * CORSERNESS);
    }
    else if (temp_v_array_current[i] > temp_v_array_desired[i])
    {
      temp_v_array_current[i] -= (CORSERNESS_OUT * CORSERNESS);
    }
    else
    {
      ; // do nothig on same level
    }
  }

  update_all_leds_simultanesouly();
}

void update_all_leds_simultanesouly()
{

  for (uint8_t i = 2; i < 12; i++)
  {
    Palatis::SoftPWM.set(i, temp_v_array_current[i]);
  }

  delayMicroseconds(FRAME_RATE);
}

void data_inject()
{
  delayMicroseconds(FRAME_RATE_DATA);

  for (uint8_t i = 2; i < 12; i++)
  {
    //    Serial.print(wave_array[wave_array_counter + displacer_couter]);
    //    Serial.print("\t");

    //        Palatis::SoftPWM.set(i, wave_array[wave_array_counter + displacer_couter]);
    //    set_custom_pin_pwm(i, wave_array[wave_array_counter + displacer_couter]);

    temp_v_array_desired[i] = (PWM_LEVELS / 100.0) * wave_array[wave_array_counter + displacer_couter];

    wave_array_counter++;

    if ((wave_array_counter + displacer_couter) == (unsigned)wave_array_len)
    {
      wave_array_counter = -displacer_couter;
    }

    //    delayMicroseconds(4*4000);
    //        delay(LED_GAP_TIME);
  }

  displacer_couter++;

  if (displacer_couter == wave_array_len)
  {
    displacer_couter = 0;
  }

  bool flag_leveled = false;

  while (!flag_leveled)
  {
    update_one_level_of_all_leds();
    flag_leveled = true;

    for (uint8_t i = 0; i < 14; i++)
    {
      if (temp_v_array_desired[i] != temp_v_array_current[i])
      {
        flag_leveled = false;
        break;
      }
    }
  }
}

void setup()
{

#ifdef DEBUG_AMOR
  Serial.begin(57600);
#endif

  // begin with 60hz pwm frequency
  Palatis::SoftPWM.begin(PWM_FREQUNCY);

#ifdef DEBUG_AMOR
  // print interrupt load for diagnostic purposes
  Palatis::SoftPWM.printInterruptLoad();
#endif
}

void all_on(uint8_t brightness = PWM_LEVELS)
{
  for (uint8_t i = 2; i < 12; i++)
  {
    Palatis::SoftPWM.set(i, brightness);
  }
}

void all_off()
{
  for (uint8_t i = 2; i < 12; i++)
  {
    Palatis::SoftPWM.set(i, 0);
  }
}


void switch_on_nleds(uint8_t nLeds){
  
    for (uint8_t i = 11; i > 1; i--)
      {
        Palatis::SoftPWM.set(i, 0);
      }
  
    for (uint8_t i = 11; i > (11 - nLeds); i--)
      {
        Palatis::SoftPWM.set(i, PWM_LEVELS);
      }
    
  }

#ifdef ANALOG_READ_FUNC
uint8_t aRead_counter = 0;
void analog_reads()
{
  uint8_t a0 = analogRead(A0);
  uint8_t a1 = analogRead(A1);

  FRAME_RATE = 4 * a0;
  FRAME_RATE_DATA = 20 * a1;

#ifdef DEBUG_AMOR
  Serial.print(a0);
  Serial.print("FRAME_RATE =");
  Serial.print(FRAME_RATE);
  Serial.print("\t");
  Serial.print(a1);
  Serial.print("FRAME_RATE_DATA=");
  Serial.println(FRAME_RATE_DATA);
#endif
}
#endif

uint8_t loop_counter = 0;

unsigned long blinking_millis = 0;

uint8_t isFirstFlikerFlag = 0;

//unsigned long full_on_millis = 0 ;

void loop()
{

#ifdef DEBUG_AMOR
  array_status();
#endif

#ifdef ANALOG_READ_FUNC
  aRead_counter++;
  if (aRead_counter == 5)
  {
    analog_reads();
    aRead_counter = 0;
  }
#endif

  data_inject();
  

  if (isFirstFlikerFlag == 0)
  {
    FRAME_RATE = 4000 * 1.0;
    FRAME_RATE_DATA = 1500 * 1.0;
  }
  else if (isFirstFlikerFlag == 1)
  {
     FRAME_RATE = 4000 * 0.9;
    FRAME_RATE_DATA = 1500 * 0.9;
  }
  else if (isFirstFlikerFlag == 2)
  {
     FRAME_RATE = 4000 * 0.8;
    FRAME_RATE_DATA = 1500 * 0.8;
  }
  else if (isFirstFlikerFlag == 3)
  {
     FRAME_RATE = 4000 * 0.7;
    FRAME_RATE_DATA = 1500 * 0.7;
  }
  else if (isFirstFlikerFlag == 4)
  {
    FRAME_RATE = 4000 * 0.6;
    FRAME_RATE_DATA = 1500 * 0.6;
  }
  else if (isFirstFlikerFlag == 5)
  {

     FRAME_RATE = 4000 * 0.5;
    FRAME_RATE_DATA = 1500 * 0.5;
  }
    else if (isFirstFlikerFlag == 6)
  {

    FRAME_RATE = 4000 * 0.4;
    FRAME_RATE_DATA = 1500 * 0.4;
    
  }
      else if (isFirstFlikerFlag == 7)
  {

    FRAME_RATE = 4000 * 0.3;
    FRAME_RATE_DATA = 1500 * 0.3;
    
  }
      else if (isFirstFlikerFlag == 8)
  {

    FRAME_RATE = 4000 * 0.2;
    FRAME_RATE_DATA = 1500 * 0.2;
    
  }

  if (millis() > blinking_millis + 8000)
  { // 45000

    switch_on_nleds(isFirstFlikerFlag + 1);
    delay(2500);
    
    for (uint8_t j = 0; j < PWM_LEVELS; j++)
    {
      all_on(j);
      delay(4);
    }
#ifdef DEBUG_AMOR
    Serial.println("all_on leds");
    Serial.println(FRAME_RATE);
    Serial.println(FRAME_RATE_DATA);
    Serial.println(isFirstFlikerFlag);
#endif
    
    isFirstFlikerFlag = isFirstFlikerFlag + 1 ;
    delay(isFirstFlikerFlag * 1000); //26000
    
    if(isFirstFlikerFlag == 9){
        isFirstFlikerFlag = 0;
      }
    blinking_millis = millis();
  }

  //    loop_counter++;

  //    delay(100);
  if (loop_counter == 20)
  {
    for (uint8_t j = 100; j > 10; j--)
    {
      all_on(j);
      delay(17);

      //            all_on(50);
      //            delay(700);
      //
      //            all_on(10);
      //            delay(700);
    }
    loop_counter = 0;
  }
}
