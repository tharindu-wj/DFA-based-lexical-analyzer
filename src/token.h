// Define the token types.

#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_DELIMITER,
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_ERROR
} TokenType;

#endif