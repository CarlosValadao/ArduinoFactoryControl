#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define F_CPU 160000000UL
#define THREE_SECONDS_MILLS 3000
#define TOP 39999
#define OCR1A_INIT 2000
#define OCR2A_INIT 32
#define OCR1B_INIT 4000
#define OCR0A_INIT 249
#define set_bit(Y, bit_x) (Y |= (1 << bit_x))
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x))
#define tst_bit(Y, bit_x) (Y & (1 << bit_x))

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

uint8_t quantity_wood_cutted = 0;

unsigned int vel_motor_max_vertical_cut = 100;   // Em ms.
unsigned int vel_motor_max_horizontal_cut = 100; // Em ms.
unsigned int vel_motor_max_inclination = 100;    // Em ms.

bool is_EI_stop_production_active = false;
bool is_wood_out_of_axis = false;
bool is_presence_sensor_active = false;

bool is_backing_OCRA = false;
bool is_backing_OCR2A = false;
bool is_backing_OCRB = false;
bool is_complete_vertical_cut = false;
bool is_complete_horizontal_cut = false;

void config_GPIO()
{
    DDRB |= 0b00001110;  // Habilita os pinos OC1A, OC1B e OC2A (PB1, PB2 e PB3) como saídas.
    DDRC |= 0b00111011;  // Habilita os pinos PCO, PC1, PC3, PC4, PC5 como saídas.
    PORTC |= 0b00000010; // Seta o pino PC1 em HIGH (Produção ativa).
    DDRD &= 0b00111011;  // Seta os pinos PD2, PD6 e PD7 como entradas.
    DDRD |= 0b00001000;  // Seta o pino PD3 como saída.
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

void config_TIMER2_Fast_PWM()
{
    // TIMER CT2: modo Fast PW via TOP(0xFF) (modo 3), Prescaler = 1024.
    // F_PWM = ((F_CPU / (Prescaler * TOP)) = 61Hz. F_CPU = 16MHz; Prescaler = 1024;  TOP = 256.
    TCCR2A = 0b10100011; // Modo 3 não invertido (OC2A/OC2B).
    TCCR2B = 0b00000111; // Prescaler = 1024.

    // Definição do Duty Cycle.
    OCR2A = OCR2A_INIT; // 1,95ms ~12%.
    OCR2B = 0;          // 1,95ms ~12%.
}

void config_TIMER0_CTC()
{
    TCCR0A |= 0b00000010;
    TCCR0B |= 0b000000011;
    OCR0A = OCR0A_INIT;
    TIMSK0 |= 0b00000010; // 1ms (64[Prescaler] * (249 + 1)) / 16MHz.
}

void handle_green_LED()
{
    if (is_wood_out_of_axis || is_EI_stop_production_active || is_presence_sensor_active)
    {
        PORTC &= 0b11111101;
    }
    else
    {
        PORTC |= 0b00000010;
    }
}

int is_production_not_stopped()
{
    return !is_EI_stop_production_active && !is_wood_out_of_axis && !is_presence_sensor_active;
}

void increment_quantity_wood_cutted()
{

    quantity_wood_cutted += 1;

    if (quantity_wood_cutted == 99)
        quantity_wood_cutted = 0;
}

void handle_quantity_wood_cutted()
{

    if (is_complete_vertical_cut && is_complete_horizontal_cut)
    {
        increment_quantity_wood_cutted();

        is_complete_vertical_cut = false;
        is_complete_horizontal_cut = false;
    }
}

void run_motor_OCR1A_vertical_cut()
{

    if (!is_complete_vertical_cut)
    {
        if (OCR1A < 4000 && !is_backing_OCRA)
        {
            OCR1A += 100;
            is_complete_vertical_cut = false;

            if (OCR1A == 4000)
                is_backing_OCRA = true;
        }
        else if (is_backing_OCRA)
        {
            OCR1A -= 100;
            if (OCR1A == 2000)
            {
                is_complete_vertical_cut = true;
                is_backing_OCRA = false;
            }
        }
    }
}

void run_motor_OCR1B_horizontal_cut()
{
    if (!is_complete_horizontal_cut)
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
            {
                is_backing_OCRB = false;
                is_complete_horizontal_cut = true;
            }
        }
    }
}

void run_motor_OCR2A_inclination()
{

    if (OCR2A > 14 && !is_backing_OCR2A)
    {
        OCR2A -= 2;

        if (OCR2A == 14)
            is_backing_OCR2A = true;
    }
    else if (is_backing_OCR2A)
    {
        OCR2A += 2;

        if (OCR2A == 32)
        {
            is_backing_OCR2A = false;
            is_wood_out_of_axis = false;
        }
    }
}

void handle_servo_motor_vertical_cut()
{
    static unsigned int counter_vel_motor_init = 0;
    counter_vel_motor_init += 1;

    if (counter_vel_motor_init == vel_motor_max_vertical_cut)
    {
        counter_vel_motor_init = 0;

        if (is_production_not_stopped())
        {
            run_motor_OCR1A_vertical_cut();
        }
    }
}

