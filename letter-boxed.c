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

void strip(char** word, ssize_t* len) {
    if (*len > 0) {
        if ((*word)[(*len) - 1] == '\n') {
            (*word)[(*len) - 1] = '\0';
            *len -= 1;
        }
    }
}

void readFile(char* file_name, char*** lines, long int* num_lines) {

    FILE* file = fopen(file_name,"r");

    if (file == NULL)
        exit(EXIT_FAILURE);

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    
    long int max_lines = 1;
    *lines = (char**)malloc(max_lines * sizeof(char*));

    long int idx = 0;
    while ((read = getline(&line, &len, file)) != -1) {

        if (idx == max_lines) { // buffer full, need to resize
            max_lines *= 2;
            *lines = (char**)realloc(*lines, max_lines * sizeof(char*));
        }
        
        strip(&line, &read);
        if (read > 0) { // ignore blank lines
            (*lines)[idx] = (char*)malloc(read + 1);
            strcpy((*lines)[idx], line);
            idx++;
        }
    }

    *num_lines = idx;
    *lines = (char**)realloc(*lines, *num_lines * sizeof(char*));
    
    free(line);
    fclose(file);
}

void check_if_word_is_on_board(bool* letters_on_board, char* word) {
    for (long int i = 0; word[i] != '\0'; i++) {
        if (!letters_on_board[word[i] - 'a'])
            my_exit(EXIT_SUCCESS, "Used a letter not present on the board\n");
    }
}

void check_if_word_is_connected_to_previous(char** solution, int solution_size, char* word) {
    if (solution_size > 0) {
        char* last_word_of_solution = solution[solution_size - 1];
        unsigned long length_of_last_word = strlen(last_word_of_solution);
        char last_character_of_last_word = last_word_of_solution[length_of_last_word - 1];
        
        if (word[0] != last_character_of_last_word)
            my_exit(EXIT_SUCCESS, "First letter of word does not match last letter of previous word\n");
    }
}

void check_if_word_has_consecutive_same_side_letters(char** invalid_sequences, size_t total_invalid_sequences, char* word) {
    for (size_t i = 0; word[i] != '\0'; i++) {
        char sequence[3];
        sequence[0] = word[i];
        sequence[1] = word[i + 1];
        sequence[2] = '\0';
        for (size_t j = 0; j < total_invalid_sequences; j++) {
            if (strcmp(sequence, invalid_sequences[j]) == 0) // sequence is invalid
                my_exit(EXIT_SUCCESS, "Same-side letter used consecutively\n");
        }
    }
}

void check_if_word_is_in_dictionary(char** dict, long int dict_size, unsigned long shortest, unsigned long longest, char* word) {

    if (strlen(word) < shortest || strlen(word) > longest)
        my_exit(EXIT_SUCCESS, "Word not found in dictionary\n");

    for (long int i = 0; i < dict_size; i++) {
        if (strcmp(word, dict[i]) == 0) // word found in dict
            return;
    }
    my_exit(EXIT_SUCCESS, "Word not found in dictionary\n");
}

void check_if_board_is_valid(char** board, long int num_sides, bool** letters_on_board, char*** invalid_sequences, size_t* total_invalid_sequences) {

    if (num_sides < 3)
        my_exit(EXIT_FAILURE, "Invalid board\n");

    bool* seen = (bool*)malloc(26 * sizeof(bool));
    for (int i = 0; i < 26; i++)
        seen[i] = false;

    size_t invalid_sequence_idx = -1;

    for (long int i = 0; i < num_sides; i++) {

        char* side = board[i];
        size_t side_len = strlen(side);

        // check if this side has a character already present on some other side
        for (size_t j = 0; j < side_len; j++) {
            if (seen[side[j] - 'a'])
                my_exit(EXIT_FAILURE, "Invalid board\n");
            seen[side[j] - 'a'] = true;
        }

        // store this side's invalid sequences for gameplay
        *total_invalid_sequences += side_len * side_len;
        *invalid_sequences = (char**)realloc(*invalid_sequences, *total_invalid_sequences * sizeof(char*));
        for (size_t j = 0; j < side_len; j++) {
            for (size_t k = 0; k < side_len; k++) {
                char* invalid_sequence = (char*)malloc(3 * sizeof(char));
                invalid_sequence[0] = side[j];
                invalid_sequence[1] = side[k];
                invalid_sequence[2] = '\0';
                (*invalid_sequences)[++invalid_sequence_idx] = invalid_sequence;
            }
        }
    }
    *letters_on_board = seen;
}

bool is_game_solved(bool* letters_on_board, bool* letters_in_solution) {
    for (int i = 0; i < 26; i++) {
        // letter present on board but not in solution
        if (letters_on_board[i] && !letters_in_solution[i])
            return false;
    }
    return true;    
}

void play_game(char** board, long int num_sides, char** dict, long int dict_size) {
    
    // board metadata
    bool* letters_on_board = NULL;
    char** invalid_sequences = NULL;
    size_t total_invalid_sequences = 0;
    check_if_board_is_valid(board, num_sides, &letters_on_board, &invalid_sequences, &total_invalid_sequences);

    // dictionary metadata
    unsigned long shortest = INT_MAX;
    unsigned long longest = INT_MIN;

    for (long int i = 1; i < dict_size; i++) {
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
    size_t size = 10;
    ssize_t word_len = 0;

    while ((word_len = getline(&word, &size, stdin)) != EOF) {

        strip(&word, &word_len);

        if (word_len == 0) // ignore blank input lines
            continue;

        check_if_word_is_on_board(letters_on_board, word);

        check_if_word_is_connected_to_previous(solution, solution_size, word);

        check_if_word_has_consecutive_same_side_letters(invalid_sequences, total_invalid_sequences, word);

        check_if_word_is_in_dictionary(dict, dict_size, shortest, longest, word);

        // word is valid at this point, hence append it to solution
        solution_size++;
        if (solution_size == max_solution_size) {
            max_solution_size *= 2;
            solution = (char**)realloc(solution, max_solution_size * sizeof(char*));
        }

        solution[solution_size - 1] = (char*)malloc((word_len + 1) * sizeof(char));
        strcpy(solution[solution_size - 1], word);

        for (ssize_t i = 0; i < word_len; i++)
            letters_in_solution[word[i] - 'a'] = true;

        if (is_game_solved(letters_on_board, letters_in_solution))
            my_exit(EXIT_SUCCESS, "Correct\n");
    }

    free(letters_on_board);
    free(solution);
    free(word);

    my_exit(EXIT_SUCCESS, "Not all letters used\n");
}

int main(int argc, char** argv) {

    if (argc != 3)
        exit(EXIT_FAILURE);

    char** board = NULL;
    long int num_sides = 0;
    readFile(argv[1], &board, &num_sides);

    char** dict = NULL;
    long int dict_size = 0;
    readFile(argv[2], &dict, &dict_size);

    play_game(board, num_sides, dict, dict_size);

    return 0;
}
