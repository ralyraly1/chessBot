#include "types.h"
#include <iostream>
#include <string>

static int failures = 0;

static void expectEmpty(const std::string& name, const Piece& piece, bool expected) {
    bool got = piece.isEmpty();
    if (got != expected) {
        std::cout << "FAIL  " << name << ": expected isEmpty() = "
                  << (expected ? "true" : "false") << ", got "
                  << (got ? "true" : "false") << "\n";
        failures++;
    } else {
        std::cout << "ok    " << name << " -> "
                  << (got ? "empty" : "not empty") << "\n";
    }
}

int main() {
    std::cout << "--- consistent pieces ---\n";
    expectEmpty("empty square",
                Piece{PieceType::Empty, PieceColour::Empty}, true);
    expectEmpty("white pawn",
                Piece{PieceType::Pawn, PieceColour::White}, false);
    expectEmpty("black king",
                Piece{PieceType::King, PieceColour::Black}, false);

    std::cout << "\n" << (failures == 0 ? "all passed" : "some failed")
              << " (" << failures << " failures)\n";
    return failures == 0 ? 0 : 1;
}