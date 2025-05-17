/*
 * atmega328p_gpio.h
 *
 * Created: 04/05/2025 13:39:21
 * Author: chval
 *
 * @brief This header file contains all GPIO (General Purpose Input/Output) definitions
 * and function prototypes for the ATmega328P microcontroller.
 * @details Provides a complete interface for GPIO configuration including:
 *          - Pin direction control
 *          - Digital I/O operations
 *          - Pull-up resistor configuration
 *          - Pin change interrupt handling
 *          - Power management
 */
#ifndef ATMEGA328P_GPIO_H
#define ATMEGA328P_GPIO_H

// Includes necessary libraries for bitwise operations and standard types
#include <avr/io.h>         ///< For AVR I/O registers manipulation
#include "bit_utils.h"      ///< For bit manipulation utilities
#include <stdint.h>         ///< For standard integer types (uint8_t, uint32_t, etc.)
#include <stdbool.h>        ///< For boolean types (true/false)

// Macro to cast values to uint8_t type
#define CAST_TO_UINT8(x) ((uint8_t) (x))

// GPIO Pin mappings
// Define GPIO pins on the ATmega328P for various functions (e.g., digital I/O, PWM, ADC, etc.)
#define GPIO_PB0 CAST_TO_UINT8(0xB0) ///< GPIO Pin PB0
#define CLK0   GPIO_PB0               ///< Clock function on pin PB0
#define GPIO_PCINT0 GPIO_PB0          ///< Pin change interrupt (PCINT0) on pin PB0
#define ICP1   GPIO_PB0               ///< Input capture function (ICP1) on pin PB0
#define D8     GPIO_PB0               ///< Digital I/O Pin D8 on pin PB0

#define GPIO_PB1 CAST_TO_UINT8(0xB1) ///< GPIO Pin PB1
#define OC1A   GPIO_PB1               ///< Output Compare 1A (OC1A) function on pin PB1
#define GPIO_PCINT1 GPIO_PB1          ///< Pin change interrupt (PCINT1) on pin PB1
#define D9     GPIO_PB1               ///< Digital I/O Pin D9 on pin PB1

#define GPIO_PB2 CAST_TO_UINT8(0xB2) ///< GPIO Pin PB2
#define OC1B   GPIO_PB2               ///< Output Compare 1B (OC1B) function on pin PB2
#define GPIO_PCINT2 GPIO_PB2          ///< Pin change interrupt (PCINT2) on pin PB2
#define SS     GPIO_PB2               ///< Slave Select (SS) function on pin PB2 (SPI)
#define D10    GPIO_PB2               ///< Digital I/O Pin D10 on pin PB2

#define GPIO_PB3 CAST_TO_UINT8(0xB3) ///< GPIO Pin PB3
#define OC2    GPIO_PB3               ///< Output Compare 2 (OC2) function on pin PB3
#define GPIO_PCINT3 GPIO_PB3          ///< Pin change interrupt (PCINT3) on pin PB3
#define MOSI   GPIO_PB3               ///< Master Out Slave In (MOSI) function on pin PB3 (SPI)
#define D11    GPIO_PB3               ///< Digital I/O Pin D11 on pin PB3

#define GPIO_PB4 CAST_TO_UINT8(0xB4) ///< GPIO Pin PB4
#define GPIO_PCINT4 GPIO_PB4          ///< Pin change interrupt (PCINT4) on pin PB4
#define MISO   GPIO_PB4               ///< Master In Slave Out (MISO) function on pin PB4 (SPI)
#define D12    GPIO_PB4               ///< Digital I/O Pin D12 on pin PB4

#define GPIO_PB5 CAST_TO_UINT8(0xB5) ///< GPIO Pin PB5
#define GPIO_PCINT5 GPIO_PB5          ///< Pin change interrupt (PCINT5) on pin PB5
#define SCK    GPIO_PB5               ///< Serial Clock (SCK) function on pin PB5 (SPI)
#define D13    GPIO_PB5               ///< Digital I/O Pin D13 on pin PB5

