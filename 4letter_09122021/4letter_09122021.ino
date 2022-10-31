#include "aSoftPWM.h"

// #define DEBUG_AMOR 1

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

// #define wave_array_len 4
// uint8_t wave_array[] = {8, 15, 60, 100};
// uint8_t wave_array[] = {10, 26, 76, 100};
// uint8_t wave_array[] = {10, 26, 100, 50};

// uint8_t wave_array[] = {6, 20, 76, 100};
// uint8_t wave_array[] = {6, 50, 100, 50};

//#define wave_array_len 5
// uint8_t wave_array[] = {5,10, 100, 60, 25};

//#define wave_array_len 6
// uint8_t wave_array[] = {0,5,10, 15, 60, 100};

#define wave_array_len 10
////uint8_t wave_array[] = {8,12,22,34,44,56,66,76,88,100};  // liner
uint8_t wave_array[] = {8, 22, 44, 66, 88, 100, 76, 56, 34, 10}; // inverted V

// uint8_t wave_array[] = {22,66,100,66,22,22,66,100,66,22}; // inverted V
// uint8_t wave_array[] = {12,56,100,56,12,12,56,100,56,12}; // inverted V

// uint8_t wave_array[] = {12,56,100,56,12,56,100,56,12,6}; // inverted V

//#define wave_array_len 20
// uint8_t wave_array[] = {0,0,0,0,0,0,0,0,0,0,8,12,22,34,44,56,66,76,88,100};
//
//#define wave_array_len 10
// uint8_t wave_array[] = {0,0,0,10,50,70,90,100,0,0};
//
//#define wave_array_len 11
// uint8_t wave_array[] = {0,10,20,30,40,50,60,70,80,90,100};
//
//#define wave_array_len 12
// uint8_t wave_array[] = {0,9,18,27,36,45,54,63,72,81,90,100};
//
//#define wave_array_len 13
// uint8_t wave_array[] = {0,8,16,25,33,41,50,58,66,75,83,91,100};
//
//#define wave_array_len 14
// uint8_t wave_array[] = {0,10,18,24,30,38,46,54,62,70,76,84,92,100};

// uint8_t wave_array[] = {2,10,30,46,62,70,84,100,92,76,62,46,30,16}; // frame rate 1600

SOFTPWM_DEFINE_CHANNEL(0, DDRD, PORTD, PORTD0);  // Arduino pin 0
SOFTPWM_DEFINE_CHANNEL(1, DDRD, PORTD, PORTD1);  // Arduino pin 1
SOFTPWM_DEFINE_CHANNEL(2, DDRD, PORTD, PORTD2);  // Arduino pin 2
SOFTPWM_DEFINE_CHANNEL(3, DDRD, PORTD, PORTD3);  // Arduino pin 3
SOFTPWM_DEFINE_CHANNEL(4, DDRD, PORTD, PORTD4);  // Arduino pin 4
SOFTPWM_DEFINE_CHANNEL(5, DDRD, PORTD, PORTD5);  // Arduino pin 5
SOFTPWM_DEFINE_CHANNEL(6, DDRD, PORTD, PORTD6);  // Arduino pin 6
SOFTPWM_DEFINE_CHANNEL(7, DDRD, PORTD, PORTD7);  // Arduino pin 7
SOFTPWM_DEFINE_CHANNEL(8, DDRB, PORTB, PORTB0);  // Arduino pin 8
SOFTPWM_DEFINE_CHANNEL(9, DDRB, PORTB, PORTB1);  // Arduino pin 9
SOFTPWM_DEFINE_CHANNEL(10, DDRB, PORTB, PORTB2); // Arduino pin 10
SOFTPWM_DEFINE_CHANNEL(11, DDRB, PORTB, PORTB3); // Arduino pin 11
SOFTPWM_DEFINE_CHANNEL(12, DDRB, PORTB, PORTB4); // Arduino pin 12
SOFTPWM_DEFINE_CHANNEL(13, DDRB, PORTB, PORTB5); // Arduino pin 13

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
    for (uint8_t i = 0; i < 14; i++)
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

    for (uint8_t i = 0; i < 14; i++)
    {
        Palatis::SoftPWM.set(i, temp_v_array_current[i]);
    }

    delayMicroseconds(FRAME_RATE);
}

void make_wave()
{
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
}

