#pragma once
#include <iostream>
#include <array>

#include "piece.hpp"
#include "fen.hpp"
#include "move.hpp"

class Board {
    public:

    static inline std::array<int, 64> Squares;
    static inline int ColToMove = Piece::White;
    static inline std::array<std::array<int, 8>, 64> NumToEdge;
    static inline bool WhiteCanCastle = true;
    static inline bool BlackCanCastle = true;

    static void init() {
        WhiteCanCastle = true;
        BlackCanCastle = true;
        Squares = loadFen(startFen);
        ComputeEdgeCount();
        ColToMove = Piece::White;
        std::cout << "Board was initialized!!" << std::endl;
    }

    static inline void MakeMove(Move mv) {
        // Remove castle rights if move is either castle, king or rook move
        if (mv.Flags & Move::Castle || Piece::getType(Squares[mv.Start]) == Piece::King || Piece::getType(Squares[mv.Start]) == Piece::Rook) {
            if (ColToMove == Piece::White) {
                WhiteCanCastle = false;
            } else {
                BlackCanCastle = false;
            }
        }

        // Move the rook and king if it WAS actually a castle
        if (mv.Flags & Move::Castle) {
            if (mv.End == 2) {
                Squares[0] = Piece::Empty;
                Squares[3] = Piece::Rook | ColToMove;
            }
            if (mv.End == 6) {
                Squares[7] = Piece::Empty;
                Squares[5] = Piece::Rook | ColToMove;
            }
            if (mv.End == 58) {
                Squares[56] = Piece::Empty;
                Squares[59] = Piece::Rook | ColToMove;
            }
            if (mv.End == 62) {
                Squares[63] = Piece::Empty;
                Squares[61] = Piece::Rook | ColToMove;
            }
        }

        // En passant, remove the captured pawn
        if (mv.Flags & Move::EnPassant) {
            // Set the captured piece to empty
            // (remove the pawn under ours that moved)

            // En passant works by having your pawn above the captured one
            // so we dont even know how to get the direction it came from
            // its just there!
            Squares[mv.End + (ColToMove == Piece::White ? 8 : -8)] = Piece::Empty;
        }

        // Move the piece normally.
        Squares[mv.End] = Squares[mv.Start];
        Squares[mv.Start] = Piece::Empty;
        ColToMove = Piece::oppositeColor(ColToMove);
    }

    static void ComputeEdgeCount() {
        for (int i = 0; i < 64; i++) {
            int x = i % 8;
            int y = i / 8;
            //                            N   S   W  E   NW  NE SW SE
            // const int directions[8] = {-8, 8, -1, 1,  -9, -7, 7, 9 };
            NumToEdge[i][0] = y; // North
            NumToEdge[i][1] = 7 - y; // South
            NumToEdge[i][2] = x; // West
            NumToEdge[i][3] = 7 - x; // East
            NumToEdge[i][4] = std::min(x, y); // NW
            NumToEdge[i][5] = std::min(7 - x, y); // NE
            NumToEdge[i][6] = std::min(x, 7 - y); // SW
            NumToEdge[i][7] = std::min(7 - x, 7 - y); // SE

        }
    }
};