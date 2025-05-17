/*
 * atmega328p_gpio.c
 *
 * Created: 04/05/2025 13:39:34
 * Author: chval
 *
 * @brief This file contains the implementation of GPIO functions
 * for the ATmega328P microcontroller.
 * @details Includes functions for pin configuration, digital I/O,
 * pull-up control, and interrupt handling.
 */

#include "atmega328p_gpio.h"
#include <stdlib.h>
#include <limits.h>

/* Macros for Input Validation ************************************************/

/**
 * @brief Checks if a value is within a specified range.
 *
 * This macro checks if a given value is between the specified minimum and maximum values.
 *
 * @param value Value to check
 * @param min Minimum allowed value
 * @param max Maximum allowed value
 * @return true if value is within range, false otherwise
 */
#define IS_IN_RANGE(value, min, max) (((value) >= (min)) && ((value) <= (max)))

/**
 * @brief Checks if the direction is set to OUTPUT.
 *
 * This macro checks whether the pin's direction is configured as output.
 *
 * @param direction Direction to check
 * @return true if direction is OUTPUT, false otherwise
 */
#define IS_DIRECTION_OUTPUT(direction) ((direction) == GPIO_DIR_OUTPUT)

/**
 * @brief Checks if the direction is set to INPUT.
 *
 * This macro checks whether the pin's direction is configured as input.
 *
 * @param direction Direction to check
 * @return true if direction is INPUT, false otherwise
 */
#define IS_DIRECTION_INPUT(direction) ((direction) == GPIO_DIR_INPUT)

/**
 * @brief Checks if the pin state is HIGH.
 *
 * This macro checks if the state of the given pin is HIGH (1).
 *
 * @param pin Pin state to check
 * @return true if pin state is HIGH, false otherwise
 */
#define IS_PIN_TO_HIGH(pin) ((pin) == GPIO_HIGH)

/**
 * @brief Checks if the pin state is LOW.
 *
 * This macro checks if the state of the given pin is LOW (0).
 *
 * @param pin Pin state to check
 * @return true if pin state is LOW, false otherwise
 */
#define IS_PIN_TO_LOW(pin) ((pin) == GPIO_LOW)

/* Port Address Offsets *******************************************************/

/**
 * @brief Offset for PORTB pins (PB0-PB5 = 176-181).
 *
 * This macro defines the starting address for the PORTB pin registers.
 */
#define GPIO_PB_OFFSET 176

/**
 * @brief Offset for PORTC pins (PC0-PC6 = 192-198).
 *
 * This macro defines the starting address for the PORTC pin registers.
 */
#define GPIO_PC_OFFSET 192

/**
 * @brief Offset for PORTD pins (PD0-PD7 = 208-215).
 *
 * This macro defines the starting address for the PORTD pin registers.
 */
#define GPIO_PD_OFFSET 208

/* Private Helper Functions ***************************************************/

/**
 * @brief Determines the port registers and bit position for a given pin.
 *
 * This function calculates the port and bit position for a specified pin identifier.
 * It returns the bit position (0-7) and provides the register addresses for the DDR
 * (Data Direction Register) and PORT for the corresponding port.
 *
 * @param pin The pin identifier (e.g., GPIO_PB0)
 * @param ddr_port Pointer to store DDR register address
 * @param portx_port Pointer to store PORT register address
 * @return Bit position (0-7) or UCHAR_MAX if invalid pin
 * @note This is an internal helper function not exposed in the header.
 */
static uint8_t gpio_get_port_bit(uint8_t pin, volatile uint8_t **ddr_port, volatile uint8_t **portx_port)
{
    uint8_t port_bit = 0;

    // Check if the pin is within the range for PORTB
    if (IS_IN_RANGE(pin, GPIO_PB0, GPIO_PB5))
    {
        port_bit = (uint8_t)(pin - GPIO_PB_OFFSET); // Calculate bit position
        if (ddr_port != NULL)
            *ddr_port = &DDRB; // Assign DDRB address
        if (portx_port != NULL)
            *portx_port = &PORTB; // Assign PORTB address
    }
    // Check if the pin is within the range for PORTC
    else if (IS_IN_RANGE(pin, GPIO_PC0, GPIO_PC6))
    {
        port_bit = (uint8_t)(pin - GPIO_PC_OFFSET); // Calculate bit position
        if (ddr_port != NULL)
            *ddr_port = &DDRC; // Assign DDRC address
        if (portx_port != NULL)
            *portx_port = &PORTC; // Assign PORTC address
    }
    // Check if the pin is within the range for PORTD
    else if (IS_IN_RANGE(pin, GPIO_PD0, GPIO_PD7))
    {
        port_bit = (uint8_t)(pin - GPIO_PD_OFFSET); // Calculate bit position
        if (ddr_port != NULL)
            *ddr_port = &DDRD; // Assign DDRD address
        if (portx_port != NULL)
            *portx_port = &PORTD; // Assign PORTD address
    }
    else
    {
        return UCHAR_MAX; // Return UCHAR_MAX if pin is invalid
    }

    return port_bit;
}

/* Public API Implementation **************************************************/

