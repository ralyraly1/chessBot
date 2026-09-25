#pragma once
#include "types.h"
#include <array>
#include <string>
#include <optional>

std::array<Piece, FILES> parseRank(std::string rankStr);
std::array<std::array<Piece, FILES>, RANKS> parseBoard(std::string boardStr);
Side parseSide(std::string sideStr);
std::uint8_t parseCastleRights(std::string castleStr);
std::optional<Square> parseEnPassant(std::string enPStr);
std::uint8_t parseHalfmoveCount(std::string halfmoveStr);
std::uint16_t parseFullmoveCount(std::string fullmoveStr);