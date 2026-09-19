#pragma once
#include <cassert>

enum class PieceType {Pawn, Bishop, Knight, Rook, Queen, King, EmptyPiece};
enum class PieceColour {White, Black, EmptyColour};

struct Piece {
    PieceType pieceType;
    PieceColour pieceColour;

    bool isEmpty() const{
        bool isEmptyPiece = (pieceType == PieceType::EmptyPiece);
        bool isEmptyColour = (pieceColour == PieceColour::EmptyColour);
        assert(isEmptyPiece == isEmptyColour);
        return isEmptyPiece && isEmptyColour;
    }
};