void handle_servo_motor_horizontal_cut()
{
    static unsigned int counter_vel_motor_init = 0;
    counter_vel_motor_init += 1;

    if (counter_vel_motor_init == vel_motor_max_horizontal_cut)
    {
        counter_vel_motor_init = 0;

        if (is_production_not_stopped())
        {
            run_motor_OCR1B_horizontal_cut();
        }
    }
}

void handle_servo_motor_inclination()
{
    static unsigned int counter_vel_motor_init = 0;
    counter_vel_motor_init += 1;

    if (counter_vel_motor_init == vel_motor_max_inclination)
    {
        counter_vel_motor_init = 0;
        if (is_wood_out_of_axis)
            run_motor_OCR2A_inclination();
    }
}

// Genrencia os dois servo's motores do chão de fábrica e o motor de incliação.
void handle_servo_motors()
{
    handle_servo_motor_vertical_cut();
    handle_servo_motor_horizontal_cut();
    handle_servo_motor_inclination();
}

void handle_three_seconds_timer()
{
    static unsigned int counter_three_seconds_init = 0;
    counter_three_seconds_init += 1;

    if (counter_three_seconds_init == THREE_SECONDS_MILLS)
    {
        // PORTC ^= 0b00000001;
        counter_three_seconds_init = 0;
    }
}

void handle_clock_display_7_seg()
{
    PORTC ^= 0b000000001;
}

int get_value_display(uint8_t tot_value, uint8_t counter_display)
{
    switch (tot_value)
    {
    case 0:
        return NUMBER_ZERO_DISPLAY[counter_display];

    case 1:
        return NUMBER_ONE_DISPLAY[counter_display];

    case 2:
        return NUMBER_TWO_DISPLAY[counter_display];

    case 3:
        return NUMBER_THREE_DISPLAY[counter_display];

    case 4:
        return NUMBER_FOUR_DISPLAY[counter_display];

    case 5:
        return NUMBER_FIVE_DISPLAY[counter_display];

    case 6:
        return NUMBER_SIX_DISPLAY[counter_display];

    case 7:
        return NUMBER_SEVEN_DISPLAY[counter_display];

    case 8:
        return NUMBER_EIGHT_DISPLAY[counter_display];

    case 9:
        return NUMBER_NINE_DISPLAY[counter_display];

    default:
        return 0;
    }
}

void handle_displays_7seg()
{
    static uint8_t counter_display = 0;
    static uint8_t value_display_unit = 0;
    static uint8_t value_display_ten = 0;

    value_display_unit = get_value_display(quantity_wood_cutted % 10, counter_display);
    value_display_ten = get_value_display(quantity_wood_cutted / 10, counter_display);

    if (!tst_bit(PORTC, PC0))
    {

        if (value_display_unit)
            set_bit(PORTC, PC3);
        else
            clr_bit(PORTC, PC3);

        if (value_display_ten)
            set_bit(PORTC, PC4);
        else
            clr_bit(PORTC, PC4);

        counter_display += 1;

        if (counter_display == 9)
        {
            set_bit(PORTC, PC5);
            counter_display = 0;
        }
        else
            clr_bit(PORTC, PC5);
    }

    handle_clock_display_7_seg();
}

void handle_sensor_oil_tank_level()
{
    if (tst_bit(PIND, PORT7))
    {
        OCR2B += 1;

        if (OCR2B == 256)
            OCR2B = 0;
    }
    else
        OCR2B = 0;
}

void handle_sensor_presence()
{
    if (tst_bit(PIND, PORT6))
        is_presence_sensor_active = true;
    else
        is_presence_sensor_active = false;
}

// Interrupão externa ativada por qualquer mudança de borda (PD2); usada para parar a produção através de um botão.
ISR(INT0_vect)
{
    is_EI_stop_production_active = !is_EI_stop_production_active;
}

// Interrupão externa ativada por qualquer mudança de borda (PB0); usada para simular o sensor de inclinação.
ISR(PCINT0_vect)
{
    is_wood_out_of_axis = true;
}

// Interrução do TC0 por comparação (TCNT0 e OCR0A); usada para controlar a velocidade dos servos motores e sincronização de 3 segundos.
ISR(TIMER0_COMPA_vect)
{

    handle_servo_motors();
    handle_three_seconds_timer();
    handle_displays_7seg();
    handle_sensor_oil_tank_level();
    handle_sensor_presence();
    handle_green_LED();
}

int main(void)
{

    config_GPIO();
    config_EI();
    config_TIMER1_Fast_PWM();
    config_TIMER2_Fast_PWM();
    config_TIMER0_CTC();

    sei();

    while (1)
    {
        handle_quantity_wood_cutted();
    };
}