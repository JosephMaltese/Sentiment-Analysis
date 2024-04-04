#include "functions.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


void strToList(char *numStr, int *list, int lineNum) {
    // keeps track of the current integer value
    int currentDigit;
    // keeps track of position in array
    int count = 0;

    if (*numStr == '[') {
        numStr++;  
    }
    

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
        while ( *numStr != ']' && *numStr != ',') {
            numStr++;
        }

        // if a comma is reached, we move forward two elements in the string to the next integer so that the condition
        // of the loop will remain true and we can continue reading integers.
        if (*numStr == ',') {
            while (isalnum(*numStr) == 0) {
                numStr++;
            }
        }

    }
    if (count < 10) {
        printf("Incorrect formatting of list on line %d of vader_lexicon.txt file\n", (lineNum + 1));
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
        //printf("%s\n",line);
        
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
        // the word and score are separated by a tab, and so is the score and SD. However, the SD and SIS list are separated by a space or
        // a tab. Thus, the tab character is used as a delimeter for the first two tokens.
        char *currentWord = strtok(line, "\t");
        
        char *scoreStr = strtok(NULL, "\t");
        // tab and space is used as a delimeter here since a space or tab indicates the boundary between the SD and the SIS list
        char *sdStr = strtok(NULL, "\t ");
        // The newline character is used as a delimeter here since the SIS list is the last element on each line.
        char *currentSISstr = strtok(NULL, "\n");

        // If any of the token strings are null, then the line is not formatted correctly so the function skips
        // to the next iteration.
        if (currentWord == NULL || scoreStr == NULL || sdStr == NULL || currentSISstr == NULL) {
            printf("Line %d of %s file is incorrectly formatted\n", *wordCount, filename);
            continue;
        }

        // If atof() returns 0, this indicates that the given argument cannot be converted to a float. However, if
        // the argument is the string "0.0", atof also returns 0. Thus, this line checks if atof returns 0 and
        // the argument is not "0.0". If true, this indicates that the given argument is some invalid input
        // that cannot be converted to a float. In this case, the function skips to the next iteration.
        if ((atof(scoreStr) == 0 && strcmp(scoreStr, "0.0") != 0) || (atof(sdStr) == 0 && strcmp(sdStr, "0.0") != 0)) {
            printf("Line %d of %s file is incorrectly formatted.\n", *wordCount, filename);
            continue;
        }
        // atof is used to convert the string data to float to obtain the score and SD
        float currentScore = atof(scoreStr);
        float currentSD = atof(sdStr);

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
        strToList(currentSISstr, numArr, *wordCount);

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


// Function to read from the validation text file. This is the file which contains the given lines of text that
// you want to analyze. This function iterates through each line of the given file and stores each line in an 
// array of sentence structures. Each sentence structure holds each line of text, along with its average sentiment
// score, which is later calculated.
void readFromValFile(char filename[], struct sentences **sentenceListPtr, int capacity, int *sentenceCount) {
    // Open the given file for reading
    FILE *readFile = fopen(filename, "r");

    // Error check to ensure that the file has been opened correctly
    if (readFile == NULL) {
        printf("Cannot access given filename for reading.\n");
        return;
    }

    // define a string variable to hold the contents of the current line in the file
    char line[1000];

    // Iterate through each line of the text file. fgets() reads until either 1000 characters from the line
    // have been read or until the newline character is reached and stores the contents in the line variable.
    // When the line variable is null, the end of the file has been reached.
    while (fgets(line, 1000, readFile) != NULL) {
        // If the number of lines that have been read is greater than or equal to the number of lines that memory
        // has been allocated for, double the capacity and reallocate memory to hold the new capacity of sentences.
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

        // calculates the length of the current line in the file
        int len = strlen(line);
        // If the string isn't empty and the last character is a newline character, get rid of it by replacing
        // it with a null character
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }

        // Pointer notation is used to access the relevant element in the array of structures. The comment element of the
        // current structure is assigned a duplicate of a pointer to the current line. Memory is allocated 
        // by the strdup function automatically based on the line.
        ((*sentenceListPtr) + *sentenceCount)->comment = strdup(line);
        // Handles failed memory allocation
        if (((*sentenceListPtr) + *sentenceCount)->comment == NULL) {
            printf("Failed to allocate memory.\n");
            fclose(readFile);
            return;
        }
        // Keeps track of the number of sentences by modifying the value pointed to by sentenceCount
        *sentenceCount = *sentenceCount + 1;
    }
    // Closes the file when done reading.
    fclose(readFile);
}


