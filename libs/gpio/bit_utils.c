/*
 * bit_utils.c
 *
 * Created: 01/05/2025 22:49:18
 * Author: chval
 *
 * @brief This file contains functions to manipulate individual bits or bitmasks within bytes.
 * These functions are typically used for low-level operations such as setting, clearing, complementing, toggling, 
 * and checking bits in microcontroller registers.
 */

#include "bit_utils.h"

/**
 * @brief Sets a specific bit in a byte to 1.
 *
 * This function uses the bitwise OR operator to set the bit at the specified position (bit) in the byte.
 * The byte is pointed to by the `byte` argument, and the bit to be set is specified by the `bit` argument.
 * 
 * @param byte A pointer to the byte where the bit will be set.
 * @param bit The position of the bit to set (0-7).
 */
void set_bit(volatile uint8_t *byte, uint8_t bit)
{
	// Use bitwise OR to set the specified bit to 1.
	*byte |= (1 << bit);
}

/**
 * @brief Clears a specific bit in a byte (sets it to 0).
 *
 * This function uses the bitwise AND operator with the negated bit mask to clear the bit at the specified position.
 * The byte is pointed to by the `byte` argument, and the bit to be cleared is specified by the `bit` argument.
 * 
 * @param byte A pointer to the byte where the bit will be cleared.
 * @param bit The position of the bit to clear (0-7).
 */
void clr_bit(volatile uint8_t *byte, uint8_t bit)
{
	// Use bitwise AND with the negated mask to set the specified bit to 0.
	*byte &= ~(1 << bit);
}

/**
 * @brief Complements (flips) a specific bit in a byte.
 *
 * This function uses the bitwise XOR operator to toggle (flip) the bit at the specified position.
 * If the bit is 0, it becomes 1, and if the bit is 1, it becomes 0.
 * 
 * @param byte A pointer to the byte where the bit will be complemented.
 * @param bit The position of the bit to complement (0-7).
 */
void cpl_bit(volatile uint8_t *byte, uint8_t bit)
{
	// Use bitwise XOR to toggle the specified bit.
	*byte ^= (1 << bit);
}

/**
 * @brief Gets the value of a specific bit in a byte.
 *
 * This function shifts the byte to the right by the bit position and masks all other bits, 
 * returning the value of the specified bit (either 0 or 1).
 * 
 * @param byte A pointer to the byte from which the bit value will be retrieved.
 * @param bit The position of the bit to get (0-7).
 * 
 * @return The value of the specified bit (0 or 1).
 */
uint8_t get_bit(volatile uint8_t byte, uint8_t bit)
{
	// Shift the byte to the right by `bit` positions and mask the result to get the bit value.
	return (byte >> bit) & 1;
}

/**
 * @brief Sets multiple bits in a byte using a bitmask.
 *
 * This function uses the bitwise OR operator to set the bits defined by the bitmask to 1.
 * The `bitmask` argument is a mask where bits set to 1 will be set in the byte.
 * 
 * @param byte A pointer to the byte where the bitmask will be set.
 * @param bitmask The bitmask representing the bits to set to 1.
 */
void set_bitmask(volatile uint8_t *byte, uint8_t bitmask)
{
	// Use bitwise OR to set the bits defined by the bitmask.
	*byte |= bitmask;
}

/**
 * @brief Clears multiple bits in a byte using a bitmask.
 *
 * This function uses the bitwise AND operator with the negated bitmask to clear the bits defined by the bitmask.
 * The `bitmask` argument is a mask where bits set to 1 will be cleared in the byte.
 * 
 * @param byte A pointer to the byte where the bitmask will be cleared.
 * @param bitmask The bitmask representing the bits to clear.
 */
void clr_bitmask(volatile uint8_t *byte, uint8_t bitmask)
{
	// Use bitwise AND with the negated bitmask to clear the bits defined by the mask.
	*byte &= ~bitmask;
}

/**
 * @brief Toggles (flips) multiple bits in a byte using a bitmask.
 *
 * This function uses the bitwise XOR operator to toggle (flip) the bits defined by the bitmask.
 * The `bitmask` argument is a mask where bits set to 1 will be toggled.
 * 
 * @param byte A pointer to the byte where the bitmask will be toggled.
 * @param bitmask The bitmask representing the bits to toggle.
 */
void toogle_bitmask(volatile uint8_t *byte, uint8_t bitmask)
{
	// Use bitwise XOR to toggle the bits defined by the bitmask.
	*byte ^= bitmask;
}

/**
 * @brief Toggles (flips) a specific bit in a byte.
 *
 * This function uses the bitwise XOR operator to toggle (flip) a specific bit at the given position.
 * 
 * @param byte A pointer to the byte where the bit will be toggled.
 * @param bit The position of the bit to toggle (0-7).
 */
void toogle_bit(volatile uint8_t *byte, uint8_t bit)
{
	// Use bitwise XOR to toggle the specified bit.
	*byte ^= (1 << bit);
}

/**
 * @brief Checks if a specific bit is set (1) in a byte.
 *
 * This function checks the value of the bit at the specified position. If the bit is set to 1,
 * it returns true; otherwise, it returns false.
 * 
 * @param byte A pointer to the byte where the bit will be checked.
 * @param bit The position of the bit to check (0-7).
 * 
 * @return true if the bit is set (1), false if the bit is not set (0).
 */
bool is_bit_set(volatile uint8_t byte, uint8_t bit)
{
	// Use the get_bit function to check if the bit is set to 1.
	return get_bit(byte, bit);
}
