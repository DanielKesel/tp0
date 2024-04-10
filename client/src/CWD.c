#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
    char *current_dir = getcwd(NULL, 0);
    printf("El directorio actual es %s\n", current_dir);
    free(current_dir);
    return 0;
}
