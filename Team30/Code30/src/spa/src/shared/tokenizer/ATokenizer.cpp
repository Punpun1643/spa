#include "ATokenizer.h"
#include "token/IntegerToken.h"
#include "token/WordToken.h"
#include "token/SpecialCharToken.h"

ATokenizer::ATokenizer(std::istream &input) : input(input)
{
}

std::vector<std::shared_ptr<Token>> ATokenizer::tokenize()
{
    std::vector<std::shared_ptr<Token>> tokens;
    while (hasNext()) {
        std::shared_ptr<Token> token = next();
        tokens.push_back(token);
    }

    return tokens;
}

std::shared_ptr<Token> ATokenizer::next()
{
    char c;

    while (input.get(c)) {
        if (std::isalpha(c)) {
            std::string word;
            word += c;
            while (std::isalpha(input.peek())) {
                input.get(c);
                word += c;
            }
            return std::make_shared<WordToken>(word);
        }

        else if (std::isdigit(c)) {
            std::string number;
            number += c;
            while (std::isdigit(input.peek())) {
                input.get(c);
                number += c;
            }
            return std::make_shared<IntegerToken>(number);
        }

        else if (c == '{' || c == '}' || c == '(' || c == ')' || c == '+' ||
                 c == '-' || c == '/' || c == '*' || c == '%' || c == ';') {
            return std::make_shared<SpecialCharToken>(std::string(1, c));
        }

        else if (c == '&' || c == '|' || c == '!' || c == '=' || c == '>' || c == '<') {
            return handleSpecialChar(c);
        }

        else if (std::isspace(c)) {
            continue;
        }

        else {
            throw std::invalid_argument("Invalid Syntax");
        }
    }
}

std::shared_ptr<Token> ATokenizer::handleSpecialChar(char c)
{
    char next_c;
    input.get(next_c);

    if (c == '|' && next_c == '|') {
        return std::make_shared<SpecialCharToken>("||");
    }

    else if (c == '&' && next_c == '&') {
        return std::make_shared<SpecialCharToken>("&&");
    }

    else if (c == '!' || c == '=' || c == '>' || c == '<') {
        if (next_c == '=') {
            return std::make_shared<SpecialCharToken>(std::string(1, c) +
                                                      std::string(1, next_c));
        }
        return std::make_shared<SpecialCharToken>(std::string(1, c));
    }

    else {
        throw std::invalid_argument("Invalid Syntax");
    }
}

bool ATokenizer::hasNext()
{
    char c;
    while (std::isspace(input.peek())) {
        input.get(c);
    }
    return input.peek() != EOF;
}