#include "types.h"
#include <cassert>

int main() {
    Piece empty{PieceType::EmptyPiece, PieceColour::EmptyColour};
    Piece whitePawn{PieceType::Pawn, PieceColour::White};
    Piece noColourButPiece{PieceType::Pawn, PieceColour::EmptyColour};
    Piece noPieceButColour{PieceType::EmptyPiece, PieceColour::White};

    assert(empty.isEmpty());
    assert(!whitePawn.isEmpty());
    assert(noColourButPiece.isEmpty());
    assert(noPieceButColour.isEmpty());
}