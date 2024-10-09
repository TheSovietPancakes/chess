#pragma once
#include <vector>
#include <cstdint>

#include "board.hpp"
#include "move.hpp"


class MoveGenerator {
    public:

    static inline std::vector<Move> generateMoves(Move &prevMove) {
        std::vector<Move> moves;

        // Bro copilot just wrote the whole damn function are you JOKING
        for (int i = 0; i < 64; i++) {
            if (Board::Squares[i] == Piece::Empty) continue;
            if (Piece::getColor(Board::Squares[i]) != Board::ColToMove) continue;

            // Determine the piece type
            int type = Piece::getType(Board::Squares[i]);
            if (type == Piece::Pawn)
                pawnMoves(i, &moves, prevMove);
            

            if (Piece::isSliding(type))
                slidingPiece(i, &moves);

            if (type == Piece::Knight)
                knightMoves(i, &moves);
            
            if (type == Piece::King)
                kingMoves(i, &moves);
        }

        return moves;
    }

    static inline void kingMoves(int square, std::vector<Move> *moves) {
        const int directions[8] = { -8, 8, -1, 1 };
        for (int i = 0; i < 8; i++) {
            int targetSquare = square + directions[i];
            if (abs((targetSquare % 8) - (square % 8)) > 1) continue;
            if (Piece::getColor(Board::Squares[targetSquare]) == Board::ColToMove) continue;
            bool isCapture = Piece::getColor(Board::Squares[targetSquare]) == Piece::oppositeColor(Board::ColToMove);
            if (isCapture) {
                moves->push_back({square, targetSquare, Move::Capture });
            } else {
                moves->push_back({square, targetSquare, Move::Normal });
            }
        }
        // Castling
        if (Board::ColToMove == Piece::White && Board::WhiteCanCastle) {
            if (Board::Squares[57] == Piece::Empty && Board::Squares[58] == Piece::Empty && Board::Squares[59] == Piece::Empty) {
                moves->push_back({square, 58, Move::Castle});
            }
            if (Board::Squares[61] == Piece::Empty && Board::Squares[62] == Piece::Empty) {
                moves->push_back({square, 62, Move::Castle});
            }
        }
        if (Board::ColToMove == Piece::Black && Board::BlackCanCastle) {
            if (Board::Squares[1] == Piece::Empty && Board::Squares[2] == Piece::Empty && Board::Squares[3] == Piece::Empty) {
                moves->push_back({square, 2, Move::Castle});
            }
            if (Board::Squares[5] == Piece::Empty && Board::Squares[6] == Piece::Empty) {
                moves->push_back({square, 6, Move::Castle});
            }
        }
    };

    static inline void knightMoves(int square, std::vector<Move> *moves) {
        const int directions[8] = {-17, -15, -10, -6, 6, 10, 15, 17};
        // Do not wrap around the edges weirdly
        for (int i = 0; i < 8; i++) {
            int targetSquare = square + directions[i];
            // Check if on the edge of the board
            // to prevent "wrapping" around
            if (abs((targetSquare % 8) - (square % 8)) > 2) continue;
            if (Piece::getColor(Board::Squares[targetSquare]) == Board::ColToMove) continue;
            bool isCapture = Piece::getColor(Board::Squares[targetSquare]) == Piece::oppositeColor(Board::ColToMove);
            if (isCapture) {
                moves->push_back({square, targetSquare, Move::Capture });
            } else {
                moves->push_back({square, targetSquare, Move::Normal });
            }
        }

    }

    static inline void slidingPiece(int square, std::vector<Move> *moves) {
        
        const int directions[8] = {-8, 8, -1, 1,  -9, -7, 7, 9 };
        int type = Piece::getType(Board::Squares[square]);

        int startIdx = type == Piece::Bishop ? 4 : 0;
        int endIdx = type == Piece::Rook ? 4 : 8;

        for (int directionIdx = startIdx; directionIdx < endIdx; directionIdx++) {
            for (int n = 0; n < Board::NumToEdge[square][directionIdx]; n++) {
                int direction = directions[directionIdx];
                int targetSquare = square + direction * (n + 1);
                if (targetSquare < 0 || targetSquare > 63) break;
                if (Piece::getColor(Board::Squares[targetSquare]) == Board::ColToMove) break;
                if (Piece::getColor(Board::Squares[targetSquare]) == Piece::oppositeColor(Board::ColToMove)) {
                    moves->push_back({square, targetSquare, Move::Capture});
                    break;
                };
                moves->push_back({square, targetSquare, Move::Normal});
            }
        }
    };

    // I hate pawns.
    static inline void pawnMoves(int square, std::vector<Move> *moves, Move &prevMove) {
        int piece = Board::Squares[square];
        int col = Board::ColToMove;

        // Check if piece in front of us
        int forward = col == Piece::White ? -8 : 8;
        if (Board::Squares[square + forward] == Piece::Empty) {
            moves->push_back({square, square + forward, Move::Normal});
        }
        // Double push (only if on starting rank)
        if (col == Piece::White && square / 8 == 6 || col == Piece::Black && square / 8 == 1) {
            if (Board::Squares[square + 2 * forward] == Piece::Empty) {
                moves->push_back({square, square + 2 * forward, Move::DoublePawn});
            }
        }
        // Check diagonal captures
        // If on edge, don't check that side
        if (square % 8 != 0) {
            // Check for regular capture
            if (Board::Squares[square + forward - 1] != Piece::Empty && Piece::getColor(Board::Squares[square + forward - 1]) != col) {
                moves->push_back({ square, square + forward - 1, Move::Capture });
            }
            // Can i en passant here?
            if (prevMove.Flags & Move::DoublePawn && prevMove.End == square - 1) {
                moves->push_back({ square, square + forward - 1, Move::EnPassant | Move::Capture });
            }
        }

        if (square % 8 != 7) {
            if (Board::Squares[square + forward + 1] != Piece::Empty && Piece::getColor(Board::Squares[square + forward + 1]) != col) {
                moves->push_back({square, square + forward + 1, Move::Capture});
            }
            if (prevMove.Flags & Move::DoublePawn && prevMove.End == square + 1) {
                moves->push_back({ square, square + forward + 1, Move::EnPassant | Move::Capture });
            }
        }
        // Did we land on the last square? (promote?)
        // This includes captures as well.
        if (square / 8 == 0 || square / 8 == 7) {
            moves->push_back({square, square + forward, Move::PromotionToQueen});
            moves->push_back({square, square + forward, Move::PromotionToRook});
            moves->push_back({square, square + forward, Move::PromotionToBishop});
            moves->push_back({square, square + forward, Move::PromotionToKnight});
        }
    }
};