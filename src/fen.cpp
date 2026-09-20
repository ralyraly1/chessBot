#include "types.h"
#include <array>
#include <string>
#include <stdexcept>
#include <cctype>
#include <iostream>

std::array<Piece, FILES> parseRank(std::string rankStr){
    std::array<Piece, FILES> rankArr;
    int currFile = 0;
    bool wasDigit = false;
    for(const char c : rankStr){
        Piece currPiece;
        if(c >= '0' && c <= '9'){
            if(!(c >= '1' && c <= '8')){
                throw std::invalid_argument(rankStr + ": Invalid digit - " + c);
            }
            if (wasDigit){
                throw std::invalid_argument(rankStr + ": Digits can not sit next to each other");
            }
            int digit = (int)(c - '0');
            if (currFile + digit > FILES){
                throw std::invalid_argument(rankStr + ": Given number of files is above 8");
            }
            currPiece = Piece{PieceType::Empty, PieceColour::Empty};
            for(int i = currFile; i < currFile + digit; i++){
                rankArr[i] = currPiece;
            }
            currFile += digit;
            wasDigit = true;
            continue;
        }
        if(currFile + 1 > FILES){
            throw std::invalid_argument(rankStr + ": Given number of files is above 8");
        }
        char cLower;
        if(std::isupper(c)){
            currPiece.pieceColour = PieceColour::White;
            cLower = std::tolower(c);
        }
        else{
            currPiece.pieceColour = PieceColour::Black;
            cLower = c;
        }
        switch (cLower){
            case 'p':
                currPiece.pieceType = PieceType::Pawn;
                break;
            case 'b':
                currPiece.pieceType = PieceType::Bishop;
                break;
            case 'n':
                currPiece.pieceType = PieceType::Knight;
                break;
            case 'r':
                currPiece.pieceType = PieceType::Rook;
                break;
            case 'q':
                currPiece.pieceType = PieceType::Queen;
                break;
            case 'k':
                currPiece.pieceType = PieceType::King;
                break;
            default:
                throw std::invalid_argument(rankStr + ": Invalid character - " + c);
        }
        rankArr[currFile] = currPiece;
        currFile++;
        wasDigit = false;
    }
    if (currFile != FILES){
        throw std::invalid_argument(rankStr + ": Given number of files is below 8");
    }
    return rankArr;
}

std::array<std::array<Piece, FILES>, RANKS> parseBoard(std::string boardStr){
    int currSlashes = 0;
    std::string rankStr;
    std::array<Piece, FILES> rankArr;
    std::array<std::array<Piece, FILES>, RANKS> boardArr;
    for(const char c : boardStr){
        if(currSlashes + 1 > RANKS){
            throw std::invalid_argument(boardStr + ": Too many ranks, expected 8 separated by 7 slashes");
        }
        if(c == '/'){
            rankArr = parseRank(rankStr);
            boardArr[currSlashes] = rankArr;
            rankStr = "";
            currSlashes++;
            continue;
        }
        rankStr += c;
    }
    if(currSlashes < RANKS - 1){
        throw std::invalid_argument(boardStr + ": Too few ranks, expected 8 separated by 7 slashes");
    }
    if(currSlashes > RANKS - 1){
        throw std::invalid_argument(boardStr + ": Too many ranks, expected 8 separated by 7 slashes");
    }
    rankArr = parseRank(rankStr);
    boardArr[currSlashes] = rankArr;
    return boardArr;
}

Side parseSide(std::string sideStr){
    if (sideStr.length() != 1){
        throw std::invalid_argument(sideStr + ": Invalid length of side to move argument");
    }
    char sideChar = sideStr[0];
    switch(sideChar){
        case 'w':
            return Side::White;
        case 'b':
            return Side::Black;
        default:
            throw std::invalid_argument(sideStr + ": Side to move must be a 'w' or a 'b' (case sensitive)");
    }
}

std::uint8_t parseCastleRights(std::string castleStr){
    int currChar = 0;
    std::uint8_t rights = 0b0000;
    if (castleStr == "-"){
        return 0;
    }
    if (castleStr == ""){
        throw std::invalid_argument(castleStr + ": Castling rights argument missing.");
    }
    for(const char c : castleStr){
        if (currChar + 1 > 4){
            throw std::invalid_argument(castleStr + ": Castling rights argument contains too many characters");
        }
        switch(c){
            case 'K':
                if ((rights & WHITE_KINGSIDE_CASTLE) != 0){
                    throw std::invalid_argument(castleStr + ": Castling rights argument contains duplicate character: " + c);
                }
                rights |= WHITE_KINGSIDE_CASTLE;
                break;
            case 'Q':
                if ((rights & WHITE_QUEENSIDE_CASTLE) != 0){
                    throw std::invalid_argument(castleStr + ": Castling rights argument contains duplicate character: " + c);
                }
                rights |= WHITE_QUEENSIDE_CASTLE;
                break;
            case 'k':
                if ((rights & BLACK_KINGSIDE_CASTLE) != 0){
                    throw std::invalid_argument(castleStr + ": Castling rights argument contains duplicate character: " + c);
                }
                rights |= BLACK_KINGSIDE_CASTLE;
                break;
            case 'q':
                if ((rights & BLACK_QUEENSIDE_CASTLE) != 0){
                    throw std::invalid_argument(castleStr + ": Castling rights argument contains duplicate character: " + c);
                }
                rights |= BLACK_QUEENSIDE_CASTLE;
                break;
            default:
                throw std::invalid_argument(castleStr + ": Castling rights argument contains invalid character: " + c);
        }
        currChar++;
    }
    return rights;
}