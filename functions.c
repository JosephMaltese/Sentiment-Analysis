#include "functions.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// Function which converts the string containing a list of elements found on each line of the file to 
// an integer array of size 10.
void strToList(char *numStr, int *list) {
    // keeps track of the current integer value
    int currentDigit;
    // keeps track of position in array
    int count = 0;

    // sscanf returns 1 when an integer is successfully read from the current position in the string to the currentDigit
    // variable. Loops until there are no more integers to be read.
    while (sscanf(numStr, "%d", &currentDigit) == 1) {
        // assigns the first element in the string to the first position in the list.
        list[count] = currentDigit;
        // moves to next position in array
        count++;

        // moves to the next position in the string until one of these characters is reached. Typically, it will reach
        // a comma. If done reading all integers, it will move forward until it reaches ]. At this point, the condition 
        // of the while loop will be false and the loop will end.
        while (*numStr != '[' && *numStr != ']' && *numStr != ',' && *numStr != '\0') {
            numStr++;
        }

        // if a comma is reached, we move forward two elements in the string to the next integer so that the condition
        // of the loop will remain true and we can continue reading integers.
        if (*numStr == ',') {
            numStr++;
            numStr++;
        }

    }

}


// Function that reads from the VADER lexicon file and adds the data to an array of word structs
void readFromLexiconFile(char filename[], struct words **wordListPtr, int capacity, int *wordCount) {
    // open the file with the given name in reading mode
    FILE *readFile = fopen(filename, "r");

    // Error check to ensure that the file has been opened correctly
    if (readFile == NULL) {
        printf("Cannot access given filename for reading.\n");
        return;
    }

    // define a string variable to hold the contents of each line. Max size is set to 500 characters
    char line[500];

    // Define an integer array to temporarily hold the list present on each line of the file
    int numArr[10];

    // While loop that iterates until the end of the file is reached. Reads each line until it either reaches the 
    // max line length (500 characters) or reaches a newline character. Assigns the line to the previously defined line variable
    while (fgets(line, 500, readFile) != NULL) {
        
        // This statement ensures that the correct amount of memory is allocated based on how many lines are present in the file.
        // A default capacity of 1000 is set at the beginning of the program and passed as an argument to this function. Once the 
        // number of words (lines) read from the file reaches this capacity, the capacity is doubled, and the array is reallocated
        // to hold the new capacity of word structures.
        if (*wordCount >= capacity) {
            // double capacity
            capacity *= 2;
            // reallocate the memory into a temporary array
            struct words *newTemp = (struct words *)realloc(*wordListPtr, capacity*sizeof(struct words));

            // Handle the case that memory allocation fails
            if (newTemp == NULL) {
                printf("Memory Allocation Failed.\n");
                fclose(readFile);
                return;
            }
            // A pointer to a pointer to the array is given as an argument of this function. This is because if the
            // pointer to the array is modified within the function, the changes are not reflected in the main program.
            // Thus, a pointer to pointer is used. This current line makes it so that the pointer to the array now points to
            // the same array as the temp array.
            *wordListPtr = newTemp;
        }

        // The strtok function is used to break the current line into several tokens using a delimeter. In the text file,
        // the word and score are separated by a tab, and so is the score and SD. However, the SD and SIS list are separated by a space.
        // Thus, the tab character is used as a delimeter for the first two tokens.
        char *currentWord = strtok(line, "\t");
        // atof is used to convert the string data to float to obtain the score
        float currentScore = atof(strtok(NULL, "\t"));
        // space is used as a delimeter here since a space indicates the boundary between the SD and the SIS list
        float currentSD = atof(strtok(NULL, " "));
        // The newline character is used as a delimeter here since the SIS list is the last element on each line.
        char *currentSISstr = strtok(NULL, "\n");

        ///////////////////////////////
        // printf("%s ", currentWord);
        // printf("%f ", currentScore);
        // printf("%f ", currentSD);
        // printf("%s\n", currentSISstr);
        ///////////////////////////////////

        // Pointer notation is used to access the relevant element in the array of structures. The word member of the
        // current structure element is assigned a duplicate of a pointer to the current word token. Memory is allocated 
        // by the strdup function automatically based on the current word.
        ((*wordListPtr) + *wordCount)->word = strdup(currentWord);
        // Handles failed memory allocation
        if (((*wordListPtr) + *wordCount)->word == NULL) {
            printf("Failed to allocate memory.\n");
            fclose(readFile);
            return;
        }

        // Uses pointer notation to access the current structure element in the array of structures. The corresponding score
        // and SD values are assigned the currentScore and currentSD tokens obtained earlier.
        ((*wordListPtr) + *wordCount)->score = currentScore;
        ((*wordListPtr) + *wordCount)->SD = currentSD;

        // This function converts the token pointing to the string which contains the SIS list to an actual integer array
        // by assigning the elements to the numArr array defined at the top of the function.
        strToList(currentSISstr, numArr);

        // Since the list contains 10 elements, loops and assigns each of the 10 elements in numArr to the corresponding word structure
        // SIS_array element
        for (int i = 0; i < 10; i++) {
            (*wordListPtr)[*wordCount].SIS_array[i] = numArr[i];
        }

        // Increments the word count using pointer notation to keep track of how many lines have been read.
        *wordCount = *wordCount + 1;

    }
    // Closes the file once done with it.
    fclose(readFile);

}


