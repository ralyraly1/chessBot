#include "types.h"
#include "fen.h"
#include <array>
#include <iostream>
#include <stdexcept>
#include <string>
#include <optional>

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

static std::string boardToString(const std::array<std::array<Piece, FILES>, RANKS>& board) {
    std::string s;
    for (int r = 0; r < RANKS; r++) {
        if (r > 0) s += '/';
        s += rankToString(board[r]);
    }
    return s;
}

static void expectBoard(const std::string& input, const std::string& expected) {
    try {
        std::array<std::array<Piece, FILES>, RANKS> board = parseBoard(input);
        std::string got = boardToString(board);
        bool clean = true;
        for (const auto& rank : board) {
            if (!emptySquaresAreClean(rank)) clean = false;
        }
        if (got != expected) {
            std::cout << "FAIL  \"" << input << "\": expected " << expected
                      << " got " << got << "\n";
            failures++;
        } else if (!clean) {
            std::cout << "FAIL  \"" << input << "\": a square has a mismatched type and colour\n";
            failures++;
        } else {
            std::cout << "ok    \"" << input << "\"\n";
        }
    } catch (const std::exception& e) {
        std::cout << "FAIL  \"" << input << "\": unexpected throw: "
                  << e.what() << "\n";
        failures++;
    }
}

static void expectBoardThrows(const std::string& input, const std::string& messagePart) {
    try {
        parseBoard(input);
        std::cout << "FAIL  \"" << input << "\": should have thrown\n";
        failures++;
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        if (msg.find(messagePart) == std::string::npos) {
            std::cout << "FAIL  \"" << input << "\": threw the wrong message: "
                      << msg << "\n";
            failures++;
        } else {
            std::cout << "ok    \"" << input << "\" threw: " << msg << "\n";
        }
    } catch (const std::exception& e) {
        std::cout << "FAIL  \"" << input << "\": wrong exception type: "
                  << e.what() << "\n";
        failures++;
    }
}

static const char* sideName(Side s) {
    return s == Side::White ? "White" : "Black";
}

static void expectSide(const std::string& input, Side expected) {
    try {
        Side got = parseSide(input);
        if (got != expected) {
            std::cout << "FAIL  \"" << input << "\": expected " << sideName(expected)
                      << " got " << sideName(got) << "\n";
            failures++;
        } else {
            std::cout << "ok    \"" << input << "\" -> " << sideName(got) << "\n";
        }
    } catch (const std::exception& e) {
        std::cout << "FAIL  \"" << input << "\": unexpected throw: "
                  << e.what() << "\n";
        failures++;
    }
}

static void expectSideThrows(const std::string& input, const std::string& messagePart) {
    try {
        parseSide(input);
        std::cout << "FAIL  \"" << input << "\": should have thrown\n";
        failures++;
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        if (msg.find(messagePart) == std::string::npos) {
            std::cout << "FAIL  \"" << input << "\": threw the wrong message: "
                      << msg << "\n";
            failures++;
        } else {
            std::cout << "ok    \"" << input << "\" threw: " << msg << "\n";
        }
    } catch (const std::exception& e) {
        std::cout << "FAIL  \"" << input << "\": wrong exception type: "
                  << e.what() << "\n";
        failures++;
    }
}

static std::string rightsToString(std::uint8_t r) {
    std::string s;
    s += (r & WHITE_KINGSIDE_CASTLE) != 0 ? 'K' : '-';
    s += (r & WHITE_QUEENSIDE_CASTLE) != 0 ? 'Q' : '-';
    s += (r & BLACK_KINGSIDE_CASTLE) != 0 ? 'k' : '-';
    s += (r & BLACK_QUEENSIDE_CASTLE) != 0 ? 'q' : '-';
    return s;
}

static void expectRights(const std::string& input, std::uint8_t expected) {
    try {
        std::uint8_t got = parseCastleRights(input);
        if (got != expected) {
            std::cout << "FAIL  \"" << input << "\": expected " << rightsToString(expected)
                      << " got " << rightsToString(got) << "\n";
            failures++;
        } else {
            std::cout << "ok    \"" << input << "\" -> " << rightsToString(got) << "\n";
        }
    } catch (const std::exception& e) {
        std::cout << "FAIL  \"" << input << "\": unexpected throw: "
                  << e.what() << "\n";
        failures++;
    }
}

static void expectRightsThrows(const std::string& input, const std::string& messagePart) {
    try {
        parseCastleRights(input);
        std::cout << "FAIL  \"" << input << "\": should have thrown\n";
        failures++;
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        if (msg.find(messagePart) == std::string::npos) {
            std::cout << "FAIL  \"" << input << "\": threw the wrong message: "
                      << msg << "\n";
            failures++;
        } else {
            std::cout << "ok    \"" << input << "\" threw: " << msg << "\n";
        }
    } catch (const std::exception& e) {
        std::cout << "FAIL  \"" << input << "\": wrong exception type: "
                  << e.what() << "\n";
        failures++;
    }
}

