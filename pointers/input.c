#include <stdio.h>
#include <stdlib.h>

const int BUFFER_SIZE = 512;

void input(char buffer[]) {
    fgets(buffer, BUFFER_SIZE * sizeof(char), stdin);
}

static char * inputStaticBuffer() {
    static char buffer[512];
    fgets( buffer, BUFFER_SIZE * sizeof(char), stdin);
    return buffer;
}

char * inputDynamic() {
    char * buffer = (char *) malloc(BUFFER_SIZE);
    fgets(buffer, BUFFER_SIZE * sizeof(char), stdin);
    return buffer;
}

//static int main() {
//    char buffer[BUFFER_SIZE];
//    input(buffer);
//    printf("%s", buffer);
//    printf("%s", inputDynamic());
//    return 0;
//}