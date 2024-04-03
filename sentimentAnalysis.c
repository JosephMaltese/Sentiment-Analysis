#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int wordCount = 0;
    int *ptrToWordCount = &wordCount;
    int CAPACITY = 1000;
    struct words *wordList = (struct words *)malloc(CAPACITY * sizeof(struct words));
    if (wordList == NULL)
    {
        printf("Memory allocation failed!\n");
        return 0;
    }

    readFromLexiconFile("vader_lexicon.txt", &wordList, CAPACITY, ptrToWordCount);


    int sentenceCount = 0;
    int sentenceCapacity = 10;
    int *ptrToSentenceCount = &sentenceCount;
    struct sentences *sentenceList = (struct sentences *)malloc(sentenceCapacity*sizeof(struct sentences));
    readFromValFile("validation.txt", &sentenceList, sentenceCapacity, ptrToSentenceCount);

    analyzeString(sentenceList, sentenceCount, wordList, wordCount);

    printf("               string sample                                                               score\n");
    printf("---------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i< sentenceCount;i++) {
        if (sentenceList[i].avgScore < 0) {
            printf("%-89s", sentenceList[i].comment);
            printf("%f\n", sentenceList[i].avgScore);
        }
        else {
            printf("%-90s", sentenceList[i].comment);
            printf("%f\n", sentenceList[i].avgScore);
        }   
    }


    

    for (int i = 0; i < wordCount; i++)
    {
        free(wordList[i].word);
    }
    for (int i = 0; i < sentenceCount; i++) {
        free(sentenceList[i].comment);
    }
    free(wordList);
    free(sentenceList);
}