#pragma once
#include "types.h"
#include <array>
#include <optional>

struct Position {
    std::array<std::array<Piece, FILES>, RANKS> board;
    Side sideToMove;
    std::uint8_t castleRights;
    std::optional<Square> enPassantSquare;
    std::uint8_t halfmoveCount;
    std::uint16_t fullmoveCount;
};