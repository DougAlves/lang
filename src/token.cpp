#include <stdio.h>
#include <string.h>


typedef float real32;
typedef int number32;
typedef size_t usize32;

enum Token_Type {
  IDENTIFIER,
  U8,
  U16,
  U32,
  U64,
  F8,
  F16,
  F32,
  F64,
  I8,
  I16,
  I32,
  I64,
  LB,
  RB,
  KW_TRUE,
  KW_FALSE,
  KW_FOR,
  KW_IF,
  KW_ELSE,
  KW_DO,
  KW_END,
  KW_PROC,
  KW_TELOS,
  OP_PLUS,
  OP_MINUS,
  OP_MULT,
  OP_DIV,
  OP_BT_OR,
  OP_BT_AND,
  OP_BT_XOR,
  OP_NOT,
  OP_NOTEQ, // !=
  OP_ACESS, // .
  OP_OR,
  OP_AND,
  TY_ASS, // ::
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

int Token_Name(Token t, char* buffer) {
  switch (t.type) {
  case IDENTIFIER: {
    sprintf(buffer, "i(%s)", t.code);
    return 1;
  }
  case F32:
    sprintf(buffer, "f(%f)", t.real);
    return 1;
  case I32:
    sprintf(buffer, "f(%d)", t.number);
    return 1;
  case OP_PLUS:
    sprintf(buffer, "op(+)");
    return 1;
  case OP_MINUS:
    sprintf(buffer, "op(-)");
    return 1;
  case OP_MULT:
    sprintf(buffer, "op(*)");
    return 1;
  case OP_DIV:
    sprintf(buffer, "op(/)");
    return 1;
  case OP_BT_OR:
    sprintf(buffer, "op(|)");
    return 1;
  case OP_BT_AND:
    sprintf(buffer, "op(&)");
    return 1;
  case OP_BT_XOR:
    sprintf(buffer, "op(^)");
    return 1;
  case OP_NOT:
    sprintf(buffer, "op(!)");
    return 1;
  case OP_OR:
    sprintf(buffer, "op(||)");
    return 1;
  case OP_AND:
    sprintf(buffer, "op(&&)");
    return 1;
  case OP_ASS:
    sprintf(buffer, "op(:=)");
    return 1;
  case OP_EQ:
    sprintf(buffer, "op(==)");
    return 1;
  case OP_NOTEQ:
    sprintf(buffer, "op(!=)");
    return 1;
    break;
  case OP_GT:
    sprintf(buffer, "op(>)");
    return 1;
  case OP_LT:
    sprintf(buffer, "op(<)");
    return 1;
  case OP_LTE:
    sprintf(buffer, "op(<=)");
    return 1;
  case OP_GTE:
    sprintf(buffer, "op(>=)");
    return 1;
  case OP_SL:
    sprintf(buffer, "op(>>)");
    return 1;
  case OP_SR:
    sprintf(buffer, "op(<<)");
    return 1;
  case U8:
    sprintf(buffer, "t(u8)");
    return 1;
  case U16:
    sprintf(buffer, "t(u16)");
    return 1;
  case U32:
    sprintf(buffer, "t(u32)");
    return 1;
  case U64:
    sprintf(buffer, "t(u64)");
    return 1;
  case F8:
    sprintf(buffer, "t(f8)");
    return 1;
  case F16:
    sprintf(buffer, "t(f16)");
    return 1;
  case F64:
    sprintf(buffer, "t(f64)");
    return 1;
  case I8:
    sprintf(buffer, "t(i8)");
    return 1;
  case I16:
    sprintf(buffer, "t(i16)");
    return 1;
  case I64:
    sprintf(buffer, "t(i64)");
    return 1;
  case KW_FOR:
    sprintf(buffer, "kw(for)");
    return 1;
  case KW_IF:
    sprintf(buffer, "kw(if)");
    return 1;
  case KW_ELSE:
    sprintf(buffer, "kw(else)");
    return 1;
  case KW_DO:
    sprintf(buffer, "kw(do)");
    return 1;
  case KW_END:
    sprintf(buffer, "kw(end)");
    return 1;
  case KW_PROC:
    sprintf(buffer, "kw(proc)");
    return 1;
  case KW_TELOS:
    sprintf(buffer, "kw(telos)");
    return 1;
    break;
  case TY_ASS:
    sprintf(buffer, "kw(::)");
    return 1;
    break;
  case KW_TRUE:
    sprintf(buffer, "kw(true)");
    return 1;
  case KW_FALSE:
    sprintf(buffer, "kw(false)");
    return 1;
  case OP_ACESS:
    sprintf(buffer, "kw(.)");
    return 1;
  case LB:
    sprintf(buffer, "kw([)");
    return 1;
  case RB:
    sprintf(buffer, "kw(])");
    return 1;
  }
  return 1;
}
