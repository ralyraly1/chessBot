#pragma once
#include "types.h"
#include <array>
#include <string>

std::array<Piece, FILES> parseRank(std::string rankStr);
std::array<std::array<Piece, FILES>, RANKS> parseBoard(std::string boardStr);
Side parseSide(std::string sideStr);