#define F_CPU 160000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

int main()
{
    DDRC = 0b000000001;
    PORTC = 0b11111110;

    TCCR0A = 0b00000010;
    TCCR0B = 0b000000011;
    OCR0A = 249;
    TIMSK0 = 0b00000010; // 1ms (64[Prescaler] * (249 + 1)) / 16MHz.

    sei();

    while (1)
    {
    }
}

ISR(TIMER0_COMPA_vect)
{ // Interrução do TC0 por Comparação.

    PORTC ^= 0b000000001; // Inverte o estado do pino PC0
}