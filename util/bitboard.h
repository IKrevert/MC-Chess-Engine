#pragma once

#include "../defs.h"

#define POP(b) popLSB(b)
#define CNT(b) countSetBits(b)

/**
 * @brief Lookup table for bit index extraction.
 */
extern const int BitTable[64];

/**
 * @brief Pops the least significant 1-bit from a bitboard and returns its index (0–63).
 * 
 * @param bb Pointer to the bitboard to modify.
 * @return Index (0–63) of the popped bit.
 */
extern int popLSB(U64* bb);

/**
 * @brief Counts the number of 1-bits in a bitboard.
 * 
 * @param b Bitboard value.
 * @return Number of set bits.
 */
extern int countSetBits(U64 b);

/**
 * @brief Prints a visual representation of a bitboard to stdout.
 * 
 * 'X' marks set bits, '-' marks unset bits.
 * 
 * @param bb Bitboard to print.
 */
extern void printBitBoard(U64 bb);
