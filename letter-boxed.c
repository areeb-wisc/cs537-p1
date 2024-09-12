#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<limits.h>

void my_exit(int exit_code, char* message) {
    printf("%s\n", message);
    exit(exit_code);
}

void readFile(char* file_name, char*** lines, long int* num_lines) {

    FILE* file = fopen(file_name,"r");

    if (file == NULL)
        exit(EXIT_FAILURE);

    char *line = NULL;
    unsigned long len = 0;
    long read;
    
    long int max_lines = 1;
    *lines = (char**)malloc(max_lines * sizeof(char*));

    long int idx = 0;
    while ((read = getline(&line, &len, file)) != -1) {

        if (idx == max_lines) { // buffer full, need to resize
            max_lines *= 2;
            *lines = (char**)realloc(*lines, max_lines * sizeof(char*));
        }
        
        if (read > 1) { // ignore blank lines
            (*lines)[idx] = (char*)malloc(read);
            strncpy((*lines)[idx], line, read - 1); // copy excluding '\n'
            (*lines)[idx][read - 1] = '\0'; // adding '\0'
            idx++;
        }
    }

    *num_lines = idx;
    *lines = (char**)realloc(*lines, *num_lines * sizeof(char*));
    
    free(line);
    fclose(file);
}

void printArrayOfStrings(char** arr, int size) {
    printf("size = %d\n", size);
    if (size > 10)
        size = 10;
    for(int i = 0; i < size; i++)
        printf("%s\n", arr[i]);
}

void check_if_word_is_on_board(bool* letters_on_board, char* word) {
    printf("check_if_word_is_on_board\n");
    for (int i = 0; word[i] != '\0'; i++) {
        if (!letters_on_board[word[i] - 'a']) {
            printf("wrong letter %c\n", word[i]);
            my_exit(EXIT_FAILURE, "Used a letter not present on the board\n");
        }
    }
}

void check_if_word_is_connected_to_previous(char** solution, int solution_size, char* word) {
    printf("check_if_word_is_connected_to_previous\n");
    if (solution_size > 0) {
        char* last_word_of_solution = solution[solution_size - 1];
        unsigned long length_of_last_word = strlen(last_word_of_solution);
        char last_character_of_last_word = last_word_of_solution[length_of_last_word - 1];
        
        if (word[0] != last_character_of_last_word)
            my_exit(EXIT_FAILURE, "First letter of word does not match last letter of previous word\n");
        }
}

// this is wrong, correct it
// create vector of all disallowed consecutive characters
void check_if_word_has_consecutive_same_side_letters(char** board, int num_sides, char* word) {
    printf("check_if_word_has_consecutive_same_side_letters\n");
    board = board; // remove
    num_sides = num_sides; // remove
    for (int i = 1; word[i] != '\0'; i++) {
        if (word[i] == word[i-1])
            my_exit(EXIT_FAILURE, "Same-side letter used consecutively\n");
    }
}

void check_if_word_is_in_dictionary(char** dict, long int dict_size, unsigned long shortest, unsigned long longest, char* word) {

    printf("check_if_word_is_in_dictionary\n");

    if (strlen(word) < shortest || strlen(word) > longest)
        my_exit(EXIT_FAILURE, "Word not found in dictionary\n");

    for (int i = 0; i < dict_size; i++) {
        if (strcmp(word, dict[i]) == 0) // word found in dict
            return;
    }
    my_exit(EXIT_FAILURE, "Word not found in dictionary\n");
}

void check_if_board_is_valid(char** board, int num_sides, bool** letters_on_board) {

    printf("check_if_board_is_valid\n");

    if (num_sides < 3)
        my_exit(EXIT_FAILURE, "Invalid board\n");

    bool* seen = (bool*)malloc(26 * sizeof(bool));
    for (int i = 0; i < 26; i++)
        seen[i] = false;

    for (int i = 0; i < num_sides; i++) {
        for (unsigned long j = 0; j < strlen(board[i]); j++) {
            if (seen[board[i][j] - 'a'])
                my_exit(EXIT_FAILURE, "Invalid board\n");
            seen[board[i][j] - 'a'] = true;
        }
    }
    *letters_on_board = seen;
}

bool is_game_solved(bool* letters_on_board, bool* letters_in_solution) {
    printf("is_game_solved\n");

    for (int i = 0; i < 26; i++) {
        // letter present on board but not in solution
        if (letters_on_board[i] && !letters_in_solution[i])
            return false;
    }
    return true;    
}

void play_game(char** board, long int num_sides, char** dict, long int dict_size) {
    printf("play_game\n");
    
    // board metadata
    bool* letters_on_board = NULL;
    check_if_board_is_valid(board, num_sides, &letters_on_board);

    // dictionary metadata
    unsigned long shortest = strlen(dict[0]);
    unsigned long longest = strlen(dict[0]);

    for (int i = 1; i < dict_size; i++) {
        unsigned long len = strlen(dict[i]);
        if (len < shortest)
            shortest = len;
        if (len > longest)
            longest = len;
    }

    // solution vector
    int solution_size = 0;
    int max_solution_size = 10;
    char** solution = (char**)malloc(max_solution_size * sizeof(char*));
    bool letters_in_solution[26] = {false};
    
    char* word = NULL;
    unsigned long size = 10;
    int word_len = 0;

    while ((word_len = getline(&word, &size, stdin)) != EOF) {

        word[word_len - 1] = '\0';
        printf("word read = %s\n", word);

        check_if_word_is_on_board(letters_on_board, word);

        check_if_word_is_connected_to_previous(solution, solution_size, word);

        check_if_word_has_consecutive_same_side_letters(board, num_sides, word);

        check_if_word_is_in_dictionary(dict, dict_size, shortest, longest, word);

        // word is valid at this point, hence add it to solution
        solution_size++;
        if (solution_size == max_solution_size) {
            max_solution_size *= 2;
            solution = (char**)realloc(solution, max_solution_size * sizeof(char*));
        }
        solution[solution_size - 1] = (char*)malloc(word_len * sizeof(char));
        strcpy(solution[solution_size - 1], word);
        for (int i = 0; i < word_len - 1; i++)
            letters_in_solution[word[i] - 'a'] = true;

        if (is_game_solved(letters_on_board, letters_in_solution))
            my_exit(EXIT_SUCCESS, "Correct\n");
    }

    free(letters_on_board);
    free(solution);
    free(word);

    my_exit(EXIT_FAILURE, "Not all letters used\n");
}

int main(int argc, char** argv) {

    if (argc != 3)
        exit(EXIT_FAILURE);

    char** board = NULL;
    long int num_sides = 0;
    readFile(argv[1], &board, &num_sides);
    printArrayOfStrings(board, num_sides);

    char** dict = NULL;
    long int dict_size = 0;
    readFile(argv[2], &dict, &dict_size);
    // printArrayOfStrings(dict, dict_size);

    play_game(board, num_sides, dict, dict_size);

    return 0;
}
