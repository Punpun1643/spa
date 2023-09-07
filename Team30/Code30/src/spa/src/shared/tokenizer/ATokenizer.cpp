#include "ATokenizer.h"
#include "token/IntegerToken.h"
#include "token/WordToken.h"
#include "token/SpecialCharToken.h"

ATokenizer::ATokenizer(std::istream &input) : input(input)
{
}

std::shared_ptr<Token> ATokenizer::next()
{
    char c;
    while (input.get(c)) {
        if (std::isalpha(c)) {
            std::string word;
            word += c;
            while (input.get(c) && (std::isalpha(c))) {
                word += c;
            }
            return std::make_shared<WordToken>(word);
        }
        else if (std::isdigit(c)) {
            std::string number;
            number += c;
            while (input.get(c) && std::isdigit(c)) {
                number += c;
            }
            return std::make_shared<IntegerToken>(number);
        }
        else if (std::isspace(c)) {
            continue; // Skip whitespace
        }
        else {
            return std::make_shared<SpecialCharToken>(std::string(1, c));
        }
    }
}

//std::vector<std::shared_ptr<Token>> ATokenizer::tokenize()
//{
//}