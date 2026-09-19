#include "types.h"
#include <array>
#include <string>
#include <stdexcept>
#include <cctype>

std::array<Piece, FILES> parseRank(std::string rankStr){
    std::array<Piece, FILES> rankArr;
    int currFile = 0;
    bool wasDigit = false;
    for(const char c :rankStr){
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