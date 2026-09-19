#include "types.h"
#include "fen.h"
#include <array>
#include <iostream>
#include <stdexcept>
#include <string>

static int failures = 0;

static char pieceToChar(const Piece& p) {
    char letter = '?';
    switch (p.pieceType) {
        case PieceType::Pawn:   letter = 'p'; break;
        case PieceType::Bishop: letter = 'b'; break;
        case PieceType::Knight: letter = 'n'; break;
        case PieceType::Rook:   letter = 'r'; break;
        case PieceType::Queen:  letter = 'q'; break;
        case PieceType::King:   letter = 'k'; break;
        case PieceType::Empty:  return '.';
    }
    if (p.pieceColour == PieceColour::White) {
        letter = static_cast<char>(letter - 'a' + 'A');
    }
    return letter;
}

static std::string rankToString(const std::array<Piece, FILES>& rank) {
    std::string s;
    for (const Piece& p : rank) s += pieceToChar(p);
    return s;
}

static bool emptySquaresAreClean(const std::array<Piece, FILES>& rank) {
    for (const Piece& p : rank) {
        if (p.pieceType == PieceType::Empty && p.pieceColour != PieceColour::Empty) {
            return false;
        }
        if (p.pieceType != PieceType::Empty && p.pieceColour == PieceColour::Empty) {
            return false;
        }
    }
    return true;
}

static void expectRank(const std::string& input, const std::string& expected) {
    try {
        std::array<Piece, FILES> rank = parseRank(input);
        std::string got = rankToString(rank);
        if (got != expected) {
            std::cout << "FAIL  \"" << input << "\": expected " << expected
                      << " got " << got << "\n";
            failures++;
        } else if (!emptySquaresAreClean(rank)) {
            std::cout << "FAIL  \"" << input << "\": a square has a mismatched type and colour\n";
            failures++;
        } else {
            std::cout << "ok    \"" << input << "\" -> " << got << "\n";
        }
    } catch (const std::exception& e) {
        std::cout << "FAIL  \"" << input << "\": unexpected throw: "
                  << e.what() << "\n";
        failures++;
    }
}

static void expectThrows(const std::string& input) {
    try {
        parseRank(input);
        std::cout << "FAIL  \"" << input << "\": should have thrown\n";
        failures++;
    } catch (const std::invalid_argument& e) {
        std::cout << "ok    \"" << input << "\" threw: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cout << "FAIL  \"" << input << "\": wrong exception type: "
                  << e.what() << "\n";
        failures++;
    }
}

int main() {
    std::cout << "--- valid ranks ---\n";
    expectRank("rnbqkbnr", "rnbqkbnr");
    expectRank("RNBQKBNR", "RNBQKBNR");
    expectRank("pppppppp", "pppppppp");
    expectRank("PPPPPPPP", "PPPPPPPP");
    expectRank("8",        "........");
    expectRank("4P3",      "....P...");
    expectRank("p3p3",     "p...p...");
    expectRank("P7",       "P.......");
    expectRank("7P",       ".......P");
    expectRank("1k6",      ".k......");
    expectRank("3q1r1b",   "...q.r.b");
    expectRank("1p1P1n1N", ".p.P.n.N");

    std::cout << "--- too short ---\n";
    expectThrows("");
    expectThrows("7");
    expectThrows("rnbqkbn");
    expectThrows("4P2");

    std::cout << "--- too long ---\n";
    expectThrows("rnbqkbnrr");
    expectThrows("88");
    expectThrows("P8");
    expectThrows("4P4");
    expectThrows("8p");

    std::cout << "--- bad digits ---\n";
    expectThrows("0");
    expectThrows("9");
    expectThrows("rnbqkbn0");

    std::cout << "--- adjacent digits ---\n";
    expectThrows("44");
    expectThrows("43");
    expectThrows("P44");
    expectThrows("1111");
    expectThrows("1p33");

    std::cout << "--- invalid characters ---\n";
    expectThrows("rnbqkbnx");
    expectThrows("rnbqkbn/");
    expectThrows("rnbqkbn ");
    expectThrows("rnbqkbnX");

    std::cout << "\n" << (failures == 0 ? "all passed" : "some failed")
              << " (" << failures << " failures)\n";
    return failures == 0 ? 0 : 1;
}