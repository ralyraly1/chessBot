#include "board.h"
#include <stdexcept>

void validateEnPassant(const Position& currPosition){
    if (!currPosition.enPassantSquare.has_value()){
        return;
    }
    if (currPosition.sideToMove == Side::White){
        if (currPosition.enPassantSquare->rank != 2){
            throw std::invalid_argument("When White to move, en passant square must be on the 6th rank.");
        }
    }
    else if (currPosition.sideToMove == Side::Black){
        if (currPosition.enPassantSquare->rank != 5){
            throw std::invalid_argument("When Black to move, en passant square must be on the 3rd rank.");
        }
    }
}