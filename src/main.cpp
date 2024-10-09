// Include SDL2 and SDL2_image
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <unordered_map>

#include "render/window.hpp"
#include "engine/board.hpp"
#include "engine/movegenerator.hpp"

int main() {
    // no args required :)
    // create a new render window

    if (SDL_Init(SDL_INIT_VIDEO) > 0) {
        std::cerr << "SDL failed to init :(" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        exit(-1);
    }
    if (!IMG_Init(IMG_INIT_PNG)) {
        std::cerr << "SDL image failed to init :(" << std::endl;
        // errors flow through same channel
        std::cerr << IMG_GetError() << std::endl;
        exit(-1);
    }
    if (!Mix_Init(MIX_INIT_MP3)) {
        std::cerr << "SDL mixer failed to init :(" << std::endl;
        std::cerr << Mix_GetError() << std::endl;
        exit(-1);
    }
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_Chunk *capture = Mix_LoadWAV("res/sounds/capture.mp3");
    RenderWindow window ("Chess engine", 800, 800);

    SDL_Texture *whitePawn = window.loadSVG("res/pieces/wp.svg");
    SDL_Texture *whiteKing = window.loadSVG("res/pieces/wk.svg");
    SDL_Texture *whiteQueen = window.loadSVG("res/pieces/wq.svg");
    SDL_Texture *whiteRook = window.loadSVG("res/pieces/wr.svg");
    SDL_Texture *whiteBishop = window.loadSVG("res/pieces/wb.svg");
    SDL_Texture *whiteKnight = window.loadSVG("res/pieces/wn.svg");
    // seperate the colors!!!!!!!
    SDL_Texture *blackPawn = window.loadSVG("res/pieces/bp.svg");
    SDL_Texture *blackKing = window.loadSVG("res/pieces/bk.svg");
    SDL_Texture *blackQueen = window.loadSVG("res/pieces/bq.svg");
    SDL_Texture *blackRook = window.loadSVG("res/pieces/br.svg");
    SDL_Texture *blackBishop = window.loadSVG("res/pieces/bb.svg");
    SDL_Texture *blackKnight = window.loadSVG("res/pieces/bn.svg");
    
    Board::init(); // Bro copilot is CARRYING

    std::unordered_map<int, SDL_Texture *> pieceTexTable = {
        {Piece::Pawn | Piece::White, whitePawn},
        {Piece::King | Piece::White, whiteKing},
        {Piece::Queen | Piece::White, whiteQueen},
        {Piece::Rook | Piece::White, whiteRook},
        {Piece::Bishop | Piece::White, whiteBishop},
        {Piece::Knight | Piece::White, whiteKnight},

        {Piece::Pawn | Piece::Black, blackPawn},
        {Piece::King | Piece::Black, blackKing},
        {Piece::Queen | Piece::Black, blackQueen},
        {Piece::Rook | Piece::Black, blackRook},
        {Piece::Bishop | Piece::Black, blackBishop},
        {Piece::Knight | Piece::Black, blackKnight}
    };

    // Get the FPS
    float frameTime = 1000.0f / window.getFPS();
    bool running = true;
    int playerColor = Piece::White;
    int highlightedSquare = -1;

    Move prevMove (-1, -1, 0);

    std::vector<Move> moves = MoveGenerator::generateMoves(prevMove);
    while (running) {
        // Get the time (this is the ms since SDL init'd, it doesnt have to be specific or anything)
        Uint32 frameStart = SDL_GetTicks();
        // Time to render!!
        window.drawGrid();
        if (highlightedSquare > -1) {
            window.highlightSquare(highlightedSquare);
            for (Move &move : moves) {
                if (move.Start == highlightedSquare) {
                    window.highlightSquare(move.End);
                }
            }
        }
        

        // WE DO IT MAIN BABYY
        for (int i = 0; i < 64; i++) {
            if (Board::Squares[i] == Piece::Empty) continue;
            SDL_Rect pos = {i % 8 * 100, i / 8 * 100, 100, 100};
            int piece = Board::Squares[i];
            window.renderTex(pieceTexTable.at(piece), &pos);
        }
        
        window.render();
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
            if (event.type == SDL_MOUSEBUTTONUP) {
                if (Board::ColToMove != playerColor) continue;
                int x, y;
                SDL_GetMouseState(&x, &y);
                int square = (y / 100) * 8 + (x / 100);
                if (highlightedSquare == square) {
                    highlightedSquare = -1;
                } else {
                     if (highlightedSquare != -1) {
                        // Check if the move is valid
                        for (Move &move : moves) {
                            // Check if this is the move the user wants to make
                            if (move.Start == highlightedSquare && move.End == square) {

                                if (move.Flags & Move::Capture) {
                                    Mix_PlayChannel(-1, capture, 0);
                                }

                                Board::MakeMove(move);
                                playerColor = Board::ColToMove;
                                prevMove = move;
                                highlightedSquare = -1;
                                moves.clear();
                                moves = MoveGenerator::generateMoves(prevMove);
                                break;
                            }
                        }
                        if (highlightedSquare == -1) continue;
                    }
                    highlightedSquare = square;
                }
            }
        }

        // Get new time
        Uint32 offset = SDL_GetTicks() - frameStart;
        // Delay the next frame (if needed)
        if (offset < frameTime) SDL_Delay(frameTime - offset);
    }

    Mix_FreeChunk(capture);
    window.cleanup();
    SDL_Quit();

    return 0; // it went well!
}