#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

#include "lexer.h"

typedef struct value Value;

typedef struct {
    Token string;
    Value *value;
} Member;

typedef struct {
    size_t size, capacity;
    Member *items;
} Object;

typedef struct {
    size_t size, capacity;
    Value **items;
} Array;

typedef enum {
    VT_OBJECT,
    VT_ARRAY,
    VT_LITERAL,
} ValueType;

typedef union {
    Object object;
    Array array;
    Token literal; // STRING | NUMBER | 'true' | 'false' | 'null'
} AnyValue;

typedef struct value {
    ValueType type;
    AnyValue as;
} Value;

typedef struct {
    size_t pos;
    Token *tokens;
    char *content;
} Parser;

void parser_init(Parser *parser, Lexer *lexer);
TokenType cur_type(Parser *parser);
Token cur_token(Parser *parser);
Value *parse_literal(Parser *parser);
Value *parse_array(Parser *parser);
Value *parse_object(Parser *parser);
Value *parse_value(Parser *parser);
Value *parse_json(Parser *parser);
void print_value(Parser *parser, Value *value);
void print_member(Parser *parser, Member member);
void print_value(Parser *parser, Value *value);
void free_value(Value *value);

#endif