// Define analog input pins
#define GPIO_PC0 CAST_TO_UINT8(0xC0) ///< GPIO Pin PC0
// #define ADC0    GPIO_PC0              ///< ADC0 Analog Input on pin PC0
#define A0      GPIO_PC0              ///< Analog Input A0 on pin PC0

#define GPIO_PC1 CAST_TO_UINT8(0xC1) ///< GPIO Pin PC1
// #define ADC1    GPIO_PC1              ///< ADC1 Analog Input on pin PC1
#define A1      GPIO_PC1              ///< Analog Input A1 on pin PC1

#define GPIO_PC2 CAST_TO_UINT8(0xC2) ///< GPIO Pin PC2
#define GPIO_PCINT10 GPIO_PC2         ///< Pin change interrupt (PCINT10) on pin PC2
// #define ADC2    GPIO_PC2              ///< ADC2 Analog Input on pin PC2
#define A2      GPIO_PC2              ///< Analog Input A2 on pin PC2

#define GPIO_PC3 CAST_TO_UINT8(0xC3) ///< GPIO Pin PC3
#define GPIO_PCINT11 GPIO_PC3         ///< Pin change interrupt (PCINT11) on pin PC3
// #define ADC3    GPIO_PC3              ///< ADC3 Analog Input on pin PC3
#define A3      GPIO_PC3              ///< Analog Input A3 on pin PC3

#define GPIO_PC4 CAST_TO_UINT8(0xC4) ///< GPIO Pin PC4
#define GPIO_PCINT12 GPIO_PC4         ///< Pin change interrupt (PCINT12) on pin PC4
// #define ADC4    GPIO_PC4              ///< ADC4 Analog Input on pin PC4
#define SDA     GPIO_PC4              ///< SDA I2C Data Pin on pin PC4
#define A4      GPIO_PC4              ///< Analog Input A4 on pin PC4

#define GPIO_PC5 CAST_TO_UINT8(0xC5) ///< GPIO Pin PC5
#define GPIO_PCINT13 GPIO_PC5         ///< Pin change interrupt (PCINT13) on pin PC5
// #define ADC5    GPIO_PC5              ///< ADC5 Analog Input on pin PC5
#define SCL     GPIO_PC5              ///< SCL I2C Clock Pin on pin PC5
#define A5      GPIO_PC5              ///< Analog Input A5 on pin PC5

#define GPIO_PC6 CAST_TO_UINT8(0xC6) ///< GPIO Pin PC6
#define RST     GPIO_PC6              ///< Reset Pin (RST) on pin PC6
#define GPIO_PCINT14 GPIO_PC6         ///< Pin change interrupt (PCINT14) on pin PC6

#define GPIO_PD0 CAST_TO_UINT8(0xD0) ///< GPIO Pin PD0
#define RXD     GPIO_PD0              ///< UART Receive (RXD) on pin PD0
#define GPIO_PCINT16 GPIO_PD0         ///< Pin change interrupt (PCINT16) on pin PD0
#define RX0     GPIO_PD0              ///< RX0 Serial Pin on pin PD0

#define GPIO_PD1 CAST_TO_UINT8(0xD1) ///< GPIO Pin PD1
#define TXD     GPIO_PD1              ///< UART Transmit (TXD) on pin PD1
#define GPIO_PCINT17 GPIO_PD1         ///< Pin change interrupt (PCINT17) on pin PD1
#define TX1     GPIO_PD1              ///< TX1 Serial Pin on pin PD1

#define GPIO_PD2 CAST_TO_UINT8(0xD2) ///< GPIO Pin PD2
#define GPIO_PCINT18 GPIO_PD2         ///< Pin change interrupt (PCINT18) on pin PD2
#define D2      GPIO_PD2              ///< Digital I/O Pin D2 on pin PD2