// Function to remove the punctuation from the given word and convert all uppercase letters to lowercase. This is
// used for each word in the validation text file to clean up the words so that they can later be compared to the words
// in the array containing the words from the vader lexicon.
void removePunctuation(char *word, char *newWord) {
    // find the length of the given word
    size_t word_length = strlen(word);
    // Find the last index position in the word
    int k = word_length - 1;

    // Loop that is used in the removal of trailing punctuation from the word. Loops while it is not at the start of
    // the word, and while the current letter in the word is either a comma, exclamation mark, or period (indicating
    // that we are currently looping over trailing punctuation). Loop ends when we reach the start of the word or once
    // we reach the first character that is not trailing punctuation.
    while (k > 0 && (word[k] == ',' || word[k] == '!' || word[k] == '.')) {
        k--;
    }

    // Keeps track of the current position in the new word
    int j = 0;
    
    // Loops through each position in the given word until the last position before trailing punctuation. This ensures
    // that trailing punctuation is not copied into the new word.
    for (int i = 0; i <= k; i++) {
        // if the current character in the word is an uppercase letter, its lowercase form is added to the new
        // word instead.
        if (isalpha(word[i]) && isupper(word[i])) {
            newWord[j] = word[i] + 32;
        }
        // all other characters and punctuation within the given word is copied into the new word
        else {
            newWord[j] = word[i];            
        }
        // Every time a letter is added to the new word, we move to the next position in it.
        j++;
    }
    // terminate the new word with a null character.
    newWord[j] = '\0';
}

// Function to find the score of the given word by checking if the word is found in the wordList array. If it is
// found, its score is returned. Otherwise, 0.0 is returned.
float findWordScore(char word[], struct words *wordList, int numWords) {
    // Loops through each word structure in the wordList array
    for (int i = 0; i < numWords; i++) {
        // Compares the given word to the word element of the current word structure in the array. If they are
        // equal, this means the word has been found in the array, so it returns the word's score by accessing
        // the score element of the current word structure.
        if (strcmp(word, wordList[i].word) == 0) {
            return wordList[i].score;
        }
    }

    // return 0.0 if word is not found in list
    return 0.0;
}

// Function to loop through each sentence structure and analyze its comment string to find its sentiment score
// based on rule-based sentiment analysis. Each word in a sentence is looked up in the wordList array. If it is present
// in the array, its word score is obtained and added to a sum. Once done looping through the sentence, the overall
// sentiment score is obtained. Then, this score is divided by the number of words in the sentence to obtain the
// average score of the sentence.
void analyzeString(struct sentences *sentenceList, int numSentences, struct words *wordList, int numWords) {
    // Loops through all sentence structures in the array.
    for (int i = 0; i < numSentences; i++) {
        // Defines a string with maximum size of 1000 to hold the sentence from the sentence structure.
        char sentence[1000];
        // copies the sentence into the string variable.
        strcpy(sentence, sentenceList[i].comment);

        // initializes variables to track the length of the sentence and the overall sentiment score.
        int sentenceLen = 0;
        float sentenceScore = 0;

        // tokenizes the sentence using strtok with a space, a tab, or a newline character as the delimeter. The
        // first occurence of any of these indicates the end of the first token.
        char *word = strtok(sentence," \t\n");
        // Loops as long as the current token (word) is not null (end of sentence hasn't been reached)
        while (word != NULL) {
            // keeps track of sentence length
            sentenceLen++;

            // defines a string with the same length as the word token to hold the same word after removing
            // punctuation and converting all uppercase to lowercase.
            char newWord[strlen(word) + 1];
            removePunctuation(word, newWord); 

            // Looks up the word in the wordList found earlier and obtains the score of the word
            float wordScore = findWordScore(newWord, wordList, numWords);
            
            // adds the score of the word to the overall sentence score
            sentenceScore += wordScore;

            // moves to the next token in the sentence using the same delimeters.
            word = strtok(NULL, " \t\n");
        }
        // Adds the average score of the sentence to the sentence structure.
        sentenceList[i].avgScore = sentenceScore / sentenceLen;
    }


}