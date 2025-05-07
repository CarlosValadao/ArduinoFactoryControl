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

uint8_t counter_display = 0;

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
    counter_display += 1;

    if (counter_display == 10)
    {
        counter_display = 0;
    }
}

ISR(TIMER0_COMPA_vect)
{ // Interrução do TC0 por Comparação.

    static unsigned int counter = 0;
    static unsigned int counter_test = 0;
    static unsigned int counter_test_display = 0;
    static unsigned int value = 0;

    switch (counter_display)
    {
    case 0:
        value = NUMBER_ZERO_DISPLAY[counter_test_display];
        break;

    case 1:
        value = NUMBER_ONE_DISPLAY[counter_test_display];
        break;

    case 2:
        value = NUMBER_TWO_DISPLAY[counter_test_display];
        break;

    case 3:
        value = NUMBER_THREE_DISPLAY[counter_test_display];
        break;

    case 4:
        value = NUMBER_FOUR_DISPLAY[counter_test_display];
        break;

    case 5:
        value = NUMBER_FIVE_DISPLAY[counter_test_display];
        break;

    case 6:
        value = NUMBER_SIX_DISPLAY[counter_test_display];
        break;

    case 7:
        value = NUMBER_SEVEN_DISPLAY[counter_test_display];
        break;

    case 8:
        value = NUMBER_EIGHT_DISPLAY[counter_test_display];
        break;

    case 9:
        value = NUMBER_NINE_DISPLAY[counter_test_display];
        break;

    default:
        break;
    }

    if (!tst_bit(PORTC, PC0))
    {
        counter_test += 1;

        if (value)
            set_bit(PORTC, PC4);
        else
            clr_bit(PORTC, PC4);

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