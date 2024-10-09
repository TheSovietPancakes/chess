#pragma once

struct Piece {

    static const int Empty = 0;
    static const int Pawn = 1;
    static const int King = 2;
    static const int Knight = 3;
    
    // Sliding pieces
    static const int Rook = 4;
    static const int Bishop = 5;
    static const int Queen = 6;
    static const int Black = 8;
    static const int White = 16;

    static bool isSliding(int pc) {
        return getType(pc) >= Rook;
    }

    static int getColor(int pc) {
        return pc & 0b11000;
    };

    static int getType(int pc) {
        return pc & 0b00111;
    };

    static int oppositeColor(int col) {
        return col == White ? Black : White;
    };
};