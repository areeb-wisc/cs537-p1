#include<stdio.h>
#include<stdlib.h>

char** readFile(FILE* file, char** lines) {
    if (file == NULL)
        exit(EXIT_FAILURE);

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

   while ((read = getline(&line, &len, file)) != -1) {
        printf("Retrieved line of length %zu :\n", read);
        printf("%s", line);
    }
   free(line);
}

int main() {
    FILE* fp = fopen("board1.txt","r");
    
    return 0;
}