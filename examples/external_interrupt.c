#define F_CPU 160000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

int main()
{
    DDRD = 0X00;       // Seta todos os pinos da porta D como entrada.
    PORTD = 0XFF;      // Habilita todos os pull-ups da porta D.
    DDRC = 0b00000100; // Seta somento o PC2 como saída da porta C.

    EICRA = 0b00000010; // Interrupão externa INT0 ativada em qualquer nível.
    EIMSK = 0b00000001; // Ativa a interrupção externa INT0
    sei();

    while (1)
    {
    };
}

ISR(INT0_vect)
{
    PORTC ^= 0b000000100;
}