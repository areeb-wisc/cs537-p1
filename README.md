# CS537 Fall 2024, Project 1

## Author
Name - Mohammad Areeb

CS Login - areeb@cs.wisc.edu

Net ID - 9087095403

Email - areeb2@wisc.edu

## Status
Working as intended

## Assumptions
1. Blank lines in user input will be ignored, and not treated as an intended word
2. Blank lines in the board will be ignored, and not treated as a valid side with 0 characters
3. Blank lines in the dictionary will be ignored (empty dictioary is still valid)
4. Board, dictionary, and input files can be any length, words in each one can also be any length
5. No white spaces before, in between, and after any words (except '\n' at the end of each word)
   
   i.e. Each line in above files is a single string with only 'a-z' terminated with a '\n'

## Implementation logic / Pseudo Code
1. Load board and dictionary
2. Check if board is valid: sides < 3, or repeated character on board = INVALID board
3. Until we reach EOF for user input, do the following:
   * Read a word
   * Check if it uses all characters from the board, else fail
   * Check if it is connected to previous word (if any), else fail
   * Check if it uses consecutive characters from the same side of the board, fail if it does
   * Check if it is present in the dictionary, else fail
   * Add it to solution
   * Check if the solution is valid at this point, exit wit SUCCESS if true, else keep going
  
4. If we reach this step, fail, as it means all the letters on the board haven't been used

## Added these test cases
Test case 4 - Empty board

Test case 5 - Board with less than 3 sides

Test case 6 - Board with repeated character

Test case 7 - Playing a character not on board

Test case 8 - Using disconnected words

Test case 9 - Using same side letter used consecutively

Test case 10 - Using a word not present in dictionary

Test case 11 - Game complete without using all board letters

Test case 12 - Error opening file

Test case 13 - Board with blank lines

Test case 14 - Input with blank lines

Test case 15 - Empty dictionary

Test case 16 - Dictionary with blank lines
