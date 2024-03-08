#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "vector.h"
#include "lexer.h"

void read_content(Buffer *buffer, FILE *f)
{
    char c;
    do {
        c = fgetc(f);
        v_append(*buffer, c);
    } while (c != EOF);
}

char cur_char(Lexer *lexer)
{
    return lexer->content[lexer->pos];
}

void lex_string(Lexer *lexer)
{
    lexer->pos++;
    char c;
    while ((c = cur_char(lexer)) != '"') {
        if (c == EOF) {
            printf("Unexpected EOF at line %zu\n", lexer->line);
            exit(1);
        }

        if (c == '\\') {
            lexer->pos += 2;
        } else {
            lexer->pos += 1;
        }
    }
}

bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

void lex_digits(Lexer *lexer)
{
    while (is_digit(cur_char(lexer))) {
        lexer->pos++;
    }
}

void lex_number(Lexer *lexer)
{
    if (cur_char(lexer) == '-') {
        lexer->pos++;
    }

    if (cur_char(lexer) == '0') {
        lexer->pos++;
    } else if (is_digit(cur_char(lexer))) {
        lex_digits(lexer);
    } else {
        printf("Expected digit at line %zu\n", lexer->line);
        exit(1);
    }

    if (cur_char(lexer) == '.') {
        lexer->pos++;
        lex_digits(lexer);
    }

    if (cur_char(lexer) == 'e' || cur_char(lexer) == 'E') {
        lexer->pos++;
        if (cur_char(lexer) == '-' || cur_char(lexer) == '+') {
            lexer->pos++;
        }
        lex_digits(lexer);
    }

    lexer->pos--;
}

#define cmp(_string, _type) \
    if (strncmp(_string, lexer->content + lexer->pos, sizeof(_string) - 1) == 0) { \
        token.type = _type; \
        lexer->pos += sizeof(_string) - 2; \
    } else { \
        printf("Unexpected token at line %zu\n", lexer->line); \
        exit(1); \
    }

bool lex_token(Lexer *lexer)
{
    Token token = {
        .start = lexer->pos,
        .end = lexer->pos,
        .line = lexer->line,
    };

    switch (cur_char(lexer)) {

    // Braces
    case '{':
        token.type = TT_LBRACE;
        break;
    case '}':
        token.type = TT_RBRACE;
        break;

    // Square brackets
    case '[':
        token.type = TT_LSQUARE;
        break;
    case ']':
        token.type = TT_RSQUARE;
        break;

    // String
    case '"':
        lex_string(lexer);
        token.end = lexer->pos;
        token.type = TT_STRING;
        break;

    // true / false / null
    case 't':
        cmp("true", TT_TRUE);
        break;
    case 'f':
        cmp("false", TT_FALSE);
        break;
    case 'n':
        cmp("null", TT_NULL);
        break;

    // Punctuation
    case ',':
        token.type = TT_COMMA;
        break;
    case ':':
        token.type = TT_COLON;
        break;

    // Whitespace
    case ' ':
    case '\t':
    case '\r':
    case '\f':
        return true;
    case '\n':
        lexer->line++;
        return true;

    // EOF
    case EOF:
        token.type = TT_EOF;
        v_append(*lexer, token);
        return false;

    default:
        if (is_digit(cur_char(lexer))
                || cur_char(lexer) == '-') {
            lex_number(lexer);
            token.end = lexer->pos;
            token.type = TT_NUMBER;
        } else {
            printf("Unexpected token '%c' at line %zu\n",
                    cur_char(lexer),
                    lexer->line);
            exit(1);
        }
    }

    v_append(*lexer, token);
    return true;
}

#undef cmp

void lex(Lexer *lexer)
{
    while (lex_token(lexer)) {
        lexer->pos++;
    }
}

void lexer_init(Lexer *lexer, char *content)
{
    v_init(*lexer);
    lexer->pos = 0;
    lexer->line = 1;
    lexer->content = content;
}

void print_token(char *content, const Token token)
{
    switch (token.type) {
    case TT_LBRACE: printf("{"); break;
    case TT_RBRACE: printf("}"); break;
    case TT_LSQUARE: printf("["); break;
    case TT_RSQUARE: printf("]"); break;
    case TT_STRING:
    case TT_NUMBER: {
        char *start = content + token.start;
        int size = token.end - token.start + 1;
        printf("%.*s", size, start);
        break;
    }
    case TT_TRUE: printf("true"); break;
    case TT_FALSE: printf("false"); break;
    case TT_NULL: printf("null"); break;
    case TT_COMMA: printf(","); break;
    case TT_COLON: printf(":"); break;
    case TT_EOF: printf("EOF"); break;
    }
}

void print_tokens(const Lexer *lexer)
{
    for (size_t i = 0; i < lexer->size; i++) {
        print_token(lexer->content, lexer->items[i]);
    }
}
