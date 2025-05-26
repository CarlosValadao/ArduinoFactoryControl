#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "DHT.h"
#include "atmega328p_usart.h"

#define F_CPU 16000000UL
#define THREE_SECONDS_MILLS 3000
#define TOP 39999
#define OCR1A_INIT 2000
#define OCR2A_INIT 32
#define OCR1B_INIT 4000
#define OCR0A_INIT 249
#define set_bit(Y, bit_x) (Y |= (1 << bit_x))
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x))
#define tst_bit(Y, bit_x) (Y & (1 << bit_x))
#define CHAR_TO_NUM(value) ((uint8_t)(value - 48))

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
uint8_t temperature;
uint8_t humidity;

unsigned int vel_motor_max_vertical_cut = 100;   // Em ms.
unsigned int vel_motor_max_horizontal_cut = 100; // Em ms.
unsigned int vel_motor_max_inclination = 100;    // Em ms.

unsigned int counter_vel_motor_init_vertical = 0;
unsigned int counter_vel_motor_init_horizontal = 0;

bool is_EI_stop_production_active = false;
bool is_wood_out_of_axis = false;
bool is_presence_sensor_active = false;
bool is_critical_temperature = false;

bool is_backing_OCRA = false;
bool is_backing_OCR2A = false;
bool is_backing_OCRB = false;
bool is_complete_vertical_cut = false;
bool is_complete_horizontal_cut = false;
bool is_motor_velocity_changed = false;

volatile uint8_t buffer_tx[8] = {0};
volatile uint8_t buffer_tx_idx = 0;

volatile uint8_t buffer_rx[2] = {0};
volatile uint8_t buffer_rx_idx = 0;

volatile uint16_t motors_velocity[10] = {
    0,
    100,
    200,
    300,
    400,
    500,
    600,
    700,
    800,
    900};

void config_GPIO()
{
    DDRB |= 0b00001110;  // Habilita os pinos OC1A, OC1B e OC2A (PB1, PB2 e PB3) como saídas.
    DDRC |= 0b00111011;  // Habilita os pinos PCO, PC1, PC3, PC4, PC5 como saídas.
    PORTC |= 0b00000010; // Seta o pino PC1 em HIGH (Produção ativa).
    DDRD &= 0b00111011;  // Seta os pinos PD2, PD6 e PD7 como entradas.
    DDRD |= 0b00011000;  // Seta os pinos PD3 e PD4 como saída.
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
    TCCR2A = 0b10000011; // Modo 3 não invertido (OC2A).
    TCCR2B = 0b00000111; // Prescaler = 1024.

    // Definição do Duty Cycle.
    OCR2A = OCR2A_INIT; // 1,95ms ~12%.
}

void config_TIMER0_CTC()
{
    TCCR0A |= 0b00000010;
    TCCR0B |= 0b000000011;
    OCR0A = OCR0A_INIT;
    TIMSK0 |= 0b00000010; // 1ms (64[Prescaler] * (249 + 1)) / 16MHz.
}

int is_production_not_stopped()
{
    return !is_EI_stop_production_active && !is_wood_out_of_axis && !is_presence_sensor_active && !is_critical_temperature;
}

void handle_green_LED()
{
    if (is_production_not_stopped())
    {
        PORTC |= 0b00000010;
    }
    else
    {
        PORTC &= 0b11111101;
    }
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
    counter_vel_motor_init_vertical += 1;

    if (counter_vel_motor_init_vertical == vel_motor_max_vertical_cut)
    {
        counter_vel_motor_init_vertical = 0;

        if (is_production_not_stopped())
        {
            run_motor_OCR1A_vertical_cut();
        }
    }
}