/**
 * @brief Sets the direction of a GPIO pin (input/output).
 *
 * This function sets the direction of a specific GPIO pin to either input or output.
 * It uses the `gpio_get_port_bit` helper function to find the port and bit positions.
 *
 * @param pin The pin to configure
 * @param direction GPIO_DIR_INPUT or GPIO_DIR_OUTPUT
 */
void gpio_pin_set_direction(uint8_t pin, uint8_t direction)
{
    uint8_t port_bit = 0;
    volatile uint8_t *ddr_port = NULL;

    port_bit = gpio_get_port_bit(pin, &ddr_port, NULL); // Get port bit position for the pin

    // Set direction to OUTPUT or INPUT based on the provided argument
    if (IS_DIRECTION_OUTPUT(direction))
        set_bit(ddr_port, port_bit);
    else if (IS_DIRECTION_INPUT(direction))
        clr_bit(ddr_port, port_bit);
}

/**
 * @brief Writes a value to a GPIO pin.
 *
 * This function writes a high or low value to the specified GPIO pin.
 * It uses the `gpio_get_port_bit` helper function to find the appropriate port.
 *
 * @param pin The pin to write to
 * @param value GPIO_HIGH or GPIO_LOW
 */
void gpio_pin_write(uint8_t pin, uint8_t value)
{
    // volatile uint8_t *ddr_port = NULL;
    volatile uint8_t *portx_port = NULL;

    uint8_t port_bit = gpio_get_port_bit(pin, NULL, &portx_port); // Get port bit position for the pin

    // Set pin to HIGH or LOW based on the value argument
    if (IS_PIN_TO_HIGH(value))
        set_bit(portx_port, port_bit); // Set pin high
    else
        clr_bit(portx_port, port_bit); // Set pin low
}

/**
 * @brief Configures the pull-up resistor for a GPIO pin.
 *
 * This function enables or disables the internal pull-up resistor for the specified pin.
 *
 * @param pin The pin to configure
 * @param enable_pullup true to enable, false to disable
 */
void gpio_pin_pullup(uint8_t pin, bool enable_pullup)
{
    uint8_t port_bit = 0;
    volatile uint8_t *portx_port = NULL;

    port_bit = gpio_get_port_bit(pin, NULL, &portx_port); // Get port bit position for the pin

    // Enable or disable pull-up resistor
    if (enable_pullup)
        set_bit(portx_port, port_bit); // Enable pull-up
    else
        clr_bit(portx_port, port_bit); // Disable pull-up
}

/**
 * @brief Enables/disables pull-up resistors for all pins.
 *
 * This function enables or disables the pull-up resistors for all GPIO pins by manipulating the PUD bit in the MCUCR register.
 *
 * @param enable_all true to enable all pull-ups, false to disable all pull-ups
 */
void gpio_pullup_enable_all(bool enable_all)
{
    uint8_t port_bit = 4; // PUD bit in MCUCR is bit 4

    // Enable or disable pull-up resistors for all pins based on the argument
    if (enable_all)
        set_bit(&MCUCR, port_bit); // Enable pull-ups
    else
        clr_bit(&MCUCR, port_bit); // Disable pull-ups
}

/**
 * @brief Toggles the state of a GPIO pin.
 *
 * This function changes the state of a pin from HIGH to LOW or from LOW to HIGH.
 *
 * @param pin The pin to toggle
 */
void gpio_pin_toggle_state(uint8_t pin)
{
    uint8_t port_bit = 0;
    volatile uint8_t *portx_port = NULL;

    port_bit = gpio_get_port_bit(pin, NULL, &portx_port); // Get port bit position for the pin
    toogle_bit(portx_port, port_bit);                     // Toggle the state of the pin
}

/* Interrupt Functions ********************************************************/

/**
 * @brief Enables INT0 interrupt with specified trigger condition.
 *
 * This function enables the INT0 interrupt and sets the trigger condition (level or edge).
 *
 * @param event_mask One of GPIO_IRQ_* constants for trigger condition
 */
void gpio_enable_int0_interrupt(uint8_t event_mask)
{
    set_bit(&EIMSK, 0); // Enable INT0 interrupt

    // Set the trigger condition based on the provided event mask
    switch (event_mask)
    {
    case GPIO_IRQ_LEVEL_LOW:
        clr_bitmask(&EICRA, 0b00000011); // Level low trigger
        break;
    case GPIO_IRQ_ANY_CHANGE:
        clr_bit(&EICRA, 1); // Any change trigger
        set_bit(&EICRA, 0);
        break;
    case GPIO_IRQ_EDGE_FALL:
        clr_bit(&EICRA, 0); // Falling edge trigger
        set_bit(&EICRA, 1);
        break;
    case GPIO_IRQ_EDGE_RISE:
        set_bitmask(&EICRA, 0b00000011); // Rising edge trigger
        break;
    default:
        break;
    }
}

// The rest of the interrupt functions follow a similar pattern, configuring INT1, PCINTs, and disabling interrupts

/* Clean up macros ************************************************************/
#undef IS_IN_RANGE
#undef IS_DIRECTION_OUTPUT
#undef IS_DIRECTION_INPUT
#undef IS_PIN_TO_HIGH
#undef IS_PIN_TO_LOW
#undef GPIO_PB_OFFSET
#undef GPIO_PC_OFFSET
#undef GPIO_PD_OFFSET
