#pragma once
#include <cassert>
#include <cstdint>

inline constexpr int RANKS = 8;
inline constexpr int FILES = 8;

enum class PieceType : std::uint8_t {Pawn, Bishop, Knight, Rook, Queen, King, Empty};
enum class PieceColour : std::uint8_t {White, Black, Empty};

struct Piece {
    PieceType pieceType = PieceType::Empty;
    PieceColour pieceColour = PieceColour::Empty;

    bool isEmpty() const{
        bool isEmptyPiece = (pieceType == PieceType::Empty);
        bool isEmptyColour = (pieceColour == PieceColour::Empty);
        assert(isEmptyPiece == isEmptyColour);
        return isEmptyPiece && isEmptyColour;
    }
};

static_assert(sizeof(Piece) == 2);