#include "board.h"
#include "boardstate.h"
#include "types.h"
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

static int failures = 0;

static Position makePosition(Side side, std::optional<Square> enPassantSquare) {
    Position pos{};
    pos.sideToMove = side;
    pos.enPassantSquare = enPassantSquare;
    return pos;
}

static void expectValidEnPassant(const std::string& label, const Position& pos) {
    try {
        validateEnPassant(pos);
        std::cout << "ok    \"" << label << "\" -> valid\n";
    } catch (const std::exception& e) {
        std::cout << "FAIL  \"" << label << "\": unexpected throw: "
                  << e.what() << "\n";
        failures++;
    }
}

static void expectValidEnPassantThrows(const std::string& label, const Position& pos, const std::string& messagePart) {
    try {
        validateEnPassant(pos);
        std::cout << "FAIL  \"" << label << "\": should have thrown\n";
        failures++;
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        if (msg.find(messagePart) == std::string::npos) {
            std::cout << "FAIL  \"" << label << "\": threw the wrong message: "
                      << msg << "\n";
            failures++;
        } else {
            std::cout << "ok    \"" << label << "\" threw: " << msg << "\n";
        }
    } catch (const std::exception& e) {
        std::cout << "FAIL  \"" << label << "\": wrong exception type: "
                  << e.what() << "\n";
        failures++;
    }
}

int main() {
    std::cout << "--- no en passant square set ---\n";
    expectValidEnPassant("white to move, no ep", makePosition(Side::White, std::nullopt));
    expectValidEnPassant("black to move, no ep", makePosition(Side::Black, std::nullopt));

    std::cout << "--- white to move ---\n";
    expectValidEnPassant("white to move, ep on rank 6 (e6)", makePosition(Side::White, Square{2, 4}));
    expectValidEnPassantThrows("white to move, ep on rank 3 (e3)", makePosition(Side::White, Square{5, 4}), "6th rank");
    expectValidEnPassantThrows("white to move, ep on rank 8", makePosition(Side::White, Square{0, 4}), "6th rank");
    expectValidEnPassantThrows("white to move, ep on rank 1", makePosition(Side::White, Square{7, 4}), "6th rank");

    std::cout << "--- black to move ---\n";
    expectValidEnPassant("black to move, ep on rank 3 (e3)", makePosition(Side::Black, Square{5, 4}));
    expectValidEnPassantThrows("black to move, ep on rank 6 (e6)", makePosition(Side::Black, Square{2, 4}), "3rd rank");
    expectValidEnPassantThrows("black to move, ep on rank 8", makePosition(Side::Black, Square{0, 4}), "3rd rank");
    expectValidEnPassantThrows("black to move, ep on rank 1", makePosition(Side::Black, Square{7, 4}), "3rd rank");

    std::cout << "\n" << (failures == 0 ? "all passed" : "some failed")
              << " (" << failures << " failures)\n";
    return failures == 0 ? 0 : 1;
}