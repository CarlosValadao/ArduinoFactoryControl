#define F_CPU 160000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t LUT[10] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B};

volatile uint8_t num_test = 0;

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
    static uint8_t num = 0;
    num_test += 1;

    if (num == 10)
        num = 0;
}

ISR(TIMER0_COMPA_vect)
{ // Interrução do TC0 por Comparação.

    static unsigned int counter = 0;
    static unsigned int counter_test = 0;

    counter += 1;

    if (counter == 1000)
    {
        counter_test += 1;

        PORTC |= 0b00010000; // 00000001;
        counter = 0;
        PORTC ^= 0b000000001;
        if (counter_test == 16)
        {
            PORTC |= 0b000100000;
            // PORTC &= 0b111011111;
        }
    }
}