///////// CHECK COMMENTS ON THIS FUNCTION ///////////////////
void readFromValFile(char filename[], struct sentences **sentenceListPtr, int capacity, int *sentenceCount) {
    FILE *readFile = fopen(filename, "r");

    // Error check to ensure that the file has been opened correctly
    if (readFile == NULL) {
        printf("Cannot access given filename for reading.\n");
        return;
    }

    char line[1000];

    while (fgets(line, 1000, readFile) != NULL) {
        if (*sentenceCount >= capacity) {
            // double capacity
            capacity *= 2;
            // reallocate the memory into a temporary array
            struct sentences *newTemp = (struct sentences *)realloc(*sentenceListPtr, capacity*sizeof(struct sentences));

            // Handle the case that memory allocation fails
            if (newTemp == NULL) {
                printf("Memory Allocation Failed.\n");
                fclose(readFile);
                return;
            }
            // A pointer to a pointer to the array is given as an argument of this function. This is because if the
            // pointer to the array is modified within the function, the changes are not reflected in the main program.
            // Thus, a pointer to pointer is used. This current line makes it so that the pointer to the array now points to
            // the same array as the temp array.
            *sentenceListPtr = newTemp;
        }

        int len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        // Pointer notation is used to access the relevant element in the array of structures. The word member of the
        // current structure element is assigned a duplicate of a pointer to the current word token. Memory is allocated 
        // by the strdup function automatically based on the current word.
        ((*sentenceListPtr) + *sentenceCount)->comment = strdup(line);
        // Handles failed memory allocation
        if (((*sentenceListPtr) + *sentenceCount)->comment == NULL) {
            printf("Failed to allocate memory.\n");
            fclose(readFile);
            return;
        }

        *sentenceCount = *sentenceCount + 1;
    }
    fclose(readFile);
}

void removePunctuation(char *word, char *newWord) {
    size_t word_length = strlen(word);
    int k = word_length - 1;
    while (k > 0 && (word[k] == ',' || word[k] == '!' || word[k] == '.')) {
        k--;
    }

    int j = 0;
    

    for (int i = 0; i <= k; i++) {
        if (isalpha(word[i]) && isupper(word[i])) {
            newWord[j] = word[i] + 32;
        }
        else {
            newWord[j] = word[i];            
        }
        j++;
    }
    newWord[j] = '\0';
    //printf("%s", newWord);
}

float findWordScore(char word[], struct words *wordList, int numWords) {
    for (int i = 0; i < numWords; i++) {
        if (strcmp(word, wordList[i].word) == 0) {
            return wordList[i].score;
        }
    }

    // return 0.0 if word is not found in list
    return 0.0;
}

void analyzeString(struct sentences *sentenceList, int numSentences, struct words *wordList, int numWords) {
    for (int i = 0; i < numSentences; i++) {
        char sentence[1000];
        strcpy(sentence, sentenceList[i].comment);
        int sentenceLen = 0;
        float sentenceScore = 0;

        char *word = strtok(sentence," \t\n");
        while (word != NULL) {
            sentenceLen++;
            char newWord[strlen(word) + 1];
            removePunctuation(word, newWord); 

            float wordScore = findWordScore(newWord, wordList, numWords);
            //printf("%f ",wordScore);
            sentenceScore += wordScore;

            word = strtok(NULL, " \t\n");
        }
        //printf("\n");
        sentenceList[i].avgScore = sentenceScore / sentenceLen;
    }


}