#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include "pwm_motor.h"

#define F_CPU 160000000UL
#define THREE_SECONDS_MILLS 3000
#define TOP 39999
#define OCR1A_INIT 2000
#define OCR1B_INIT 4000
#define OCR0A_INIT 249

unsigned int counter_vel_motor_max = 1000; // Em ms.

bool is_EI_stop_production_active = false;
bool is_wood_out_of_axis = false;

bool is_backing_OCRA = false;
bool is_backing_OCRB = false;

// Interrupão externa ativada por qualquer mudança de borda (PD2); usada para parar a produção através de um botão.
ISR(INT0_vect)
{
    is_EI_stop_production_active = !is_EI_stop_production_active;
    handle_green_LED();
}

// Interrupão externa ativada por qualquer mudança de borda (PB0); usada para simular o sensor de inclinação.
ISR(PCINT0_vect)
{
    is_wood_out_of_axis = true;
    handle_green_LED();
}

// Interrução do TC0 por comparação (TCNT0 e OCR0A); usada para controlar a velocidade dos servos motores s sincronização de 3 segundos.
ISR(TIMER0_COMPA_vect)
{

    handle_servo_motors();
    handle_three_seconds_timer();
}

int main()
{

    config_GPIO();
    config_EI();
    config_TIMER1_Fast_PWM();
    config_TIME0_CTC();

    sei();

    while (1)
    {
    };
}

void config_GPIO()
{
    DDRB |= 0b00000110;  // Habilita os pinos OC1A e OC1B (PB1 E PB2) como saídas.
    DDRC |= 0b00000011;  // Habilita os pinos PCO e PC1 como saídas.
    PORTC |= 0b00000010; // Habilita os pinos PC1
    DDRD &= 0b11111011;  // Seta o pino PD2 como saída.
    PORTD |= 0b00000100; // Habilita pull-up da porta PD2.
    PORTB |= 0b00000001; // Habilita pull-up da porta PB0.
}

void config_EI()
{
    EICRA |= 0b00000010;  // Interrupão externa INT0 ativada em qualquer nível.
    EIMSK |= 0b00000001;  // Ativa a interrupção externa INT0
    PCICR |= 0b00000001;  // Ativa a interrupção externa por mudança de qualquer pino da porta B.
    PCMSK0 |= 0b00000001; // Ativa a interrupção externa do pino PB0.
}

void config_TIMER1_Fast_PWM()
{
    // TIMER CT1: modo Fast PW via ICR1 (modo 14), Prescaler = 8.
    // TOP = ((F_CPU / (Prescaler * F_PWM)) - 1. F_CPU = 16MHz; Prescaler = 8; F_PWM = 50Hz (20ms); TOP = 39999.
    ICR1 = TOP;
    TCCR1A = 0b10100010; // Modo 14 não invertido (OC1A/OC1B).
    TCCR1B = 0b00011010; // Prescaler = 8.

    // Definição do Duty Cycle.
    OCR1A = OCR1A_INIT; // 1ms ~5%.
    OCR1B = OCR1B_INIT; // 2ms ~10%.
}

void config_TIME0_CTC()
{
    TCCR0A |= 0b00000010;
    TCCR0B |= 0b000000011;
    OCR0A = OCR0A_INIT;
    TIMSK0 |= 0b00000010; // 1ms (64[Prescaler] * (249 + 1)) / 16MHz.
}

void handle_green_LED()
{
    if (is_wood_out_of_axis || is_EI_stop_production_active)
    {
        PORTC &= 0b11111101;
    }
    else
    {
        PORTC |= 0b00000010;
    }
}

// Genrencia os dois servo's motores do chão de fábrica e o motor de incliação.
void handle_servo_motors()
{
    static unsigned int counter_vel_motor_init = 0;
    counter_vel_motor_init += 1;

    if (counter_vel_motor_init == counter_vel_motor_max)
    {
        counter_vel_motor_init = 0;

        if (!is_EI_stop_production_active && !is_wood_out_of_axis)
        {
            run_motor_OCR1A();
        }
    }

    if (is_wood_out_of_axis && counter_vel_motor_init == counter_vel_motor_max / 2)
    {
        run_motor_OCR1B();
    }
}

void handle_three_seconds_timer()
{
    static unsigned int counter_three_seconds_init = 0;
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

    if (OCR1B > 3000 && !is_backing_OCRB)
    {
        OCR1B -= 100;

        if (OCR1B == 3000)
            is_backing_OCRB = true;
    }
    else if (is_backing_OCRB)
    {
        OCR1B += 100;

        if (OCR1B == 4000)
        {
            is_backing_OCRB = false;
            is_wood_out_of_axis = false;
            handle_green_LED();
        }
    }
}