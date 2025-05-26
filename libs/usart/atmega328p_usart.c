/*
 * atmega328p_usart.c
 *
 * Created: 27/04/2025 03:22:08
 * Author: chval
 *
 * @brief This file contains the implementation of USART (Universal Synchronous and Asynchronous serial Receiver and Transmitter) functions
 * for the ATmega328P microcontroller. It includes functions to initialize, deinitialize, send and receive data, and manage interrupts.
 */

#include "atmega328p_usart.h" ///< For USART function prototypes and register manipulations
#include "bit_utils.h"        ///< For bit manipulation utilities
#include <string.h>           ///< For string handling functions like strlen()

/**
 * @brief Computes the UBRR0 value for the desired baud rate.
 *
 * This function calculates the UBRR0 (USART Baud Rate Register) value needed to set the baud rate
 * for USART communication based on the provided baud rate and the system clock frequency (F_CPU).
 *
 * @param baud_rate The desired baud rate for USART communication.
 * @return The calculated UBRR0 value.
 */
static uint16_t compute_ubrr0_value(uint32_t baud_rate)
{
    // The formula for computing UBRR0 is: UBRR0 = (F_CPU / (16 * baud_rate)) - 1
    return (uint16_t)(F_CPU / 16 / baud_rate - 1);
}

/**
 * @brief Sets the baud rate for USART communication.
 *
 * This function sets the UBRR0 value in the appropriate high and low registers (UBRR0H and UBRR0L)
 * based on the desired baud rate.
 *
 * @param baud_rate The desired baud rate for USART communication.
 */
static void set_baud_rate(uint32_t baud_rate)
{
    // Compute the UBRR0 value for the given baud rate
    uint16_t ubrr0_value = compute_ubrr0_value(baud_rate);
    // Set the high and low bytes of the UBRR0 register
    UBRR0H = (uint8_t)(ubrr0_value >> 8); // Set the high byte
    UBRR0L = (uint8_t)ubrr0_value;        // Set the low byte
}

/**
 * @brief Initializes USART with specified settings.
 *
 * This function initializes the USART peripheral with the specified mode, baud rate, number of data bits,
 * number of stop bits, and parity settings. It also enables the transmitter and receiver.
 *
 * @param mode The mode of operation (Asynchronous, Synchronous, etc.).
 * @param baud_rate The desired baud rate for communication.
 * @param data_bits The number of data bits (5-8).
 * @param stop_bits The number of stop bits (1 or 2).
 * @param parity The parity configuration (None, Odd, Even).
 */
void usart_init(uint8_t mode, uint32_t baud_rate, uint8_t data_bits, uint8_t stop_bits, uint8_t parity)
{
    // Enable receiver and transmitter
    set_bit(&UCSR0B, RXEN0); // Enable receiver
    set_bit(&UCSR0B, TXEN0); // Enable transmitter
    // Set USART configuration (mode, data bits, stop bits, and parity)
    set_bitmask(&UCSR0C, (mode | data_bits | stop_bits | parity));
    // Set the baud rate
    set_baud_rate(baud_rate);
}

/**
 * @brief Deinitializes the USART.
 *
 * This function disables the USART and clears all configuration registers, effectively turning off USART communication.
 */
void usart_deinit(void)
{
    // Disable receiver, transmitter, and clear configuration registers
    UCSR0B = 0x0; // Disable receiver and transmitter
    UCSR0C = 0x0; // Clear configuration register
    UBRR0L = 0x0; // Clear low byte of baud rate register
    UBRR0H = 0x0; // Clear high byte of baud rate register
}

/**
 * @brief Sends a single character over USART.
 *
 * This function waits for the USART transmit buffer to be ready and sends a single byte of data.
 *
 * @param data The byte to send via USART.
 */
void usart_sendc(uint8_t data)
{
    // Wait until the USART transmit buffer is ready for sending data
    // while(!usart_transmit_ready());
    // Load the data into the USART data register for transmission
    UDR0 = data;
}

/**
 * @brief Receives a single character from USART.
 *
 * This function waits for data to be received in the USART receive buffer and returns the received byte.
 *
 * @return The received byte from USART.
 */
uint8_t usart_recvc(void)
{
    // Wait until data is available in the USART receive buffer
    while (!usart_available())
        ;
    // Read and return the received byte
    return UDR0;
}

/**
 * @brief Sends a string of characters over USART.
 *
 * This function sends a null-terminated string of characters one by one over USART.
 *
 * @param str The string to send via USART.
 */
void usart_send_string(const char *restrict str)
{
    // Send each character in the string
    while (*str != '\0')
    {
        usart_sendc(*str);
        while (!is_bit_set(UCSR0A, UDRE0))
            ;
        str++;
    }
}

/**
 * @brief Receives a string of characters from USART.
 *
 * This function receives a string of characters from USART and returns the received string.
 * This function is not implemented yet.
 *
 * @return A pointer to the received string.
 */
char *usart_recv_string(void)
{
    // Not implemented
    return NULL;
}

/**
 * @brief Enables USART interrupts for specific events.
 *
 * This function enables USART interrupts for the specified event(s), such as transmission complete or reception complete.
 *
 * @param event The interrupt event to enable (TX complete, RX complete, etc.).
 */
void usart_enable_interrupt(uint8_t event)
{
    // Enable the specified USART interrupt event
    set_bitmask(&UCSR0B, event);
}

/**
 * @brief Disables USART interrupts for specific events.
 *
 * This function disables USART interrupts for the specified event(s).
 *
 * @param event The interrupt event to disable.
 */
void usart_disable_interrupt(uint8_t event)
{
    // Disable the specified USART interrupt event
    clr_bitmask(&UCSR0B, event);
}

/**
 * @brief Disables all USART interrupts.
 *
 * This function disables all USART interrupt events by clearing the interrupt-related bits in UCSR0B.
 */
void usart_disable_all_interrupt(void)
{
    // Disable all USART-related interrupts (RX, TX, Empty)
    clr_bitmask(&UCSR0B, 0b11100000);
}

/**
 * @brief Checks if data is available to read from USART.
 *
 * This function checks if there is data available in the USART receive buffer.
 *
 * @return true if data is available, false otherwise.
 */
bool usart_available(void)
{
    // Check if the RXC0 (Receive Complete) bit is set in the UCSR0A register
    return is_bit_set(UCSR0A, RXC0);
}

/**
 * @brief Checks if USART is ready to transmit data.
 *
 * This function checks if the USART data register is ready to accept new data for transmission.
 *
 * @return true if USART is ready to transmit, false otherwise.
 */
bool usart_transmit_ready(void)
{
    // Check if the UDRE0 (USART Data Register Empty) bit is set in the UCSR0A register
    return is_bit_set(UCSR0A, UDRE0);
}
