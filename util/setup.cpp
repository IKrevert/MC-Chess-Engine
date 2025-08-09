// setup.cpp

#include "setup.h"
#include <cstdio>
#include <cstdlib>
#include "../move/move_generator.h"


#define RAND_64  ( (U64)std::rand()        | \
                  (U64)std::rand() << 15   | \
                  (U64)std::rand() << 30   | \
                  (U64)std::rand() << 45   | \
                 ((U64)std::rand() & 0xF) << 60 )

// globals (unchanged)
int  square120To64[BRD_SQ_NUM];
int  square64To120[64];

U64  setBitMask[64];
U64  clearBitMask[64];

U64  zobristPieceKeys[13][120];
U64  zobristSideKey;
U64  zobristCastleKeys[16];

int  fileIndex120[BRD_SQ_NUM];
int  rankIndex120[BRD_SQ_NUM];

U64  fileBitMask[8];
U64  rankBitMask[8];

U64  blackPassedPawnMask[64];
U64  whitePassedPawnMask[64];
U64  isolatedPawnMask[64];

S_OPTIONS engineOptions[1];

// ---- functions (same behavior, C++-style locals) ----

void initEvaluationMasks() {
    // zero file/rank masks
    for (int i = 0; i < 8; ++i) {
        fileBitMask[i] = 0ULL;
        rankBitMask[i] = 0ULL;
    }

    // build file/rank masks
    for (int r = RANK_8; r >= RANK_1; --r) {
        for (int f = FILE_A; f <= FILE_H; ++f) {
            const int sq = r * 8 + f;
            fileBitMask[f] |= (1ULL << sq);
            rankBitMask[r] |= (1ULL << sq);
        }
    }

    // zero pawn structure masks
    for (int sq = 0; sq < 64; ++sq) {
        isolatedPawnMask[sq]     = 0ULL;
        whitePassedPawnMask[sq]  = 0ULL;
        blackPassedPawnMask[sq]  = 0ULL;
    }

    for (int sq = 0; sq < 64; ++sq) {
        // same-file forward/backward rays
        for (int tsq = sq + 8; tsq < 64; tsq += 8) {
            whitePassedPawnMask[sq] |= (1ULL << tsq);
        }
        for (int tsq = sq - 8; tsq >= 0; tsq -= 8) {
            blackPassedPawnMask[sq] |= (1ULL << tsq);
        }

        const int file = fileIndex120[SQ120(sq)];

        if (file > FILE_A) {
            isolatedPawnMask[sq] |= fileBitMask[file - 1];

            for (int tsq = sq + 7; tsq < 64; tsq += 8) {
                whitePassedPawnMask[sq] |= (1ULL << tsq);
            }
            for (int tsq = sq - 9; tsq >= 0; tsq -= 8) {
                blackPassedPawnMask[sq] |= (1ULL << tsq);
            }
        }

        if (file < FILE_H) {
            isolatedPawnMask[sq] |= fileBitMask[file + 1];

            for (int tsq = sq + 9; tsq < 64; tsq += 8) {
                whitePassedPawnMask[sq] |= (1ULL << tsq);
            }
            for (int tsq = sq - 7; tsq >= 0; tsq -= 8) {
                blackPassedPawnMask[sq] |= (1ULL << tsq);
            }
        }
    }
}

void InitFilesrankIndex120() {
    // mark offboard
    for (int i = 0; i < BRD_SQ_NUM; ++i) {
        fileIndex120[i] = OFFBOARD;
        rankIndex120[i] = OFFBOARD;
    }

    // fill valid squares
    for (int r = RANK_1; r <= RANK_8; ++r) {
        for (int f = FILE_A; f <= FILE_H; ++f) {
            const int sq = FR2SQ(f, r);
            fileIndex120[sq] = f;
            rankIndex120[sq] = r;
        }
    }
}

void initZobristKeys() {
    for (int p = 0; p < 13; ++p) {
        for (int sq = 0; sq < 120; ++sq) {
            zobristPieceKeys[p][sq] = RAND_64;
        }
    }
    zobristSideKey = RAND_64;

    for (int i = 0; i < 16; ++i) {
        zobristCastleKeys[i] = RAND_64;
    }
}

void initBitMasks() {
    for (int i = 0; i < 64; ++i) {
        setBitMask[i]   = 0ULL;
        clearBitMask[i] = 0ULL;
    }
    for (int i = 0; i < 64; ++i) {
        setBitMask[i]   = (1ULL << i);
        clearBitMask[i] = ~setBitMask[i];
    }
}

void initSquareIndexMaps() {
    // sentinels
    for (int i = 0; i < BRD_SQ_NUM; ++i) square120To64[i] = 65;
    for (int i = 0; i < 64;        ++i) square64To120[i]   = 120;

    int sq64 = 0;
    for (int r = RANK_1; r <= RANK_8; ++r) {
        for (int f = FILE_A; f <= FILE_H; ++f) {
            const int sq120 = FR2SQ(f, r);
            ASSERT(SqOnBoard(sq120));
            square64To120[sq64]   = sq120;
            square120To64[sq120]  = sq64;
            ++sq64;
        }
    }
}

void initializeEngine() {
    initSquareIndexMaps();
    initBitMasks();
    initZobristKeys();
    InitFilesrankIndex120();
    initEvaluationMasks();
    InitMvvLva(); // from move_generator
}
