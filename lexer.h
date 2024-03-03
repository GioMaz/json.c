#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>

typedef enum {
    TT_LBRACE,
    TT_RBRACE,
    TT_LSQUARE,
    TT_RSQUARE,
    TT_STRING,
    TT_NUMBER,
    TT_TRUE,
    TT_FALSE,
    TT_NULL,
    TT_COMMA,
    TT_COLON,
    TT_EOF,
} TokenType;

typedef struct {
    size_t start;
    size_t end;
    size_t line;
    TokenType type;
} Token;

typedef struct {
    size_t size;
    size_t capacity;
    Token *items;
    size_t pos;
    size_t line;
    char *content;
} Lexer;

typedef struct {
    size_t size, capacity;
    char *items;
} Buffer;

void read_content(Buffer *buffer, FILE *f);
char cur_char(Lexer *lexer);
void lex_string(Lexer *lexer);
bool is_digit(char c);
void lex_digits(Lexer *lexer);
void lex_number(Lexer *lexer);
bool lex_token(Lexer *lexer);
void lex(Lexer *lexer);
void lexer_init(Lexer *lexer, char *content);
void print_token(char *content, Token token);
void print_tokens(Lexer *lexer);

#endif
