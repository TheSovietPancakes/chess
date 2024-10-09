#pragma once

// No! Look away! This is bad practice!

#include <string>
#include <array>
#include <unordered_map>

#include "piece.hpp"

const char *startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
const std::unordered_map<char, int> pieceCharTable = {
    {'p', Piece::Pawn | Piece::Black},
    {'r', Piece::Rook | Piece::Black},
    {'n', Piece::Knight | Piece::Black},
    {'b', Piece::Bishop | Piece::Black},
    {'q', Piece::Queen | Piece::Black},
    {'k', Piece::King | Piece::Black},
    {'P', Piece::Pawn | Piece::White},
    {'R', Piece::Rook | Piece::White},
    {'N', Piece::Knight | Piece::White},
    {'B', Piece::Bishop | Piece::White},
    {'Q', Piece::Queen | Piece::White},
    {'K', Piece::King | Piece::White}
};

std::array<int, 64> loadFen(std::string fen) {
    std::array<int, 64> squares = {0};
    int horizontal = 0;
    int vertical = 0;
    for (int i = 0; i < fen.length(); i++) {
        char c = fen[i];
        if (c == '/') {
            horizontal = 0;
            vertical++;
            continue;
        }
        if (c >= '1' && c <= '8') {
            horizontal += c - '0';
        } else {
            int piece = 0;
            if (pieceCharTable.find(c) != pieceCharTable.end()) {
                piece = pieceCharTable.at(c);
            }
            squares[vertical * 8 + horizontal] = piece;
            horizontal++;
        }
    }
    return squares;
};