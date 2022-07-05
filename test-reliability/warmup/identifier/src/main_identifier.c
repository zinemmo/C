#include <stdio.h>
#include "identifier.h"

// gcc main_identifier.c identifier.c -o identifier
// ./identifier <argumento do make ou default>

int main(int argc, char *argv[]){
    if(argc == 1) {
        printf("Insira um identificador para ser avaliado em tempo de compilação.\n");
    } else {
        identifier(argv[1], 1);
    }
    return 0;
}