#include <stdio.h>
#include <assert.h>

#include "../vector.h"
#include "../lexer.h"
#include "../parser.h"

void test(char *test_file)
{
    FILE *f = fopen(test_file, "r");
    if (f == NULL) {
        fprintf(stderr, "Cannot open file '%s'\n", test_file);
        exit(1);
    }
    assert(f != NULL && "Cannot open test file");

    Buffer buffer;
    v_init(buffer);
    read_content(&buffer, f);

    Lexer lexer = {0};
    lexer_init(&lexer, buffer.items);
    lex(&lexer);

    Parser parser = {0};
    parser_init(&parser, &lexer);
    Value *value = parse_json(&parser);

    assert(value != NULL);

    printf("Completed %s!\n", test_file);
}

int main()
{
    test("test_1.json");
    test("test_2.json");
    test("test_3.json");
    test("test_4.json");
    test("test_5.json");
    test("test_6.json");
    test("test_7.json");
}