static void expectSquare(const std::string& input, int expectedFile, int expectedRank) {
    try {
        std::optional<Square> got = parseEnPassant(input);
        if (!got.has_value()) {
            std::cout << "FAIL  \"" << input << "\": expected a square but got none\n";
            failures++;
        } else if (got->file != expectedFile || got->rank != expectedRank) {
            std::cout << "FAIL  \"" << input << "\": expected file " << expectedFile
                      << " rank " << expectedRank
                      << " got file " << static_cast<int>(got->file)
                      << " rank " << static_cast<int>(got->rank) << "\n";
            failures++;
        } else {
            std::cout << "ok    \"" << input << "\" -> file " << static_cast<int>(got->file)
                      << " rank " << static_cast<int>(got->rank) << "\n";
        }
    } catch (const std::exception& e) {
        std::cout << "FAIL  \"" << input << "\": unexpected throw: "
                  << e.what() << "\n";
        failures++;
    }
}

static void expectNoSquare(const std::string& input) {
    try {
        std::optional<Square> got = parseEnPassant(input);
        if (got.has_value()) {
            std::cout << "FAIL  \"" << input << "\": expected no square but got one\n";
            failures++;
        } else {
            std::cout << "ok    \"" << input << "\" -> none\n";
        }
    } catch (const std::exception& e) {
        std::cout << "FAIL  \"" << input << "\": unexpected throw: "
                  << e.what() << "\n";
        failures++;
    }
}

static void expectEnPassantThrows(const std::string& input, const std::string& messagePart) {
    try {
        parseEnPassant(input);
        std::cout << "FAIL  \"" << input << "\": should have thrown\n";
        failures++;
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        if (msg.find(messagePart) == std::string::npos) {
            std::cout << "FAIL  \"" << input << "\": threw the wrong message: "
                      << msg << "\n";
            failures++;
        } else {
            std::cout << "ok    \"" << input << "\" threw: " << msg << "\n";
        }
    } catch (const std::exception& e) {
        std::cout << "FAIL  \"" << input << "\": wrong exception type: "
                  << e.what() << "\n";
        failures++;
    }
}

static void expectHalfmoveCount(const std::string& input, std::uint8_t expectedCount) {
    try {
        std::uint8_t got = parseHalfmoveCount(input);
        if (got != expectedCount) {
            std::cout << "FAIL  \"" << input << "\": expected count " << static_cast<int>(expectedCount)
                    << " got count " << static_cast<int>(got) << "\n";
            failures++;
        } else {
            std::cout << "ok    \"" << input << "\" -> count " << static_cast<int>(got) << "\n";
        }
    } catch (const std::exception& e) {
        std::cout << "FAIL  \"" << input << "\": unexpected throw: "
                << e.what() << "\n";
        failures++;
    }
}

