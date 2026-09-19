#pragma once
#include <cassert>

inline constexpr int RANKS = 8;
inline constexpr int FILES = 8;

enum class PieceType {Pawn, Bishop, Knight, Rook, Queen, King, None};
enum class PieceColour {White, Black, None};

struct Piece {
    PieceType pieceType;
    PieceColour pieceColour;

    bool isEmpty() const{
        bool isEmptyPiece = (pieceType == PieceType::None);
        bool isEmptyColour = (pieceColour == PieceColour::None);
        assert(isEmptyPiece == isEmptyColour);
        return isEmptyPiece && isEmptyColour;
    }
};