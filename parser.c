#include <stdio.h>

#include "vector.h"
#include "lexer.h"
#include "parser.h"

void parser_init(Parser *parser, Lexer *lexer)
{
    parser->pos = 0;
    parser->tokens = lexer->items;
    parser->content = lexer->content;
}

TokenType cur_type(Parser *parser)
{
    return parser->tokens[parser->pos].type;
}

Token cur_token(Parser *parser)
{
    return parser->tokens[parser->pos];
}

Value *parse_literal(Parser *parser)
{
    switch (cur_type(parser)) {
    case TT_STRING:
    case TT_NUMBER:
    case TT_TRUE:
    case TT_FALSE:
    case TT_NULL: {
        Value *value = malloc(sizeof(Value));
        value->type = VT_LITERAL;
        AnyValue as = {
            .literal = cur_token(parser),
        };
        value->as = as;
        parser->pos++;
        return value;
    }
    default:
        printf("Unexpected literal '");
        print_token(parser->content, cur_token(parser));
        printf("' at line %zu\n", cur_token(parser).line);
        exit(1);
    }
}

Value *parse_array(Parser *parser)
{
    if (cur_type(parser) == TT_LSQUARE) {
        Array array;
        v_init(array);
        parser->pos++;
        while (cur_type(parser) != TT_RSQUARE) {
            Value *value = parse_value(parser);
            v_append(array, value);
            if (cur_type(parser) == TT_COMMA) {
                parser->pos++;
            } else if (cur_type(parser) == TT_RSQUARE) {
            } else {
                printf("Expected ']' at line %zu\n",
                        cur_token(parser).line);
                exit(1);
            }
        }
        parser->pos++;

        Value *value = malloc(sizeof(Value));
        AnyValue as = {
            .array = array,
        };
        value->type = VT_ARRAY;
        value->as = as;
        return value;
    } else {
        return parse_literal(parser);
    }
}

Value *parse_object(Parser *parser)
{
    if (cur_type(parser) == TT_LBRACE) {
        parser->pos++;
        Object object;
        v_init(object);
        while (cur_type(parser) != TT_RBRACE) {

            Token string = cur_token(parser);
            parser->pos++;
            if (cur_type(parser) == TT_COLON) {
                parser->pos++;
            } else {
                printf("Expected ':' at line %zu\n",
                        cur_token(parser).line);
                exit(1);
            }
            Value *value = parse_value(parser);

            Member member = {
                .string = string,
                .value = value,
            };
            v_append(object, member);

            if (cur_type(parser) == TT_COMMA) {
                parser->pos++;
            } else if (cur_type(parser) == TT_RBRACE) {
            } else {
                printf("Expected '}' at line %zu\n",
                        cur_token(parser).line);
                exit(1);
            }
        }
        parser->pos++;

        Value *value = malloc(sizeof(Value));
        AnyValue as = {
            .object = object,
        };
        value->type = VT_OBJECT;
        value->as = as;
        return value;
    } else {
        return parse_array(parser);
    }
}

Value *parse_value(Parser *parser)
{
    return parse_object(parser);
}

Value *parse_json(Parser *parser)
{
    return parse_value(parser);
}

void print_member(Parser *parser, Member member)
{
    print_token(parser->content, member.string);
    printf(":");
    print_value(parser, member.value);
}

void print_value(Parser *parser, Value *value)
{
    switch (value->type) {

    case VT_OBJECT: {
        Object object = value->as.object;
        printf("{");
        if (object.size) {
            size_t limit = object.size - 1;
            for (size_t i = 0; i < limit; i ++) {
                print_member(parser, object.items[i]);
                printf(",");
            }
            print_member(parser, object.items[limit]);
        }
        printf("}");
        break;
    }

    case VT_ARRAY: {
        Array array = value->as.array;
        printf("[");
        if (array.size) {
            size_t limit = array.size - 1;
            for (size_t i = 0; i < limit; i ++) {
                print_value(parser, array.items[i]);
                printf(",");
            }
            print_value(parser, array.items[limit]);
        }
        printf("]");
        break;
    }

    case VT_LITERAL: {
        Token literal = value->as.literal;
        print_token(parser->content, literal);
        break;
    }

    }
}

void free_value(Value *value)
{
    switch (value->type) {

    case VT_OBJECT: {
        Object object = value->as.object;
        for (size_t i = 0; i < object.size; i++) {
            free_value(object.items[i].value);
        }
        free(object.items);
        break;
    }

    case VT_ARRAY: {
        Array array = value->as.array;
        for (size_t i = 0; i < array.size; i++) {
            free_value(array.items[i]);
        }
        free(array.items);
        break;
    }

    default:
        break;
    }
}
