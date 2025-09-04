#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

typedef float real32;
typedef int number32;
typedef size_t usize32;

enum Token_Type {
  IDENTIFIER,
  F32,
  I32,
  OP_PLUS,
  OP_MINUS,
  OP_MULT,
  OP_DIV,
  OP_BT_OR,
  OP_BT_AND,
  OP_BT_XOR,
  OP_NOT,
  OP_OR,
  OP_AND,
  OP_ASS, // :=
  OP_EQ,  // ==
  OP_GT,  // >
  OP_LT,  // <
  OP_LTE, // <=
  OP_GTE, // >=
  OP_SL,  // >>
  OP_SR,  // <<
};

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

std::string Token_Name(Token t) {
    char buffer[100];
  switch (t.type) {
  case IDENTIFIER: {
    sprintf(buffer, "i(%s)", t.code);
    return buffer;
  }
  case F32:
    sprintf(buffer, "f(%f)", t.real);
    return buffer;
  case I32:
    sprintf(buffer, "f(%d)", t.number);
    return buffer;
  case OP_PLUS:
    sprintf(buffer, "op(+)");
    return buffer;
  case OP_MINUS:
    sprintf(buffer, "op(-)");
    return buffer;
  case OP_MULT:
    sprintf(buffer, "op(*)");
    return buffer;
  case OP_DIV:
    sprintf(buffer, "op(/)");
    return buffer;
  case OP_BT_OR:
    sprintf(buffer, "op(|)");
    return buffer;
  case OP_BT_AND:
    sprintf(buffer, "op(&)");
    return buffer;
  case OP_BT_XOR:
    sprintf(buffer, "op(^)");
    return buffer;
  case OP_NOT:
    sprintf(buffer, "op(!)");
    return buffer;
  case OP_OR:
    sprintf(buffer, "op(||)");
    return buffer;
  case OP_AND:
    sprintf(buffer, "op(&&)");
    return buffer;
  case OP_ASS:
    sprintf(buffer, "op(:=)");
    return buffer;
  case OP_EQ:
    sprintf(buffer, "op(==)");
    return buffer;
  case OP_GT:
    sprintf(buffer, "op(>)");
    return buffer;
  case OP_LT:
    sprintf(buffer, "op(<)");
    return buffer;
  case OP_LTE:
    sprintf(buffer, "op(<=)");
    return buffer;
  case OP_GTE:
    sprintf(buffer, "op(>=)");
    return buffer;
  case OP_SL:
    sprintf(buffer, "op(>>)");
    return buffer;
  case OP_SR:
    sprintf(buffer, "op(<<)");
    return buffer;
    break;
  }
  return buffer;
}

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

void Telos_Lexer_Advance(Lexer* lex) {
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
        Telos_Lexer_Advance(lex);
    }
}

Token Telos_Eat_Number(Lexer *lex) {
    usize32 init = lex->cursor;
    if (lex->input[lex->cursor] == '-') {
        Telos_Lexer_Advance(lex);
    }
    while (is_num(lex->input[lex->cursor])) {
        Telos_Lexer_Advance(lex);
    }
    if (lex->input[lex->cursor] == '.') {
        Telos_Lexer_Advance(lex);
        while (is_num(lex->input[lex->cursor])) {
            Telos_Lexer_Advance(lex);
        }

        usize32 size = lex->cursor - init;
        char *code = (char *)calloc(size, sizeof(char));
        if (!code) {
            exit(2);
        }
        errno_t err = memcpy_s(code, size, lex->input + init, size);
        code[size] = '\0';
        real32 f = std::stof(code);
        free(code);
        return Token{
            .line = lex->line,
            .col = init,
            .token_size = size,
            .code = nullptr,
            .type = F32,
            .real = f,
        };
    }

    usize32 size = lex->cursor - init;
    char* code = (char*) calloc(size, sizeof(char));
    if (!code) {
        exit(2);
    }

    errno_t err = memcpy_s(code, size, lex->input + init, size);
    code[size] = '\0';

    number32 x = atoi(code);
    free(code);
    return Token{
        .line = lex->line,
        .col = init,
        .token_size = size,
        .code = nullptr,
        .type = I32,
        .number = x,
    };
}

Token Telos_Eat_Identifier(Lexer *lex) {
    usize32 init = lex->cursor;
    while (is_alfa(lex->input[lex->cursor])) {
        Telos_Lexer_Advance(lex);
    }
    usize32 size = lex->cursor - init;
    char* code = (char*) malloc(sizeof(char) * (size + 1));
    errno_t err = memcpy_s(code, size + 1, lex->input + init,
              size + 1);

    code[size] = '\0';
    return Token {
        .line = lex->line,
        .col = init,
        .token_size = lex->cursor - init,
        .code = code,
        .type = IDENTIFIER,
    };
}

Lexer Telos_Lexer_init(char *input, usize32 size) {
    return Lexer{.input = input, .input_size = size, .cursor = 0, .peak = 1, .line = 1};
}

