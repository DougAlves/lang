#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef float real32;
typedef int number32;
typedef size_t usize32;

enum Token_Type { idenfityer, f32, i32 };
struct Token {
  usize32 line;
  usize32 col;
  usize32 token_size;
  char *code;
  Token_Type type;
  union {
    real32 real;
    number32 number;
  };
};

struct Lexer {
    char *input;
    usize32 input_size;
    usize32 cursor;
    usize32 peak;
    usize32 line;
};

bool is_alfa(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool is_num(char c) {
    return (c >= '0' && c <= '9');
}

bool is_whitespace(char c) {
    return c == ' '
        || c == '\n'
        || c == '\t'
        || c == '\v'
        || c == '\f'
        || c == '\r';
}

void Telos_Lexer_advance(Lexer* lex) {
    if (lex->cursor < lex->input_size) {

        if (lex->input[lex->cursor] == '\n') {
            lex->line++;
        }
        lex->cursor++;
        lex->peak = lex->cursor + 1;
    }
}

void Telos_eat_whitespace(Lexer* lex) {
    while(is_whitespace(lex->input[lex->cursor])) {
        Telos_Lexer_advance(lex);
    }
}

Token Telos_eat_i32(Lexer *lex) {
    usize32 init = lex->cursor;
    while (is_num(lex->input[lex->cursor])) {
        Telos_Lexer_advance(lex);
    }

    usize32 size = lex->cursor - init;
    char* code = (char*) calloc(size, sizeof(char));
    if (!code) {
        exit(2);
    }

    errno_t err = memcpy_s(code, size, lex->input + init, size);
    code[size] = '\0';
    return Token{
        .line = lex->line,
        .col = init,
        .token_size = size,
        .code = code,
        .type = i32,
        .number = atoi(code),
    };
}

Token Telos_eat_identifier(Lexer *lex) {
    usize32 init = lex->cursor;
    while (is_alfa(lex->input[lex->cursor])) {
        Telos_Lexer_advance(lex);
    }
    usize32 size = lex->cursor - init;
    char* code = (char*) malloc(sizeof(char) * (size + 1));
    errno_t err = memcpy_s(code, size + 1, lex->input + init,
              size + 1);

    code[size] = '\0';
    return Token{
        .line = lex->line,
        .col = init,
        .token_size = lex->cursor - init,
        .code = code,
        .type = idenfityer,
    };
}

Lexer Telos_Lexer_init(char *input, usize32 size) {
    return Lexer{.input = input, .input_size = size, .cursor = 0, .peak = 1, .line = 1};
}

int main(int argc, char *argv[]) {
    char input[] = "  32 \n  oi     iiii aaee     input     ola";
    Lexer lex = Telos_Lexer_init(input, strlen(input));
    while(lex.cursor < lex.input_size) {
        Telos_eat_whitespace(&lex);
        if (is_alfa(lex.input[lex.cursor])) {
            Token id = Telos_eat_identifier(&lex);
            printf("read token '%s' (%zu:%zu)\n", id.code, id.line, id.col);
            printf("'%s' (%zu:%zu)\n", id.code, id.line, id.col);
        } else if (is_num(lex.input[lex.cursor]) || (lex.input[lex.cursor] == '-' && is_num(lex.input[lex.peak]))) {
            Token num = Telos_eat_i32(&lex);
            printf("read token '%s', (%zu:%zu)\n", num.code, num.line, num.col);
            printf("'%d' (%zu:%zu)\n", num.number, num.line, num.col);
        }
        Telos_Lexer_advance(&lex);
    }
    return 0;
}
