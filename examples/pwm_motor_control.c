#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include "pwm_motor.h"

#define F_CPU 160000000UL
#define THREE_SECONDS_MILLS 3000

unsigned int counter_vel_motor_max = 1000;
unsigned int counter_vel_motor_init = 0;

unsigned int counter_three_seconds_init = 0;

bool is_external_interrup_active = false;
bool is_backing_OCRA = false;
bool is_backing_OCRB = false;

// Interrupão externa ativada por qualquer mudança de borda (PD2)
ISR(INT0_vect)
{
    is_external_interrup_active = !is_external_interrup_active;
    PORTC ^= 0b00000010;
}

// Interrução do TC0 por comparação (TCNT0 e OCR0A).
ISR(TIMER0_COMPA_vect)
{

    handle_servo_motors();
    handle_three_seconds_timer();
}

int main()
{

    DDRB |= 0b00000110;  // Habilita os pinos OC1A e OC1B (PB1 E PB2) como saídas.
    DDRC |= 0b00000011;  // Habilita os pinos PCO e PC1 como saídas.
    PORTC |= 0b00000010; // Habilita os pinos PC1
    DDRD &= 0b11111011;  // Seta o pino PD2 como saída.
    PORTD |= 0b00000100; // Habilita pull-up da porta PD2.

    EICRA |= 0b00000010; // Interrupão externa INT0 ativada em qualquer nível.
    EIMSK |= 0b00000001; // Ativa a interrupção externa INT0

    // TIMER CT1: modo Fast PW via ICR1 (modo 14), Prescaler = 8.
    // TOP = ((F_CPU / (Prescaler * F_PWM)) - 1. F_CPU = 16MHz; Prescaler = 8; F_PWM = 50Hz (20ms); TOP = 39999.
    ICR1 = 39999;
    TCCR1A = 0b10100010; // Modo 14 não invertido (OC1A/OC1B).
    TCCR1B = 0b00011010; // Prescaler = 8.

    // Definição do Duty Cycle.
    OCR1A = 2000; // 1ms ~5%.
    OCR1B = 4000; // 2ms ~10%.

    TCCR0A |= 0b00000010;
    TCCR0B |= 0b000000011;
    OCR0A = 249;
    TIMSK0 |= 0b00000010; // 1ms (64[Prescaler] * (249 + 1)) / 16MHz.

    sei();

    while (1)
    {
    };
}

// Genrencia os dois servo's motores do chão de fábrica.
void handle_servo_motors()
{
    counter_vel_motor_init += 1;
    if (counter_vel_motor_init == counter_vel_motor_max)
    {
        counter_vel_motor_init = 0;

        if (!is_external_interrup_active)
        {
            run_motor_OCR1A();
            run_motor_OCR1B();
        }
    }
}

void handle_three_seconds_timer()
{
    counter_three_seconds_init += 1;
    if (counter_three_seconds_init == THREE_SECONDS_MILLS)
    {
        PORTC ^= 0b00000001;
        counter_three_seconds_init = 0;
    }
}

run_motor_OCR1A(void)
{

    if (OCR1A < 4000 && !is_backing_OCRA)
    {
        OCR1A += 100;

        if (OCR1A == 4000)
            is_backing_OCRA = true;
    }
    else if (is_backing_OCRA)
    {
        OCR1A -= 100;
        if (OCR1A == 2000)
            is_backing_OCRA = false;
    }
}

run_motor_OCR1B(void)
{

    if (OCR1B > 2000 && !is_backing_OCRB)
    {
        OCR1B -= 100;

        if (OCR1B == 2000)
            is_backing_OCRB = true;
    }
    else if (is_backing_OCRB)
    {
        OCR1B += 100;

        if (OCR1B == 4000)
            is_backing_OCRB = false;
    }
}