int main(int argc, char *argv[]) {
    char input[] = ":= == > < 3 <= >= >> << +-*/&|^ 0.33 -5.2 32 -21 \n  oi     iiii aaee     input     ola";
    Lexer lex = Telos_Lexer_init(input, strlen(input));
    std::vector<Token> program{};
    while(lex.cursor < lex.input_size) {
        Telos_eat_whitespace(&lex);
        switch (lex.input[lex.cursor]) {
        case '+': {
            program.push_back(Token{
                .line = lex.line,
                .col = lex.cursor,
                .token_size = 1,
                .code = nullptr,
                .type = OP_PLUS,
            });
        } break;
        case '*': {
            program.push_back(Token{
                .line = lex.line,
                .col = lex.cursor,
                .token_size = 1,
                .code = nullptr,
                .type = OP_MULT,
            });
        } break;
        case '/': {
            program.push_back(Token{
                .line = lex.line,
                .col = lex.cursor,
                .token_size = 1,
                .code = nullptr,
                .type = OP_DIV,
            });
        } break;
        case '-': {
            if (is_num(lex.input[lex.peak])) {
                program.push_back(Telos_Eat_Number(&lex));
            } else {
                program.push_back(Token{
                    .line = lex.line,
                    .col = lex.cursor,
                    .token_size = 1,
                    .code = nullptr,
                    .type = OP_MINUS,
                });
            }
        } break;
        case '&': {
            program.push_back(Token{
                .line = lex.line,
                .col = lex.cursor,
                .token_size = 1,
                .code = nullptr,
                .type = OP_BT_AND,
            });
        } break;
        case '|': {
            program.push_back(Token{
                .line = lex.line,
                .col = lex.cursor,
                .token_size = 1,
                .code = nullptr,
                .type = OP_BT_OR,
            });
        } break;
        case '^': {
            program.push_back(Token{
                .line = lex.line,
                .col = lex.cursor,
                .token_size = 1,
                .code = nullptr,
                .type = OP_BT_XOR,
            });
        } break;

        case '!': {
            program.push_back(Token{
                .line = lex.line,
                .col = lex.cursor,
                .token_size = 1,
                .code = nullptr,
                .type = OP_NOT,
            });
        } break;
        case ':': {
            if (lex.input[lex.peak] == '=') {
                program.push_back(Token{
                    .line = lex.line,
                    .col = lex.cursor,
                    .token_size = 1,
                    .code = nullptr,
                    .type = OP_ASS,
                });
                Telos_Lexer_Advance(&lex);
            } else {
                printf("Invalid sintax at (%zu:%zu)", lex.line, lex.cursor);
            }
        } break;
        case '=': {
            if (lex.input[lex.peak] == '=') {
                program.push_back(Token{
                    .line = lex.line,
                    .col = lex.cursor,
                    .token_size = 1,
                    .code = nullptr,
                    .type = OP_EQ,
                });
                Telos_Lexer_Advance(&lex);
            } else {
                printf("Invalid sintax at (%zu:%zu)", lex.line, lex.cursor);
            }
        } break;
        case '>': {
            if (lex.input[lex.peak] == '>') {
                program.push_back(Token{
                    .line = lex.line,
                    .col = lex.cursor,
                    .token_size = 1,
                    .code = nullptr,
                    .type = OP_SL,
                });
                Telos_Lexer_Advance(&lex);
            } else if (lex.input[lex.peak] == '=') {
                program.push_back(Token{
                    .line = lex.line,
                    .col = lex.cursor,
                    .token_size = 1,
                    .code = nullptr,
                    .type = OP_GTE,
                });
                Telos_Lexer_Advance(&lex);
            } else {
                program.push_back(Token{
                    .line = lex.line,
                    .col = lex.cursor,
                    .token_size = 1,
                    .code = nullptr,
                    .type = OP_GT,
                });

            }
        } break;
        case '<': {
            if (lex.input[lex.peak] == '<') {
                program.push_back(Token{
                    .line = lex.line,
                    .col = lex.cursor,
                    .token_size = 1,
                    .code = nullptr,
                    .type = OP_SR,
                });
                Telos_Lexer_Advance(&lex);
            } else if (lex.input[lex.peak] == '=') {
                program.push_back(Token{
                    .line = lex.line,
                    .col = lex.cursor,
                    .token_size = 1,
                    .code = nullptr,
                    .type = OP_LTE,
                });
                Telos_Lexer_Advance(&lex);
            } else {
                program.push_back(Token{
                    .line = lex.line,
                    .col = lex.cursor,
                    .token_size = 1,
                    .code = nullptr,
                    .type = OP_LT,
                });

            }
        } break;
        }
        if (is_alfa(lex.input[lex.cursor])) {
            Token id = Telos_Eat_Identifier(&lex);
            program.push_back(id);
        } else if (is_num(lex.input[lex.cursor])) {
            Token num = Telos_Eat_Number(&lex);
            program.push_back(num);
        }
        Telos_Lexer_Advance(&lex);
    }

    for(auto &token : program) {
        printf("Token of type %s: \n", Token_Name(token));
    }
    return 0;
}
