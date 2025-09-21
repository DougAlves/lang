#include <corecrt_wstdio.h>
#include <cstdio>
#include <stdint.h>
#include <stdio.h>
#include <vcruntime_string.h>
#include <vector>
#include "token.cpp"
#include "lexer.cpp"



enum AstType {
  TELOS_IMPORT_STMT,
  TELOS_CONST_DECL,
  TELOS_VAR_DECL,
};

struct Telos_Import {
  Token path;
};

struct Telos_Const {
  Token name;
  Token value;
};

struct Telos_Var {
  Token name;
  Token value;
};

struct Telos_Node {
  AstType type;
  union {
    Telos_Import import_stmt;
    Telos_Const const_stmt;
    Telos_Var var_stmt;
  };
};

struct Telos_Program {
  Telos_Node* statements;
  usize32 capacity;
  usize32 size;
};

void Telos_Add_Node(Telos_Program* program, Telos_Node node) {
  if (program->size >= program->capacity) {
    printf("realocing...\n");
    auto temp = (Telos_Node*) malloc(sizeof(Telos_Node) * program->capacity * 2);
    program->capacity *= 2;
    memcpy(temp, program->statements, program->capacity * sizeof(Telos_Node));
    free(program->statements);
    program->statements = temp;
  }
  printf("adding node...\n");
  program->statements[program->size] = node;
  program->size++;
}

Telos_Program root = Telos_Program{ nullptr };

Token Telos_parse_literal(std::vector<Token> program, usize32 i) {
  switch(program[i].type) {
    case STRING_LITERAL :
    case KW_FALSE:
    case KW_TRUE:
    case INT_LITERAL:
    case FLOAT_LITERAL:
      return program[i];
    default: {
      char buffer[100];
      Token_Name(program[i], buffer);
      fprintf(stderr,
              "Erro (%zu:%zu) Expected Literal but got %s\n",
              program[i].line, program[i].col, buffer);
      exit(2);
    }
  }
}
Token Telos_parse_type(std::vector<Token> program, usize32 i) {
  switch (program[i].type) {
  case I8:
  case I16:
  case I32:
  case I64:
  case F8:
  case F16:
  case F32:
  case F64:
  case U8:
  case U16:
  case U32:
  case U64:
      return program[i];
  default: {
      fprintf(stderr, "Erro (%zu:%zu) Expected type but got %d\n",
              program[i].line, program[i].col, program[i].type);
      exit(2);
  }
  }
}

void parser_outside_statements(std::vector<Token> program, usize32 &i) {
  while (i < program.size()) {
      switch (program[i].type) {
      case IDENTIFIER: {

        // NOTE(doug): parse constants
        if (i + 1 < program.size() && program[i + 1].type == TY_DESC) {
          auto lit = Telos_parse_literal(program, i + 2);
          Telos_Const stmt = {};
          stmt.name = program[i];
          stmt.value = program[i + 2];
          Telos_Node x = {};
          x.type = TELOS_CONST_DECL;
          x.const_stmt = stmt;
          Telos_Add_Node(&root, x);
          i += 2;
        }// NOTE(doug): parse vars
        else if (i + 1 < program.size() && program[i + 1].type == TY_ASS) {
          auto type = Telos_parse_type(program, i + 2);

          if (program[i + 3].type != OP_ASS)  {
            fprintf(stderr, "Erro (%zu:%zu) Expected '=' but got %d\n",
                    program[i].line, program[i].col, program[i].type);
            exit(2);
          }
          // parse expression
          Telos_Var stmt = {};
          stmt.name = program[i];
          stmt.value = program[i + 2];
          Telos_Node x = {};
          x.type = TELOS_VAR_DECL;
          x.var_stmt = stmt;
          Telos_Add_Node(&root, x);
          i += 4;
        }
      } break;
      case KW_IMPORT: {
        if (i + 1 < program.size() && program[i + 1].type == STRING_LITERAL) {
          Telos_Add_Node(&root, Telos_Node{TELOS_IMPORT_STMT, program[i + 1]});
          i +=1;
        } else {
          fprintf(stderr,
                  "Erro (%zu:%zu) `import` deve ser seguido de uma string\n",
                  program[i].line, program[i].col);
          exit(1);
        }
      } break;
      default: {
        char buffer[100];
        Token_Name(program[i], buffer);
        printf("saiu pq entrou aqui %s ??\n", buffer);
        return;
      }
      }
      i++;
  }
}

void parse_program(std::vector<Token> program) {
  usize32 i = 0;
  parser_outside_statements(program, i);
}

int main(int argc, char *argv[]) {
    char input[] = "import \"teste\"\n x :: 69\n PI :: 3.141569\n z: i32 =";
    auto program = Lexer_parse_program(input);
    char buffer[100];
    for(auto &token : program) {
        Token_Name(token, buffer);
        printf("Token of type %s \n", buffer);
    }
    root.statements = (Telos_Node*) malloc(sizeof(Telos_Node) * 100);
    root.size = 0;
    root.capacity = 100;
    parse_program(program);


    printf("Program size %zu: \n", root.size);
    for (usize32 i = 0; i < root.size; i++ ){
      printf("stmt type %d: \n", root.statements[i].type);
      switch(root.statements[i].type) {
        case TELOS_IMPORT_STMT: {
          Token_Name(root.statements[i].import_stmt.path, buffer);
          printf("Importing %s: \n", buffer);
        } break;
        case TELOS_CONST_DECL: {
          Token_Name(root.statements[i].const_stmt.name, buffer);
          printf("%s :: ", buffer);
          Token_Name(root.statements[i].const_stmt.value, buffer);
          printf("%s\n", buffer);
        } break;
        case TELOS_VAR_DECL: {
          Token_Name(root.statements[i].var_stmt.name, buffer);
          printf("%s :: ", buffer);
          Token_Name(root.statements[i].var_stmt.value, buffer);
          printf("%s\n", buffer);
        } break;
        }
    }
    return 0;
}
