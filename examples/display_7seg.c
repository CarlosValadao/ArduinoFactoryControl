#define F_CPU 160000000UL
#define set_bit(Y, bit_x) (Y |= (1 << bit_x))
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x))
#define tst_bit(Y, bit_x) (Y & (1 << bit_x))

#include <avr/io.h>
#include <avr/interrupt.h>

const uint8_t NUMBER_ZERO_DISPLAY[8] = {0, 0, 1, 1, 1, 1, 1, 1};
const uint8_t NUMBER_ONE_DISPLAY[8] = {0, 0, 0, 0, 0, 1, 1, 0};
const uint8_t NUMBER_TWO_DISPLAY[8] = {0, 1, 0, 1, 1, 0, 1, 1};
const uint8_t NUMBER_THREE_DISPLAY[8] = {0, 1, 0, 0, 1, 1, 1, 1};
const uint8_t NUMBER_FOUR_DISPLAY[8] = {0, 1, 1, 0, 0, 1, 1, 0};
const uint8_t NUMBER_FIVE_DISPLAY[8] = {0, 1, 1, 0, 1, 1, 0, 1};
const uint8_t NUMBER_SIX_DISPLAY[8] = {0, 1, 1, 1, 1, 1, 0, 1};
const uint8_t NUMBER_SEVEN_DISPLAY[8] = {0, 0, 0, 0, 0, 1, 1, 1};
const uint8_t NUMBER_EIGHT_DISPLAY[8] = {0, 1, 1, 1, 1, 1, 1, 1};
const uint8_t NUMBER_NINE_DISPLAY[8] = {0, 1, 1, 0, 1, 1, 1, 1};

uint8_t counter_display_1 = 0;
uint8_t counter_display_2 = 0;


int main()
{
    DDRC = 0XFF;
    DDRD &= ~(1 << 0);
    PORTD |= ~(1 << 0);

    EICRA = 0b00000010;
    EIMSK = 0b00000001;

    TCCR0A = 0b00000010;
    TCCR0B = 0b000000011;
    OCR0A = 249;
    TIMSK0 = 0b00000010; // 1ms (64[Prescaler] * (249 + 1)) / 16MHz.

    sei();

    while (1)
    {
    };
}

ISR(INT0_vect)
{
    static unsigned int counter = 0;
    counter += 1;

    if (counter == 99)
    {
        counter = 0;
    }

    counter_display_1 = counter % 10;
    counter_display_2 = counter / 10;
}

ISR(TIMER0_COMPA_vect)
{ // Interrução do TC0 por Comparação.

    static unsigned int counter = 0;
    static unsigned int counter_test = 0;
    static unsigned int counter_test_display = 0;
    static unsigned int value_display_1 = 0;
    static unsigned int value_display_2 = 0;

    switch (counter_display_1)
    {
    case 0:
        value_display_1 = NUMBER_ZERO_DISPLAY[counter_test_display];
        break;

    case 1:
        value_display_1 = NUMBER_ONE_DISPLAY[counter_test_display];
        break;

    case 2:
        value_display_1 = NUMBER_TWO_DISPLAY[counter_test_display];
        break;

    case 3:
        value_display_1 = NUMBER_THREE_DISPLAY[counter_test_display];
        break;

    case 4:
        value_display_1 = NUMBER_FOUR_DISPLAY[counter_test_display];
        break;

    case 5:
        value_display_1 = NUMBER_FIVE_DISPLAY[counter_test_display];
        break;

    case 6:
        value_display_1 = NUMBER_SIX_DISPLAY[counter_test_display];
        break;

    case 7:
        value_display_1 = NUMBER_SEVEN_DISPLAY[counter_test_display];
        break;

    case 8:
        value_display_1 = NUMBER_EIGHT_DISPLAY[counter_test_display];
        break;

    case 9:
        value_display_1 = NUMBER_NINE_DISPLAY[counter_test_display];
        break;

    default:
        break;
    }

    switch (counter_display_2)
    {
    case 0:
        value_display_2 = NUMBER_ZERO_DISPLAY[counter_test_display];
        break;

    case 1:
        value_display_2 = NUMBER_ONE_DISPLAY[counter_test_display];
        break;

    case 2:
        value_display_2 = NUMBER_TWO_DISPLAY[counter_test_display];
        break;

    case 3:
        value_display_2 = NUMBER_THREE_DISPLAY[counter_test_display];
        break;

    case 4:
        value_display_2 = NUMBER_FOUR_DISPLAY[counter_test_display];
        break;

    case 5:
        value_display_2 = NUMBER_FIVE_DISPLAY[counter_test_display];
        break;

    case 6:
        value_display_2 = NUMBER_SIX_DISPLAY[counter_test_display];
        break;

    case 7:
        value_display_2 = NUMBER_SEVEN_DISPLAY[counter_test_display];
        break;

    case 8:
        value_display_2 = NUMBER_EIGHT_DISPLAY[counter_test_display];
        break;

    case 9:
        value_display_2 = NUMBER_NINE_DISPLAY[counter_test_display];
        break;

    default:
        break;
    }

    if (!tst_bit(PORTC, PC0))
    {
        counter_test += 1;

        if (value_display_1)
        {
            set_bit(PORTC, PC3);
        }
            
        else {
            clr_bit(PORTC, PC3);
        }

        if (value_display_2)
        {
            set_bit(PORTC, PC4);
        }
            
        else {
            clr_bit(PORTC, PC4);
        }

        counter_test_display += 1;

        if (counter_test_display == 9)
        {
            set_bit(PORTC, PC5);
            counter_test_display = 0;
        }
        else
        {
            clr_bit(PORTC, PC5);
        }
    }

    PORTC ^= 0b000000001;
}