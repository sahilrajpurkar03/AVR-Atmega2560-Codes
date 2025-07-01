/*
 * PS2.c
 *
 * Created: 19-06-2018 17:56:38
 * Author : Sahil
 */

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <math.h>
#include <avr/interrupt.h>

#define BAUD 9600                                   // Serial baud rate
#define BAUDRATE  ((F_CPU/(BAUD*16UL))-1)

uint8_t RX[16] = {100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100};
int RX_range = 200, RX_raw = 255, RX_ad = 255, RX_count = 0;
uint8_t TX[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
int flag[16] = {0};
uint8_t TX_raw = 200, TX_ad = 201, TX_flag = 0;

#define indicator_1 PORTB

// PS2 button indices
#define PS_L1       0
#define PS_R1       1
#define PS_L2       2
#define PS_R2       3
#define PS_L3       4
#define PS_R3       5
#define PS_TRIANGLE 6
#define PS_SQUARE   7
#define PS_CROSS    8
#define PS_CIRCLE   9
#define PS_UP       10
#define PS_LEFT     11
#define PS_DOWN     12
#define PS_RIGHT    13
#define PS_START    14
#define PS_SELECT   15

int xj1 = 0, yj1 = 0, xj2 = 0, yj2 = 0;           // Joystick analog values
int butt[16] = {0};                                // Button states
int js_error = 20;
int pwm_range = 255;

// Function prototypes
void receive(void);
void serialstart_3(void);
uint16_t ADC_read(uint8_t ch);
void ADC_initiate(void);
void port_init(void);

long map_value(long in_value, long in_min, long in_max, long out_min, long out_max);
long limit_var(long in_var, long l_limit, long h_limit);

int main(void)
{
    port_init();
    sei();
    ADC_initiate();
    serialstart_3();

    while (1)
    {
        receive();
    }
}

void port_init(void)
{
    DDRB = 0xFF;    // Set PORTB as output (indicator)
}

void receive(void)
{
    // Map joystick values from received data to -255 to +255 PWM range
    yj1 = map_value(RX[0], 0, RX_range, -pwm_range, pwm_range);
    xj1 = map_value(RX[1], 0, RX_range, pwm_range, -pwm_range);
    yj2 = map_value(RX[2], 0, RX_range, -pwm_range, pwm_range);
    xj2 = map_value(RX[3], 0, RX_range, pwm_range, -pwm_range);

    // Toggle indicator_1 on button presses and reset button flag
    if (butt[PS_START])    { indicator_1 ^= 0xFF; butt[PS_START] = 0; }
    if (butt[PS_SELECT])   { indicator_1 ^= 0xFF; butt[PS_SELECT] = 0; }
    if (butt[PS_UP])       { indicator_1 ^= 0xFF; butt[PS_UP] = 0; }
    if (butt[PS_DOWN])     { indicator_1 ^= 0xFF; butt[PS_DOWN] = 0; }
    if (butt[PS_LEFT])     { indicator_1 ^= 0xFF; butt[PS_LEFT] = 0; }
    if (butt[PS_RIGHT])    { indicator_1 ^= 0xFF; butt[PS_RIGHT] = 0; }
    if (butt[PS_SQUARE])   { indicator_1 ^= 0xFF; butt[PS_SQUARE] = 0; }
    if (butt[PS_CIRCLE])   { indicator_1 ^= 0xFF; butt[PS_CIRCLE] = 0; }
    if (butt[PS_TRIANGLE]) { indicator_1 ^= 0xFF; butt[PS_TRIANGLE] = 0; }
    if (butt[PS_CROSS])    { indicator_1 ^= 0xFF; butt[PS_CROSS] = 0; }
    if (butt[PS_L1])       { indicator_1 ^= 0xFF; butt[PS_L1] = 0; }
    if (butt[PS_R1])       { indicator_1 ^= 0xFF; butt[PS_R1] = 0; }
    if (butt[PS_L2])       { indicator_1 ^= 0xFF; butt[PS_L2] = 0; }
    if (butt[PS_R2])       { indicator_1 ^= 0xFF; butt[PS_R2] = 0; }
    if (butt[PS_L3])       { indicator_1 ^= 0xFF; butt[PS_L3] = 0; }
    if (butt[PS_R3])       { indicator_1 ^= 0xFF; butt[PS_R3] = 0; }
}

void ADC_initiate(void)
{
    // AVcc reference, right adjusted result
    ADMUX = (1 << REFS0);
    // ADC enable, no auto trigger, no interrupt, prescaler = 128
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRB = 0x00;
}

uint16_t ADC_read(uint8_t ch)
{
    // Clear previous channel selection
    ADMUX &= 0xE0;     // Clear MUX bits
    ADCSRB &= ~(1 << MUX5);

    ch &= 0x0F;        // Channel must be 0-15

    if (ch <= 7)
    {
        ADMUX |= ch;
    }
    else
    {
        ch -= 8;
        ADMUX |= ch;
        ADCSRB |= (1 << MUX5);
    }

    ADCSRA |= (1 << ADSC);           // Start conversion
    while (!(ADCSRA & (1 << ADIF))); // Wait for conversion to finish
    ADCSRA |= (1 << ADIF);           // Clear interrupt flag

    return ADC;
}

void serialstart_3(void)
{
    UBRR3H = (BAUDRATE >> 8);
    UBRR3L = BAUDRATE;
    UCSR3B = (1 << RXEN3) | (1 << TXEN3) | (1 << RXCIE3); // Enable RX, TX, RX interrupt
    UCSR3C = (1 << UCSZ31) | (1 << UCSZ30);               // 8-bit data
}

ISR(USART3_RX_vect)
{
    RX_count = 1;
    RX_raw = UDR3;

    // Analog channel addresses: 201-216
    // Button addresses: 231-246
    if (RX_raw > 200 && RX_raw < 255)
    {
        RX_ad = RX_raw;
        if (RX_raw > 230 && RX_raw < 247)
        {
            uint8_t btn_index = RX_raw - 231;
            if (btn_index < 16)
                butt[btn_index] = 1;
        }
    }
    else if (RX_raw < 201)
    {
        uint8_t index = RX_ad - 201;
        if (index < 16)
            RX[index] = RX_raw;
    }
}

long map_value(long in_value, long in_min, long in_max, long out_min, long out_max)
{
    return (in_value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

long limit_var(long in_var, long l_limit, long h_limit)
{
    if (in_var > h_limit)
        return h_limit;
    else if (in_var < l_limit)
        return l_limit;
    else
        return in_var;
}
