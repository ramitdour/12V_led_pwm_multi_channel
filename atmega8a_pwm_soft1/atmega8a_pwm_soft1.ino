#include <Ticker.h>

// #include "SoftPWM.h"

#include "aSoftPWM.h"

#define PWM_LEVELS 100

#define FADE_IN_T 1
#define FADE_OUT_T 1

#define FRAME_TIME 90
#define LED_GAP_TIME 1  //delayMicroseconds

static volatile uint8_t v = 0;

float freq = 0.80;
float phase = 0.2;

//uint8_t pin_nos[] = {3, 5, 6, 9, 10, 11};

// #include <SoftPWM.h>


void array_status();
/* pins_arduino.h defines the pin-port/bit mapping as PROGMEM so
   you have to read them with pgm_read_xxx(). That's generally okay
   for ordinary use, but really bad when you're writing super fast
   codes because the compiler doesn't treat them as constants and
   cannot optimize them away with sbi/cbi instructions.

   Therefore we have to tell the compiler the PORT and BIT here.
   Hope someday we can find a way to workaround this.

   Check the manual of your MCU for port/bit mapping.

   The following example demonstrates setting channels for all pins
   on the ATmega328P or ATmega168 used on Arduino Uno, Pro Mini,
   Nano and other boards. */

SOFTPWM_DEFINE_CHANNEL(0, DDRD, PORTD, PORTD0);  //Arduino pin 0
SOFTPWM_DEFINE_CHANNEL(1, DDRD, PORTD, PORTD1);  //Arduino pin 1
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
// SOFTPWM_DEFINE_CHANNEL(14, DDRC, PORTC, PORTC0);  //Arduino pin A0
// SOFTPWM_DEFINE_CHANNEL(15, DDRC, PORTC, PORTC1);  //Arduino pin A1
// SOFTPWM_DEFINE_CHANNEL(16, DDRC, PORTC, PORTC2);  //Arduino pin A2
// SOFTPWM_DEFINE_CHANNEL(17, DDRC, PORTC, PORTC3);  //Arduino pin A3
// SOFTPWM_DEFINE_CHANNEL(18, DDRC, PORTC, PORTC4);  //Arduino pin A4
// SOFTPWM_DEFINE_CHANNEL(19, DDRC, PORTC, PORTC5);  //Arduino pin A5

/* Or you may want inverted outputs: */
/*
  SOFTPWM_DEFINE_CHANNEL_INVERT(0, DDRD, PORTD, PORTD0);  //Arduino pin 0
  SOFTPWM_DEFINE_CHANNEL_INVERT(1, DDRD, PORTD, PORTD1);  //Arduino pin 1
  SOFTPWM_DEFINE_CHANNEL_INVERT(2, DDRD, PORTD, PORTD2);  //Arduino pin 2
  SOFTPWM_DEFINE_CHANNEL_INVERT(3, DDRD, PORTD, PORTD3);  //Arduino pin 3
  SOFTPWM_DEFINE_CHANNEL_INVERT(4, DDRD, PORTD, PORTD4);  //Arduino pin 4
  SOFTPWM_DEFINE_CHANNEL_INVERT(5, DDRD, PORTD, PORTD5);  //Arduino pin 5
  SOFTPWM_DEFINE_CHANNEL_INVERT(6, DDRD, PORTD, PORTD6);  //Arduino pin 6
  SOFTPWM_DEFINE_CHANNEL_INVERT(7, DDRD, PORTD, PORTD7);  //Arduino pin 7
  SOFTPWM_DEFINE_CHANNEL_INVERT(8, DDRB, PORTB, PORTB0);  //Arduino pin 8
  SOFTPWM_DEFINE_CHANNEL_INVERT(9, DDRB, PORTB, PORTB2);  //Arduino pin 9
  SOFTPWM_DEFINE_CHANNEL_INVERT(10, DDRB, PORTB, PORTB2);  //Arduino pin 10
  SOFTPWM_DEFINE_CHANNEL_INVERT(11, DDRB, PORTB, PORTB3);  //Arduino pin 11
  SOFTPWM_DEFINE_CHANNEL_INVERT(12, DDRB, PORTB, PORTB4);  //Arduino pin 12
  SOFTPWM_DEFINE_CHANNEL_INVERT(13, DDRB, PORTB, PORTB5);  //Arduino pin 13
  SOFTPWM_DEFINE_CHANNEL_INVERT(14, DDRC, PORTC, PORTC0);  //Arduino pin A0
  SOFTPWM_DEFINE_CHANNEL_INVERT(15, DDRC, PORTC, PORTC1);  //Arduino pin A1
  SOFTPWM_DEFINE_CHANNEL_INVERT(16, DDRC, PORTC, PORTC2);  //Arduino pin A2
  SOFTPWM_DEFINE_CHANNEL_INVERT(17, DDRC, PORTC, PORTC3);  //Arduino pin A3
  SOFTPWM_DEFINE_CHANNEL_INVERT(18, DDRC, PORTC, PORTC4);  //Arduino pin A4
  SOFTPWM_DEFINE_CHANNEL_INVERT(19, DDRC, PORTC, PORTC5);  //Arduino pin A5
*/

/* Here you make an instance of desired channel counts you want
   with the default 256 PWM levels (0 ~ 255). */
//SOFTPWM_DEFINE_OBJECT(20);