void data_inject()
{

    delayMicroseconds(FRAME_RATE_DATA);

#ifdef DEBUG_AMOR
    Serial.print("data_inject()  FRAME_RATE_DATA\t");
    Serial.println(FRAME_RATE_DATA);
#endif

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

void all_on(uint8_t brightness = 100)
{

    // #ifdef DEBUG_AMOR
    //     Serial.print("all_on() \t");
    //     Serial.println(brightness);
    // #endif

    for (uint8_t i = 0; i < 14; i++)
    {
        Palatis::SoftPWM.set(i, brightness);
        temp_v_array_desired[i] = brightness;
        temp_v_array_current[i] = brightness;
    }
}

void all_off()
{
#ifdef DEBUG_AMOR
    Serial.print("all_off() \t");
#endif

    for (uint8_t i = 0; i < 14; i++)
    {
        Palatis::SoftPWM.set(i, 0);
        temp_v_array_desired[i] = 0;
        temp_v_array_current[i] = 0;
    }
}

void all_on_with_given_delay(uint8_t dealy_time_frame)
{
#ifdef DEBUG_AMOR
    Serial.print("all_on_with_given_delay() \t");
#endif

    for (int j = 0; j < PWM_LEVELS; j++)
    {
        all_on(j);
        delay(dealy_time_frame);
    }
}

void all_off_with_given_delay(uint8_t dealy_time_frame)
{
#ifdef DEBUG_AMOR
    Serial.print("all_off_with_given_delay() \t");

#endif

    for (int j = (PWM_LEVELS - 1); j >= 0; j--)
    {
        all_on(j);
        delay(dealy_time_frame);
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

bool isFirstFlikerFlag = 1;

// unsigned long full_on_millis = 0 ;

bool speed_rate = 0;

volatile uint8_t hotel = 0;

int delay_t_all_off = 500;

int dealy_t_all_on = 4;

uint8_t x_loop_1234_count = 0;

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

#ifdef DEBUG_AMOR
    Serial.print("x_loop_1234_count \t");
    Serial.print(x_loop_1234_count);
    Serial.print("\t \t hotel \t");
    Serial.println(hotel);
#endif

    if ((x_loop_1234_count < 2) && (hotel == 5))
    {
        all_off_with_given_delay(2);
        x_loop_1234_count = x_loop_1234_count + 1;
        hotel = 0;
    }

    if (hotel == 0)
    {
        temp_v_array_desired[11] = {255};
    }
    else if (hotel == 1)
    {
        temp_v_array_desired[10] = {255};
    }
    else if (hotel == 2)
    {
        temp_v_array_desired[9] = {255};
    }
    else if (hotel == 3)
    {
        temp_v_array_desired[8] = {255};
    }
    else if (hotel == 4)
    {
        temp_v_array_desired[7] = {255};
    }

    else if (hotel == 5)
    {
        x_loop_1234_count = 0;

        for (uint8_t i = 0; i < 2; i++)
        {
            all_off_with_given_delay(4);
            all_on_with_given_delay(4);
            if (i == 2)
            {
                all_off_with_given_delay(4);
            }
        }
    }

    else if (hotel == 6)
    {
        FRAME_RATE = 2500;
        FRAME_RATE_DATA = 1100;

        blinking_millis = millis();
        while (!(millis() > blinking_millis + 10000))
        {
            make_wave();
            data_inject();
        }
    }
    else if (hotel == 7)
    {
        all_off_with_given_delay(2);
    }

    // else if (hotel == 8)
    // {
    // }
    // else if (hotel == 9)
    // {
    // }

    if (millis() > blinking_millis + 0)
    {
        hotel++;
#ifdef DEBUG_AMOR
        Serial.print("hotel \t");
        Serial.println(hotel);
#endif

        if (hotel == 8)
        {
            hotel = 0;
            x_loop_1234_count = 0;

            if (speed_rate)
            {
                speed_rate = !speed_rate;
                // pattern 1
                FRAME_RATE = 3286 * 0.5;
                FRAME_RATE_DATA = 1100 * 0.5;
            }
            else
            {
                speed_rate = !speed_rate;
                // pattern 2
                FRAME_RATE = 3286 * 0.2;
                FRAME_RATE_DATA = 1100 * 0.2;

            }
        }
    }
}
