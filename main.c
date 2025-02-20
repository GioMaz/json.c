#include <stdio.h>
#include <sys/stat.h>

#include "vector.h"
#include "lexer.h"
#include "parser.h"

static int is_dir(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file.json>\n", argv[0]);
        exit(1);
    }

    if (is_dir(argv[1])) {
        fprintf(stderr, "%s: %s: Is a directory\n", argv[0], argv[1]);
        exit(1);
    }

    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[1]);
        exit(1);
    }

    Buffer buffer;
    v_init(buffer);
    read_content(&buffer, f);

    Lexer lexer = {0};
    lexer_init(&lexer, buffer.items);
    lex(&lexer);

    Parser parser = {0};
    parser_init(&parser, &lexer);
    Value *value = parse_json(&parser);
    print_value(&parser, value);
    printf("\n");

    free_value(value);

    return 0;
}
