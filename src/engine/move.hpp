#pragma once

struct Move {
    int Start;
    int End;

    uint16_t Flags;
    static inline const int Normal = 1;
    static inline const int Capture = 2;
    static inline const int DoublePawn = 4;
    static inline const int EnPassant = 8;
    static inline const int Castle = 16;
    static inline const int PromotionToKnight = 32;
    static inline const int PromotionToQueen = 64;
    static inline const int PromotionToRook = 128;
    static inline const int PromotionToBishop = 256;
    static inline const int Check = 512;

    // Default constructor
    Move(int start, int end, uint16_t flags) : Start(start), End(end), Flags(flags) {};

    static inline bool wasPromote(int flags) {
        return flags & 0b111100000;
    }
};