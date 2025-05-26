/*
 * atmega328p_usart.h
 *
 * Created: 27/04/2025 03:22:27
 * Author: chval
 *
 * @brief Header file for controlling USART (Universal Synchronous and Asynchronous serial Receiver and Transmitter)
 * on the ATmega328P microcontroller. This file defines various macros and functions to initialize and use USART
 * in different modes for serial communication.
 */

#ifndef ATMEGA328P_USART_H
#define ATMEGA328P_USART_H

// Include necessary AVR libraries for USART and bit manipulation
#include <avr/io.h>        ///< For AVR I/O registers manipulation
#include <avr/interrupt.h> ///< For handling interrupts
#include <stdint.h>        ///< For standard integer types
#include <stdbool.h>       ///< For boolean types (true/false)
#include "bit_utils.h"     ///< For bit manipulation utilities

#define F_CPU 16000000UL

// USART modes for operation
#define USART_ASYNCRONOUS 0                            ///< Asynchronous mode for USART
#define USART_SYNCRONOUS (1 << UMSEL00)                ///< Synchronous mode for USART
#define USART_MASTER_SPI (1 << UMSEL01 | 1 << UMSEL00) ///< Master SPI mode for USART

// Baud rate calculation macro based on the formula UBRR = (F_CPU / (16 * BAUD)) - 1
#define UBRR_VALUE(BAUD) ((F_CPU / (16UL * (BAUD))) - 1)

// Predefined baud rates using the UBRR_VALUE macro
#define USART_BAUD_9600 UBRR_VALUE(9600)     ///< Baud rate for 9600 bps
#define USART_BAUD_115200 UBRR_VALUE(115200) ///< Baud rate for 115200 bps
#define USART_BAUD_19200 UBRR_VALUE(19200)   ///< Baud rate for 19200 bps
#define USART_BAUD_38400 UBRR_VALUE(38400)   ///< Baud rate for 38400 bps
#define USART_BAUD_57600 UBRR_VALUE(57600)   ///< Baud rate for 57600 bps
#define USART_BAUD_128000 UBRR_VALUE(128000) ///< Baud rate for 128000 bps

// USART parity modes
#define USART_PARITY_NONE 0                         ///< No parity bit
#define USART_PARITY_ODD (1 << UPM00)               ///< Odd parity
#define USART_PARITY_EVEN (1 << UPM01 | 1 << UPM00) ///< Even parity

// USART stop bit configuration
#define USART_1_STOP_BIT 0            ///< 1 stop bit
#define USART_2_STOP_BIT (1 << USBS0) ///< 2 stop bits

// USART character size configuration
#define USART_CHAR_SIZE_5_BITS 0                           ///< 5-bit character size
#define USART_CHAR_SIZE_6_BITS (1 << UCSZ00)               ///< 6-bit character size
#define USART_CHAR_SIZE_7_BITS (1 << UCSZ01)               ///< 7-bit character size
#define USART_CHAR_SIZE_8_BITS (1 << UCSZ01 | 1 << UCSZ00) ///< 8-bit character size

// USART interrupt enable macros - CORRIGIDO
#define USART_IRQ_TX_COMPLETE (1 << TXCIE0) ///< Interrupt for transmission complete
#define USART_IRQ_RX_COMPLETE (1 << RXCIE0) ///< Interrupt for reception complete
#define USART_IRQ_EMPTY (1 << UDRIE0)       ///< Interrupt for UART Data Register Empty

/**
 * @brief Initializes USART with the specified parameters.
 *
 * This function configures USART for communication based on the provided parameters, including mode,
 * baud rate, data bits, stop bits, and parity.
 *
 * @param mode Operation mode for USART (Asynchronous, Synchronous, or SPI).
 * @param baud_rate Baud rate for communication.
 * @param data_bits Number of data bits (5 to 8 bits).
 * @param stop_bits Number of stop bits (1 or 2).
 * @param parity Parity configuration (None, Odd, or Even).
 */
void usart_init(uint8_t mode, uint32_t baud_rate, uint8_t data_bits, uint8_t stop_bits, uint8_t parity);

/**
 * @brief Deinitializes the USART, turning it off.
 *
 * This function disables USART and clears any configuration settings.
 */
void usart_deinit(void);

/**
 * @brief Sends a single character through USART.
 *
 * This function sends a single byte (character) of data through the USART for transmission.
 *
 * @param data The byte to be transmitted.
 */
void usart_sendc(uint8_t data);

/**
 * @brief Receives a single character from USART.
 *
 * This function reads a single byte (character) of data received through the USART.
 *
 * @return The received byte.
 */
uint8_t usart_recvc(void);

/**
 * @brief Sends a string of characters through USART.
 *
 * This function sends a null-terminated string through the USART, one byte at a time.
 *
 * @param str The string to be transmitted.
 */
void usart_send_string(const char *restrict str);

/**
 * @brief Receives a string of characters through USART.
 *
 * This function receives a string from USART until a null terminator is detected.
 *
 * @return The received string.
 */
char *usart_recv_string(void);

/**
 * @brief Enables USART interrupts for specific events.
 *
 * This function enables the interrupt for specific USART events such as transmission complete or reception complete.
 *
 * @param event The interrupt event to enable (TX complete, RX complete, or Empty).
 */
void usart_enable_interrupt(uint8_t event);

/**
 * @brief Disables USART interrupts for specific events.
 *
 * This function disables the interrupt for specific USART events.
 *
 * @param event The interrupt event to disable.
 */
void usart_disable_interrupt(uint8_t event);

/**
 * @brief Disables all USART interrupts.
 *
 * This function disables all interrupts related to USART communication.
 */
void usart_disable_all_interrupt(void);

/**
 * @brief Checks if USART is ready to transmit data.
 *
 * This function checks if the USART data register is ready for transmission, meaning the previous byte has been sent.
 *
 * @return true if USART is ready to transmit; false otherwise.
 */
bool usart_transmit_ready(void);

/**
 * @brief Checks if data is available for reading from USART.
 *
 * This function checks if there is data available in the USART data register to be read.
 *
 * @return true if data is available; false otherwise.
 */
bool usart_available(void);

#endif // END ATMEGA328P_USART_H