#define GPIO_PD3 CAST_TO_UINT8(0xD3) ///< GPIO Pin PD3
#define GPIO_PCINT19 GPIO_PD3         ///< Pin change interrupt (PCINT19) on pin PD3
#define OC2B    GPIO_PD3              ///< Output Compare 2B (OC2B) on pin PD3
#define D3      GPIO_PD3              ///< Digital I/O Pin D3 on pin PD3

#define GPIO_PD4 CAST_TO_UINT8(0xD4) ///< GPIO Pin PD4
#define T0      GPIO_PD4              ///< Timer 0 (T0) on pin PD4
#define GPIO_PCINT20 GPIO_PD4         ///< Pin change interrupt (PCINT20) on pin PD4
#define XCK     GPIO_PD4              ///< External Clock (XCK) on pin PD4
#define D4      GPIO_PD4              ///< Digital I/O Pin D4 on pin PD4

#define GPIO_PD5 CAST_TO_UINT8(0xD5) ///< GPIO Pin PD5
#define T1      GPIO_PD5              ///< Timer 1 (T1) on pin PD5
#define GPIO_PCINT21 GPIO_PD5         ///< Pin change interrupt (PCINT21) on pin PD5
#define OC0B    GPIO_PD5              ///< Output Compare 0B (OC0B) on pin PD5
#define D5      GPIO_PD5              ///< Digital I/O Pin D5 on pin PD5

#define GPIO_PD6 CAST_TO_UINT8(0xD6) ///< GPIO Pin PD6
#define AIN0    GPIO_PD6              ///< Analog Input AIN0 on pin PD6
#define GPIO_PCINT22 GPIO_PD6         ///< Pin change interrupt (PCINT22) on pin PD6
#define OC0A    GPIO_PD6              ///< Output Compare 0A (OC0A) on pin PD6
#define D6      GPIO_PD6              ///< Digital I/O Pin D6 on pin PD6

#define GPIO_PD7 CAST_TO_UINT8(0xD7) ///< GPIO Pin PD7
#define AIN1    GPIO_PD7              ///< Analog Input AIN1 on pin PD7
#define GPIO_PCINT23 GPIO_PD7         ///< Pin change interrupt (PCINT23) on pin PD7
#define D7      GPIO_PD7              ///< Digital I/O Pin D7 on pin PD7

// Definitions for pin directions (output/input)
#define GPIO_DIR_OUTPUT 0  ///< Pin direction set to output
#define GPIO_DIR_INPUT  1  ///< Pin direction set to input

// Definitions for pin states (low/high)
#define GPIO_LOW  0  ///< Low state (0V) for GPIO pin
#define GPIO_HIGH 1  ///< High state (VCC) for GPIO pin

// Definitions for enabling/disabling pull-up resistors
#define GPIO_PULLUP_DISABLE 0  ///< Disable pull-up resistor
#define GPIO_PULLUP_ENABLE  1  ///< Enable pull-up resistor

// Definitions for interrupt triggers (level or edge-based)
#define GPIO_IRQ_LEVEL_LOW  0x00  ///< Interrupt trigger on low level
#define GPIO_IRQ_ANY_CHANGE 0x01  ///< Interrupt trigger on any change
#define GPIO_IRQ_EDGE_FALL  0x02  ///< Interrupt trigger on falling edge
#define GPIO_IRQ_EDGE_RISE  0x03  ///< Interrupt trigger on rising edge

// Port identifiers for GPIO
#define GPIO_PORTB 0  ///< Port B identifier
#define GPIO_PORTC 1  ///< Port C identifier
#define GPIO_PORTD 2  ///< Port D identifier


/* Port Operations ************************************************************/

/**
 * @brief Writes a value to an entire GPIO port
 * @param port The target port (GPIO_PORTB, GPIO_PORTC, GPIO_PORTD)
 * @param value The 8-bit value to write to the port
 * @note This operation affects all 8 pins of the specified port simultaneously
 */
void gpio_port_write(uint8_t port, uint8_t value);