void handle_servo_motor_horizontal_cut()
{
    counter_vel_motor_init_horizontal += 1;

    if (counter_vel_motor_init_horizontal == vel_motor_max_horizontal_cut)
    {
        counter_vel_motor_init_horizontal = 0;

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

void show_factory_status()
{

    buffer_tx[0] = temperature;
    buffer_tx[1] = is_wood_out_of_axis ? 1 : 0;
    buffer_tx[2] = is_presence_sensor_active ? 1 : 0;
    buffer_tx[3] = tst_bit(PIND, PORT7) ? 1 : 0;
    buffer_tx[4] = is_production_not_stopped();
    buffer_tx[5] = quantity_wood_cutted;
    buffer_tx[6] = (uint8_t)vel_motor_max_horizontal_cut % 10;
    buffer_tx[7] = (uint8_t)vel_motor_max_vertical_cut % 10;

    usart_sendc(buffer_tx[0]);

    buffer_tx_idx++;
}

void handle_temperature()
{
    if (temperature < 10 || temperature > 40)
    {
        usart_send_string("Critical Temperature!");
        is_critical_temperature = true;
        set_bit(PORTD, PD3);
    }
    else
    {
        clr_bit(PORTD, PD3);
        is_critical_temperature = false;
    }
}
void handle_get_temperature()
{
    DHT_Read(&temperature, &humidity);

    // Check status
    switch (DHT_GetStatus())
    {
    case (DHT_Ok):
        handle_temperature();
        break;

    case (DHT_Error_Checksum):
        usart_send_string("DHT_Error_Checksum!");
        break;

    case (DHT_Error_Timeout):
        usart_send_string("DHT_Error_Timeout!");
        break;
    }
}

void handle_three_seconds_timer()
{
    static unsigned int counter_three_seconds_init = 0;
    counter_three_seconds_init += 1;

    if (counter_three_seconds_init == THREE_SECONDS_MILLS)
    {

        counter_three_seconds_init = 0;

        handle_get_temperature();

        buffer_tx_idx = 0;
        show_factory_status();
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
        set_bit(PORTD, PD4);
    else
        clr_bit(PORTD, PD4);
}

void handle_sensor_presence()
{
    if (!tst_bit(PIND, PORT6))
        is_presence_sensor_active = true;
    else
        is_presence_sensor_active = false;
}

// Interrupão externa ativada por qualquer mudança de borda (PD2); usada para parar a produção através de um botão.
ISR(INT0_vect)
{
    is_EI_stop_production_active = !is_EI_stop_production_active;

    if (is_EI_stop_production_active)
    {
        usart_send_string("Stop Performs Successfully!");
    }
}

// Interrupão externa ativada por qualquer mudança de borda (PB0); usada para simular o sensor de inclinação.
ISR(PCINT0_vect)
{
    is_wood_out_of_axis = true;
    usart_send_string("Wood out of axis!");
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

ISR(USART_RX_vect)
{

    buffer_rx[buffer_rx_idx] = UDR0;
    buffer_rx_idx++;

    if (buffer_rx_idx == 2)
    {
        is_motor_velocity_changed = true;
        buffer_rx_idx = 0;
        return;
    }
}

ISR(USART_TX_vect)
{

    if (buffer_tx_idx == 8)
        return;
    UDR0 = buffer_tx[buffer_tx_idx++];
}

int main(void)
{

    config_GPIO();
    config_EI();
    config_TIMER1_Fast_PWM();
    config_TIMER2_Fast_PWM();
    config_TIMER0_CTC();
    DHT_Setup();

    usart_init(
        USART_ASYNCRONOUS,
        1000000UL,
        USART_CHAR_SIZE_8_BITS,
        USART_1_STOP_BIT,
        USART_PARITY_NONE);

    usart_enable_interrupt(USART_IRQ_TX_COMPLETE);
    usart_enable_interrupt(USART_IRQ_RX_COMPLETE);

    sei();

    while (1)
    {
        handle_quantity_wood_cutted();

        if (is_motor_velocity_changed)
        {
            is_motor_velocity_changed = false;
            vel_motor_max_horizontal_cut = motors_velocity[(buffer_rx[0] - 48)];
            vel_motor_max_vertical_cut = motors_velocity[(buffer_rx[1] - 48)];

            counter_vel_motor_init_horizontal = 0;
            counter_vel_motor_init_vertical = 0;
        }
    };
}