/* Or you can make one with only 100 PWM levels (0 ~ 99).
   By using less PWM levels, you may be able to use higher
   pwm frequencies. */

SOFTPWM_DEFINE_OBJECT_WITH_PWM_LEVELS(14, PWM_LEVELS);

/* If you want to use the SoftPWM object outside where it's defined,
   add the following line to the file. */
//SOFTPWM_DEFINE_EXTERN_OBJECT(16);
SOFTPWM_DEFINE_EXTERN_OBJECT_WITH_PWM_LEVELS(14, PWM_LEVELS);

static volatile uint8_t temp_v_array[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void set_custom_pin_pwm(uint8_t pin, uint8_t val)
{
//    array_status();
//    Serial.print(pin);
//    Serial.print(" =pin , val=");
//    Serial.print(val);
//    Serial.println("----stat");
    
    
    if (val < temp_v_array[pin])
    {
        //        Palatis::SoftPWM.set(pin,0);
        //        temp_v_array[pin] = 0;
        //        return;
        //

        for (uint8_t i = temp_v_array[pin]; i > val; i -= 1)
        {
            Palatis::SoftPWM.set(pin, i);
            delay(FADE_IN_T);

//            Serial.println(i);
        }
        
        Palatis::SoftPWM.set(pin, val);
        temp_v_array[pin] = val;
        return;
        
    }

    for (uint8_t i = temp_v_array[pin]; i < val; i += 1)
    {
        Palatis::SoftPWM.set(pin, i);
        delay(FADE_OUT_T);
//        Serial.println(i);
    }
    Palatis::SoftPWM.set(pin, val);
    temp_v_array[pin] = val;
}

float only_positive(float value)
{
    if (value >= 0)
    {
        return value;
    }
    else
    {
        return 0;
    }
}

void setup()
{
      Serial.begin(57600);

    // begin with 60hz pwm frequency
    Palatis::SoftPWM.begin(120);

    // print interrupt load for diagnostic purposes
    Palatis::SoftPWM.printInterruptLoad();
}

void custom_sine(float x)
{

    float f = x / 6.28;
    f = f - floor(f);
}

/*
 * for i in range(1,15):
    print("#define wave_array_len "+str(i+1))
    print("uint8_t wave_array[] = {" ,end='')
    for j in range(i):
        print(int(j*(100/i)),end=',')
    print("100};" ,end='\n\n')
    
    
*/

//#define wave_array_len 2
//uint8_t wave_array[] = {0,100};
//
//#define wave_array_len 3
//uint8_t wave_array[] = {0,50,100};
//
#define wave_array_len 4
uint8_t wave_array[] = {0, 15, 60, 100};
//
//#define wave_array_len 5
//uint8_t wave_array[] = {0,25,50,75,100};
//
//#define wave_array_len 6
//uint8_t wave_array[] = {0,20,40,60,80,100};
//
//#define wave_array_len 7
//uint8_t wave_array[] = {0,16,33,50,66,83,100};
//
//#define wave_array_len 8
//uint8_t wave_array[] = {0,14,28,42,57,71,85,100};
//
//#define wave_array_len 9
//uint8_t wave_array[] = {0,12,25,37,50,62,75,87,100};
//
//#define wave_array_len 10
//uint8_t wave_array[] = {0,11,22,33,44,55,66,77,88,100};
//
//#define wave_array_len 10
//uint8_t wave_array[] = {0,0,0,10,50,70,90,100,0,0};
//
//#define wave_array_len 11
//uint8_t wave_array[] = {0,10,20,30,40,50,60,70,80,90,100};
//
//#define wave_array_len 12
//uint8_t wave_array[] = {0,9,18,27,36,45,54,63,72,81,90,100};
//
//#define wave_array_len 13
//uint8_t wave_array[] = {0,8,16,25,33,41,50,58,66,75,83,91,100};
//
//#define wave_array_len 14
//uint8_t wave_array[] = {0,7,15,23,30,38,46,53,61,69,76,84,92,100};

int8_t wave_array_counter = 0;
int8_t displacer_couter = 0;

int8_t led_counter = 0;
int8_t place_counter = 0;

void padam_idea_chaser()
{
    for (uint8_t i = 0; i < 14; i++)
    {
            Serial.print(wave_array[wave_array_counter + displacer_couter]);
            Serial.print("\t");

        Palatis::SoftPWM.set(i, wave_array[wave_array_counter + displacer_couter]);
        
//        set_custom_pin_pwm(i, wave_array[wave_array_counter + displacer_couter]);
        wave_array_counter++;

        if ((wave_array_counter + displacer_couter) == (unsigned)wave_array_len)
        {
            wave_array_counter = - displacer_couter;
        }

//            delayMicroseconds(LED_GAP_TIME*100);
//        delay(LED_GAP_TIME);
    }

    displacer_couter++;

    if (displacer_couter == wave_array_len)
    {
        displacer_couter = 0;
    }

     delay(FRAME_TIME);
    
    //
    Serial.println(" \n");
//    array_status();
}

void array_status(){
   for(int j=0;j<14;j++){
      Serial.print(temp_v_array[j]);
      Serial.print("\t");
      }
      Serial.print("\n");
  }

void loop()
{
    padam_idea_chaser();

//    set_custom_pin_pwm(4,100);
//    delay(1000);
//
//    array_status();
//    
//    set_custom_pin_pwm(4,0);
//    delay(1000);
//    
//    array_status();

   
}