static void expectHalfmoveCountThrows(const std::string& input, const std::string& messagePart) {
    try {
        parseHalfmoveCount(input);
        std::cout << "FAIL  \"" << input << "\": should have thrown\n";
        failures++;
    }
    catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        if (msg.find(messagePart) == std::string::npos) {
            std::cout << "FAIL  \"" << input << "\": threw the wrong message: "
                      << msg << "\n";
            failures++;
        } else {
            std::cout << "ok    \"" << input << "\" threw: " << msg << "\n";
        }
    }
    catch (const std::exception& e) {
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

    std::cout << "--- valid boards ---\n";
    expectBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR",
                "rnbqkbnr/pppppppp/......../......../......../......../PPPPPPPP/RNBQKBNR");
    expectBoard("8/8/8/8/8/8/8/8",
                "......../......../......../......../......../......../......../........");
    expectBoard("k7/8/8/8/8/8/8/7K",
                "k......./......../......../......../......../......../......../.......K");
    expectBoard("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R",
                "r.bqkb.r/pppp.ppp/..n..n../....p.../..B.P.../.....N../PPPP.PPP/RNBQK..R");

    std::cout << "--- too few ranks ---\n";
    expectBoardThrows("", "Too few ranks");
    expectBoardThrows("8", "Too few ranks");
    expectBoardThrows("8/8/8/8/8/8/8", "Too few ranks");

    std::cout << "--- too many ranks ---\n";
    expectBoardThrows("8/8/8/8/8/8/8/8/8", "Too many ranks");
    expectBoardThrows("8/8/8/8/8/8/8/8/", "Too many ranks");
    expectBoardThrows("8/8/8/8/8/8/8/8//", "Too many ranks");

    std::cout << "--- empty ranks ---\n";
    expectBoardThrows("/8/8/8/8/8/8/8", "files is below 8");
    expectBoardThrows("8//8/8/8/8/8/8", "files is below 8");
    expectBoardThrows("8/8/8/8/8/8/8/", "files is below 8");

    std::cout << "--- bad rank contents ---\n";
    expectBoardThrows("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNX", "Invalid character");
    expectBoardThrows("rnbqkbnrr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", "files is above 8");
    expectBoardThrows("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBN", "files is below 8");
    expectBoardThrows("rnbqkbnr/pppppppp/44/8/8/8/PPPPPPPP/RNBQKBNR", "next to each other");
    expectBoardThrows("rnbqkbnr/pppppppp/9/8/8/8/PPPPPPPP/RNBQKBNR", "Invalid digit");

    std::cout << "--- valid side to move ---\n";
    expectSide("w", Side::White);
    expectSide("b", Side::Black);

    std::cout << "--- bad length ---\n";
    expectSideThrows("", "Invalid length");
    expectSideThrows("wb", "Invalid length");
    expectSideThrows("w ", "Invalid length");
    expectSideThrows(" w", "Invalid length");
    expectSideThrows("white", "Invalid length");

    std::cout << "--- wrong case ---\n";
    expectSideThrows("W", "must be a 'w' or a 'b'");
    expectSideThrows("B", "must be a 'w' or a 'b'");

    std::cout << "--- invalid characters ---\n";
    expectSideThrows("x", "must be a 'w' or a 'b'");
    expectSideThrows("1", "must be a 'w' or a 'b'");
    expectSideThrows("-", "must be a 'w' or a 'b'");
    expectSideThrows("/", "must be a 'w' or a 'b'");

        std::cout << "--- valid castling rights ---\n";
    expectRights("-", 0);
    expectRights("K", WHITE_KINGSIDE_CASTLE);
    expectRights("Q", WHITE_QUEENSIDE_CASTLE);
    expectRights("k", BLACK_KINGSIDE_CASTLE);
    expectRights("q", BLACK_QUEENSIDE_CASTLE);
    expectRights("KQ", WHITE_KINGSIDE_CASTLE | WHITE_QUEENSIDE_CASTLE);
    expectRights("kq", BLACK_KINGSIDE_CASTLE | BLACK_QUEENSIDE_CASTLE);
    expectRights("Kq", WHITE_KINGSIDE_CASTLE | BLACK_QUEENSIDE_CASTLE);
    expectRights("KQkq", WHITE_KINGSIDE_CASTLE | WHITE_QUEENSIDE_CASTLE | BLACK_KINGSIDE_CASTLE | BLACK_QUEENSIDE_CASTLE);
    expectRights("qkQK", WHITE_KINGSIDE_CASTLE | WHITE_QUEENSIDE_CASTLE | BLACK_KINGSIDE_CASTLE | BLACK_QUEENSIDE_CASTLE);

    std::cout << "--- empty ---\n";
    expectRightsThrows("", "argument missing");

    std::cout << "--- duplicates ---\n";
    expectRightsThrows("KK", "duplicate character");
    expectRightsThrows("KQK", "duplicate character");
    expectRightsThrows("KKKK", "duplicate character");
    expectRightsThrows("kqkq", "duplicate character");

    std::cout << "--- too many characters ---\n";
    expectRightsThrows("KQkqK", "too many characters");
    expectRightsThrows("KQkqkq", "too many characters");

    std::cout << "--- invalid characters ---\n";
    expectRightsThrows("x", "invalid character");
    expectRightsThrows("-K", "invalid character");
    expectRightsThrows("K-", "invalid character");
    expectRightsThrows("--", "invalid character");
    expectRightsThrows(" K", "invalid character");
    expectRightsThrows("KQx", "invalid character");
    expectRightsThrows("1", "invalid character");

    std::cout << "--- valid en passant squares ---\n";
    expectNoSquare("-");
    expectSquare("e3", 4, 5);
    expectSquare("a6", 0, 2);
    expectSquare("h3", 7, 5);
    expectSquare("a8", 0, 0);
    expectSquare("e4", 4, 4);
    expectSquare("a1", 0, 7);
    expectSquare("h8", 7, 0);
    expectSquare("h1", 7, 7);


    std::cout << "--- invalid en passant ---\n";
    expectEnPassantThrows("", "argument missing");
    expectEnPassantThrows("i3", "file must be a letter");
    expectEnPassantThrows("e", "length invalid");
    expectEnPassantThrows("e33", "length invalid");
    expectEnPassantThrows("e3 ", "length invalid");
    expectEnPassantThrows("E3", "file must be a letter");
    expectEnPassantThrows("3e", "file must be a letter");
    expectEnPassantThrows("e9", "rank must be a digit");
    expectEnPassantThrows("e0", "rank must be a digit");
    expectEnPassantThrows("ea", "rank must be a digit");

    std::cout << "--- valid halfmove counts ---\n";
    expectHalfmoveCount("0", 0);
    expectHalfmoveCount("7", 7);
    expectHalfmoveCount("100", 100);
    expectHalfmoveCount("150", 150);


    std::cout << "--- invalid halfmove count ---\n";
    expectHalfmoveCountThrows("", "argument missing");
    expectHalfmoveCountThrows("-5", "invalid character");
    expectHalfmoveCountThrows("+3", "invalid character");
    expectHalfmoveCountThrows("5x", "invalid character");
    expectHalfmoveCountThrows(" 5", "invalid character");
    expectHalfmoveCountThrows("00", "leading 0");
    expectHalfmoveCountThrows("007", "leading 0");
    expectHalfmoveCountThrows("256", "exceed 255");
    expectHalfmoveCountThrows("99999999999999", "too large");


    std::cout << "\n" << (failures == 0 ? "all passed" : "some failed")
              << " (" << failures << " failures)\n";
    return failures == 0 ? 0 : 1;
}