/**
 * @brief Reads the current state of an entire GPIO port
 * @param port The target port (GPIO_PORTB, GPIO_PORTC, GPIO_PORTD)
 * @return The 8-bit value representing the state of all port pins
 */
uint8_t gpio_port_read(uint8_t port);

/**
 * @brief Configures the direction of all pins in a port
 * @param port The target port (GPIO_PORTB, GPIO_PORTC, GPIO_PORTD)
 * @param directions Bitmask where 1=input, 0=output
 */
void gpio_port_set_direction(uint8_t port, uint8_t directions);

/**
 * @brief Gets the current direction configuration of a port
 * @param port The target port (GPIO_PORTB, GPIO_PORTC, GPIO_PORTD)
 * @return Bitmask representing current directions (1=input, 0=output)
 */
uint8_t gpio_port_get_direction(uint8_t port);

/* Pin Operations *************************************************************/

/**
 * @brief Sets the direction of an individual GPIO pin
 * @param pin The pin identifier (e.g., GPIO_PB0)
 * @param direction GPIO_DIR_INPUT or GPIO_DIR_OUTPUT
 */
void gpio_pin_set_direction(uint8_t pin, uint8_t direction);

/**
 * @brief Gets the direction of a specific pin
 * @param port The port containing the pin (GPIO_PORTB, GPIO_PORTC, GPIO_PORTD)
 * @param pin The pin number (0-7)
 * @return GPIO_DIR_INPUT or GPIO_DIR_OUTPUT
 */
uint8_t gpio_pin_get_direction(uint8_t port, uint8_t pin);

/**
 * @brief Writes a logic level to a GPIO pin
 * @param pin The pin identifier (e.g., GPIO_PB0)
 * @param value GPIO_HIGH or GPIO_LOW
 */
void gpio_pin_write(uint8_t pin, uint8_t value);

/**
 * @brief Reads the current state of a GPIO pin
 * @param pin The pin identifier (e.g., GPIO_PB0)
 * @return GPIO_HIGH or GPIO_LOW
 */
uint8_t gpio_pin_read(uint8_t pin);

/**
 * @brief Toggles the current state of a GPIO pin
 * @param pin The pin identifier (e.g., GPIO_PB0)
 */
void gpio_pin_toggle(uint8_t pin);

/**
 * @brief Configures the pull-up resistor of a GPIO pin
 * @param pin The pin identifier (e.g., GPIO_PB0)
 * @param enable_pullup true to enable pull-up, false to disable
 * @note Only effective when pin is configured as input
 */
void gpio_pin_pullup(uint8_t pin, bool enable_pullup);

/* Interrupt Configuration *****************************************************/

/**
 * @brief Sets the callback function for pin change interrupt
 * @param port The port containing the pin (GPIO_PORTB, GPIO_PORTC, GPIO_PORTD)
 * @param pin The pin number (0-7)
 * @warning The callback function must be defined elsewhere in the application
 */
void gpio_set_irq_callback(uint8_t port, uint8_t pin);

/**
 * @brief Disables interrupt for a specific pin
 * @param port The port containing the pin (GPIO_PORTB, GPIO_PORTC, GPIO_PORTD)
 * @param pin The pin number (0-7)
 */
void gpio_interrupt_disable(uint8_t port, uint8_t pin);

/**
 * @brief Sets the interrupt mask for a port
 * @param port The target port (GPIO_PORTB, GPIO_PORTC, GPIO_PORTD)
 * @param mask Bitmask indicating which pins should generate interrupts
 */
void gpio_interrupt_mask_set(uint8_t port, uint8_t mask);

/**
 * @brief Gets the interrupt flags for a port
 * @param port The target port (GPIO_PORTB, GPIO_PORTC, GPIO_PORTD)
 * @return Bitmask indicating which pins triggered interrupts
 */
uint8_t gpio_interrupt_flag_get(uint8_t port);

/**
 * @brief Clears interrupt flags for a port
 * @param port The target port (GPIO_PORTB, GPIO_PORTC, GPIO_PORTD)
 */
