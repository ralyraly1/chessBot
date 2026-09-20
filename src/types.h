#pragma once
#include <cassert>
#include <cstdint>

inline constexpr int RANKS = 8;
inline constexpr int FILES = 8;

inline constexpr std::uint8_t BLACK_QUEENSIDE_CASTLE = 0b0001;
inline constexpr std::uint8_t BLACK_KINGSIDE_CASTLE = 0b0010;
inline constexpr std::uint8_t WHITE_QUEENSIDE_CASTLE = 0b0100;
inline constexpr std::uint8_t WHITE_KINGSIDE_CASTLE = 0b1000;

enum class PieceType : std::uint8_t {Pawn, Bishop, Knight, Rook, Queen, King, Empty};
enum class PieceColour : std::uint8_t {White, Black, Empty};
enum class Side : std::uint8_t {White, Black};

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