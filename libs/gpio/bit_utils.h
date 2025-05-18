/*
 * bit_utils.h
 *
 * Created: 01/05/2025 22:49:06
 * Author: chval
 *
 * @brief Header file containing utility functions for manipulating individual bits or bitmasks within bytes.
 * These functions allow for easy setting, clearing, complementing, toggling, and checking of specific bits or bitmasks.
 * They are useful for low-level register manipulation in embedded systems.
 */

#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include <stdint.h>    // For standard integer types like uint8_t
#include <stdbool.h>   // For boolean type (true/false)

/**
 * @brief Sets a specific bit in a byte to 1.
 *
 * This function sets the bit at the specified position (bit) in the byte pointed to by the given address.
 * 
 * @param byte A pointer to the byte in which the bit will be set.
 * @param bit The position of the bit to set (0-7).
 */
void set_bit(volatile uint8_t *byte, uint8_t bit);

/**
 * @brief Clears a specific bit in a byte (sets it to 0).
 *
 * This function clears the bit at the specified position (bit) in the byte pointed to by the given address.
 * 
 * @param byte A pointer to the byte in which the bit will be cleared.
 * @param bit The position of the bit to clear (0-7).
 */
void clr_bit(volatile uint8_t *byte, uint8_t bit);

/**
 * @brief Complements (flips) a specific bit in a byte.
 *
 * This function inverts the value of the bit at the specified position (bit). If the bit is 0, it becomes 1, and vice versa.
 * 
 * @param byte A pointer to the byte in which the bit will be complemented.
 * @param bit The position of the bit to complement (0-7).
 */
void cpl_bit(volatile uint8_t *byte, uint8_t bit);

/**
 * @brief Gets the value of a specific bit in a byte.
 *
 * This function returns the current value (0 or 1) of the bit at the specified position (bit).
 * 
 * @param byte A pointer to the byte from which the bit value will be retrieved.
 * @param bit The position of the bit to get (0-7).
 * 
 * @return The value of the specified bit (0 or 1).
 */
uint8_t get_bit(volatile uint8_t byte, uint8_t bit);

/**
 * @brief Sets a bitmask in a byte.
 *
 * This function sets the bits defined by the bitmask (where 1 represents bits to set) in the byte pointed to by the given address.
 * 
 * @param byte A pointer to the byte in which the bitmask will be set.
 * @param bitmask A bitmask representing which bits to set (1 = set, 0 = no change).
 */
void set_bitmask(volatile uint8_t *byte, uint8_t bitmask);

/**
 * @brief Clears a bitmask in a byte.
 *
 * This function clears the bits defined by the bitmask (where 1 represents bits to clear) in the byte pointed to by the given address.
 * 
 * @param byte A pointer to the byte in which the bitmask will be cleared.
 * @param bitmask A bitmask representing which bits to clear (1 = clear, 0 = no change).
 */
void clr_bitmask(volatile uint8_t *byte, uint8_t bitmask);

/**
 * @brief Toggles (flips) a specific bit in a byte.
 *
 * This function inverts the value of the bit at the specified position (bit). It works similarly to the complement function, 
 * but it’s used specifically for toggling.
 * 
 * @param byte A pointer to the byte in which the bit will be toggled.
 * @param bit The position of the bit to toggle (0-7).
 */
void toogle_bit(volatile uint8_t *byte, uint8_t bit);

/**
 * @brief Toggles (flips) a bitmask in a byte.
 *
 * This function inverts the bits defined by the bitmask (where 1 represents bits to toggle) in the byte pointed to by the given address.
 * 
 * @param byte A pointer to the byte in which the bitmask will be toggled.
 * @param bitmask A bitmask representing which bits to toggle (1 = toggle, 0 = no change).
 */
void toogle_bitmask(volatile uint8_t *byte, uint8_t bitmask);

/**
 * @brief Checks if a specific bit is set (1) in a byte.
 *
 * This function checks the value of the bit at the specified position (bit). If the bit is set (1), the function returns true.
 * Otherwise, it returns false.
 * 
 * @param byte A pointer to the byte in which the bit will be checked.
 * @param bit The position of the bit to check (0-7).
 * 
 * @return true if the bit is set (1), false otherwise.
 */
bool is_bit_set(volatile uint8_t byte, uint8_t bit);

#endif