void gpio_interrupt_flag_clear(uint8_t port);

/* Utility Functions **********************************************************/

/**
 * @brief Converts port and pin to PCINT number
 * @param port The port containing the pin (GPIO_PORTB, GPIO_PORTC, GPIO_PORTD)
 * @param pin The pin number (0-7)
 * @return The corresponding PCINT number (0-23)
 */
uint8_t gpio_to_pcint(uint8_t port, uint8_t pin);

/**
 * @brief Controls power reduction for GPIO ports
 * @param port The target port (GPIO_PORTB, GPIO_PORTC, GPIO_PORTD)
 * @param enable true to enable power reduction, false to disable
 */
void gpio_power_reduction(uint8_t port, uint8_t enable);

/* Analog Pin Handling ********************************************************/

/**
 * @brief Disables analog function on PORTC pins
 * @param pin The pin identifier (must be PORTC pin)
 */
void gpio_analog_disable(uint8_t pin);

/* Initialization/Deinitialization ********************************************/

/**
 * @brief Deinitializes a GPIO pin
 * @param gpio The pin identifier (e.g., GPIO_PB0)
 */
void gpio_deinit(uint8_t gpio);

/**
 * @brief Initializes multiple GPIO pins using a bitmask
 * @param gpio_mask Bitmask representing pins to initialize
 */
void gpio_init_mask(uint8_t gpio_mask);

/**
 * @brief Gets the complete configuration of a GPIO pin
 * @param gpio The pin identifier (e.g., GPIO_PB0)
 */
void gpio_get(uint8_t gpio);

/**
 * @brief Enables/disables pull-up resistors for all pins
 * @param enable_all true to enable all pull-ups, false to disable
 */
void gpio_pullup_enable_all(bool enable_all);

/* External Interrupts *******************************************************/

/**
 * @brief Enables INT0 interrupt with specified trigger
 * @param event_mask The trigger condition (GPIO_IRQ_* constants)
 */
void gpio_enable_int0_interrupt(uint8_t event_mask);

/**
 * @brief Enables INT1 interrupt with specified trigger
 * @param event_mask The trigger condition (GPIO_IRQ_* constants)
 */
void gpio_enable_int1_interrupt(uint8_t event_mask);

/**
 * @brief Disables INT0 interrupt
 */
void gpio_disable_int0_interrupt(void);

/**
 * @brief Disables INT1 interrupt
 */
void gpio_disable_int1_interrupt(void);

/* Pin Change Interrupts *****************************************************/

/**
 * @brief Enables pin change interrupt for specific pin
 * @param port The port containing the pin (GPIO_PORTB, GPIO_PORTC, GPIO_PORTD)
 * @param pin The pin number (0-7)
 */
void gpio_enable_pcint_for_pin(uint8_t port, uint8_t pin);

/**
 * @brief Disables pin change interrupt for specific pin
 * @param port The port containing the pin (GPIO_PORTB, GPIO_PORTC, GPIO_PORTD)
 * @param pin The pin number (0-7)
 */
void gpio_disable_pcint_for_pin(uint8_t port, uint8_t pin);

/**
 * @brief Enables all pin change interrupts for PORTB
 */
void gpio_enable_pcint_portb_interrupt(void);

/**
 * @brief Enables all pin change interrupts for PORTC
 */
void gpio_enable_pcint_portc_interrupt(void);

/**
 * @brief Enables all pin change interrupts for PORTD
 */
void gpio_enable_pcint_portd_interrupt(void);

/**
 * @brief Disables all pin change interrupts for PORTB
 */
void gpio_disable_pcint_portb_interrupt(void);

/**
 * @brief Disables all pin change interrupts for PORTC
 */
void gpio_disable_pcint_portc_interrupt(void);

/**
 * @brief Disables all pin change interrupts for PORTD
 */
void gpio_disable_pcint_portd_interrupt(void);

#endif /* ATMEGA328P_GPIO_H */