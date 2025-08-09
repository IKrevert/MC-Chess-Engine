#pragma once
#include "../defs.h"

/**
 * @brief Lookup table to convert 120-based board index to 64-based.
 */
 extern int square120To64[BRD_SQ_NUM];

/**
 * @brief Lookup table to convert 64-based board index to 120-based.
 */
 extern int square64To120[64];

/**
 * @brief Bit masks for setting and clearing individual bits on a bitboard.
 */
 extern U64 setBitMask[64];
 extern U64 clearBitMask[64];

/**
 * @brief Zobrist hashing keys for each piece on each square.
 */
 extern U64 zobristPieceKeys[13][120];

/**
 * @brief Zobrist hashing key for side to move.
 */
 extern U64 zobristSideKey;

/**
 * @brief Zobrist hashing keys for castling rights.
 */
 extern U64 zobristCastleKeys[16];

/**
 * @brief File and rank lookup tables for each square (120-based).
 */
 extern int fileIndex120[BRD_SQ_NUM];
 extern int rankIndex120[BRD_SQ_NUM];

/**
 * @brief Bitboard masks for each file and rank.
 */
 extern U64 fileBitMask[8];
 extern U64 rankBitMask[8];

/**
 * @brief Bitboard masks for pawn structure evaluation.
 */
 extern U64 blackPassedPawnMask[64];
 extern U64 whitePassedPawnMask[64];
 extern U64 isolatedPawnMask[64];

/**
 * @brief Engine options structure.
 */
 extern S_OPTIONS engineOptions[1];

/**
 * @brief Initialize evaluation-related bitboard masks.
 */
 void initEvaluationMasks();

/**
 * @brief Initialize file and rank lookup arrays.
 */
 void InitFilesrankIndex120();

/**
 * @brief Initialize Zobrist hash keys.
 */
 void initZobristKeys();

/**
 * @brief Initialize bit masks for individual bitboard operations.
 */
 void initBitMasks();

/**
 * @brief Initialize lookup tables for 120->64 square indexing.
 */
 void initSquareIndexMaps();

/**
 * @brief Run all initialization routines.
 */
 extern void initializeEngine();
