#include <stdio.h>

typedef float real32;
typedef int int32;

struct Token {
    int line;
    int col;
    char code[100];
    union {
        float32 d;
    };
};

int main(int argc, char *argv[]) {
    printf("helo \n");
    return 0;
}
