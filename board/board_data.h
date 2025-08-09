#pragma once 

#include <array>

//macros 
//#include "../utils/"

//globals
#include "../defs.h"

/** 
 * @brief Maps piece, side-to-move, rank and file to their character symbol.
 * 
 * For example: black king = 'k', white pawn = 'P', file 1 = 'a' etc.
 */
extern char pieceToCharacter[];
extern char SideChar[];
extern char rankToCharacter[];
extern char fileToCharacter[];

/**
 * @brief Lookup tables for piece attributes.
 *
 * Each array index corresponds to a piece constant (EMPTY, wP, wN, ..., bK).
 * Boolean arrays return true if the piece matches the property:
 * - isBigPiece:    true if piece is not a pawn
 * - isMajorPiece:  true if rook or queen
 * - isMinorPiece:  true if bishop or knight
 * - isPawn:        true if pawn
 * - isKnight:      true if knight
 * - isKing:        true if king
 * - isRookOrQueen: true if rook or queen
 * - isBishopOrQueen: true if bishop or queen
 * - isSlidingPiece: true if bishop, rook, or queen
 */

extern bool isBigPiece[13];
extern bool isMajorPiece[13];
extern bool isMinorPiece[13];
extern bool isPawn[13];	
extern bool isKnight[13];
extern bool isKing[13];
extern bool isRookQueen[13];
extern bool isBishopQueen[13];
extern bool isSlingPiece[13];

/**
 * @brief Material value of each piece type.
 *
 * Indexed by piece constant (EMPTY, wP, wN, ..., bK).
 * Value is in centipawns, e.g., pawn = 100, queen = 1000, king = 50000.
 */
extern int pieceValue[13];

/**
 * @brief Color of each piece type.
 *
 * Indexed by piece constant (EMPTY, wP, wN, ..., bK).
 * Possible values: WHITE, BLACK, or BOTH (for EMPTY).
 */
extern int pieceColor[13];

/**
 * @brief Square mirroring lookup from white's perspective.
 *
 * Indexed by 0–63 board square index. Returns the mirrored
 * square index as if the board were flipped vertically.
 * Useful for symmetrical evaluation and move generation.
 */
extern int Mirror64[64];