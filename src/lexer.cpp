#pragma once
#include "token.cpp"
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

struct Lexer {
    char *input;
    usize32 input_size;
    usize32 cursor;
    usize32 peak;
    usize32 line;
};

bool is_alfa(char c) {
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z');
        // || (c > '0' && c <= '9');
}

bool is_identifier_valid(char c) {
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c > '0' && c <= '9');
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

Token Telos_eat_string(Lexer* lex) {
    Telos_Lexer_Advance(lex);
    usize32 init = lex->cursor;
    while(lex->input[lex->cursor] != '"') {
        Telos_Lexer_Advance(lex);
    }
    usize32 size = lex->cursor - init;
    char *code = (char *)calloc(size, sizeof(char));
    errno_t err = memcpy_s(code, size, lex->input + init, size);
    code[size] = '\0';
    printf("returning %s\n", code);
    return Token{
        .line = lex->line,
        .col = init,
        .token_size = size,
        .code = nullptr,
        .type = STRING_LITERAL,
        .str_literal = code,
    };

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
            .type = FLOAT_LITERAL,
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
        .type = INT_LITERAL,
        .number = x,
    };
}

Token Telos_Eat_Identifier(Lexer *lex) {
    usize32 init = lex->cursor;
    while (is_identifier_valid(lex->input[lex->peak])) {
        Telos_Lexer_Advance(lex);
    }
    usize32 size = lex->peak - init;
    char* code = (char*) malloc(sizeof(char) * (size + 1));
    errno_t err = memcpy_s(code, size + 1, lex->input + init,
              size + 1);

    code[size] = '\0';
    if (strcmp(code, "for") == 0) {
        free(code);
        return Token{
            .line = lex->line,
            .col = init,
            .token_size = lex->cursor - init,
            .code = 0,
            .type = KW_FOR,
        };
    } else if (strcmp(code, "if") == 0) {
        free(code);
        return Token{
            .line = lex->line,
            .col = init,
            .token_size = lex->cursor - init,
            .code = 0,
            .type = KW_IF,
        };
    } else if (strcmp(code, "else") == 0) {
        free(code);
        return Token{
            .line = lex->line,
            .col = init,
            .token_size = lex->cursor - init,
            .code = 0,
            .type = KW_ELSE,
        };
    } else if (strcmp(code, "do") == 0) {
        free(code);
        return Token{
            .line = lex->line,
            .col = init,
            .token_size = lex->cursor - init,
            .code = 0,
            .type = KW_DO,
        };
    } else if (strcmp(code, "end") == 0) {
        free(code);
        return Token{
            .line = lex->line,
            .col = init,
            .token_size = lex->cursor - init,
            .code = 0,
            .type = KW_END,
        };
    } else if (strcmp(code, "proc") == 0) {
        free(code);
        return Token{
            .line = lex->line,
            .col = init,
            .token_size = lex->cursor - init,
            .code = 0,
            .type = KW_PROC,
        };
    } else if (strcmp(code, "telos") == 0) {
        free(code);
        return Token{
            .line = lex->line,
            .col = init,
            .token_size = lex->cursor - init,
            .code = 0,
            .type = KW_TELOS,
        };
    } else if (strcmp(code, "import") == 0) {
        free(code);
        return Token{
            .line = lex->line,
            .col = init,
            .token_size = lex->cursor - init,
            .code = 0,
            .type = KW_IMPORT,
        };
    } else if (strcmp(code, "i32") == 0) {
        free(code);
        return Token{
            .line = lex->line,
            .col = init,
            .token_size = lex->cursor - init,
            .code = 0,
            .type = I32,
        };
    } else if (strcmp(code, "f32") == 0) {
        free(code);
        return Token{
            .line = lex->line,
            .col = init,
            .token_size = lex->cursor - init,
            .code = 0,
            .type = F32,
        };
    }else if (strcmp(code, "i16") == 0) {
        free(code);
        return Token{
            .line = lex->line,
            .col = init,
            .token_size = lex->cursor - init,
            .code = 0,
            .type = I16,
        };
    }


    return Token{
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

std::vector<Token> Lexer_parse_program(char input[]) {
    Lexer lex = Telos_Lexer_init(input, strlen(input));
    std::vector<Token> program{};
    while(lex.cursor < lex.input_size) {
        Telos_eat_whitespace(&lex);
        switch (lex.input[lex.cursor]) {
        case '(': {
            program.push_back(Token{
                .line = lex.line,
                .col = lex.cursor,
                .token_size = 1,
                .code = nullptr,
                .type = LP,
            });
        } break;
        case ')': {
            program.push_back(Token{
                .line = lex.line,
                .col = lex.cursor,
                .token_size = 1,
                .code = nullptr,
                .type = RP,
            });
        } break;
        case '[': {
            program.push_back(Token{
                .line = lex.line,
                .col = lex.cursor,
                .token_size = 1,
                .code = nullptr,
                .type = LB,
            });
        } break;
        case ']': {
            program.push_back(Token{
                .line = lex.line,
                .col = lex.cursor,
                .token_size = 1,
                .code = nullptr,
                .type = RB,
            });
        } break;
        case '.': {
            if (lex.input[lex.peak] == '.') {
              program.push_back(Token{
                  .line = lex.line,
                  .col = lex.cursor,
                  .token_size = 1,
                  .code = nullptr,
                  .type = OP_INTERVAL,
              });
              Telos_Lexer_Advance(&lex);
            } else {
              program.push_back(Token{
                  .line = lex.line,
                  .col = lex.cursor,
                  .token_size = 1,
                  .code = nullptr,
                  .type = OP_ACESS,
              });
            }
        } break;
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
            if (lex.input[lex.peak] == '=') {
                program.push_back(Token{
                    .line = lex.line,
                    .col = lex.cursor,
                    .token_size = 1,
                    .code = nullptr,
                    .type = OP_NOTEQ,
                });
                Telos_Lexer_Advance(&lex);
            } else {
                program.push_back(Token{
                    .line = lex.line,
                    .col = lex.cursor,
                    .token_size = 1,
                    .code = nullptr,
                    .type = OP_NOT,
                });
            }
        } break;
        case ':': {
            if (lex.input[lex.peak] == ':') {
                program.push_back(Token{
                    .line = lex.line,
                    .col = lex.cursor,
                    .token_size = 2,
                    .code = nullptr,
                    .type = TY_DESC,
                });
                Telos_Lexer_Advance(&lex);
            } else {
                program.push_back(Token{
                    .line = lex.line,
                    .col = lex.cursor,
                    .token_size = 1,
                    .code = nullptr,
                    .type = TY_ASS,
                });
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
                program.push_back(Token{
                    .line = lex.line,
                    .col = lex.cursor,
                    .token_size = 1,
                    .code = nullptr,
                    .type = OP_ASS,
                });
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
        case '"': {
            Token str = Telos_eat_string(&lex);
            program.push_back(str);

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
    return program;
}
