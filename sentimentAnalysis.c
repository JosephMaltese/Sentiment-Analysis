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



    // for (int i = 0; i< wordCount;i++) {
    //     printf("word: %s, score: %.4f, sd: %.4f, list: [%d %d %d %d %d %d %d %d %d %d]\n", wordList[i].word, wordList[i]. score, wordList[i].SD, wordList[i].SIS_array[0],wordList[i].SIS_array[1],wordList[i].SIS_array[2],wordList[i].SIS_array[3],wordList[i].SIS_array[4],wordList[i].SIS_array[5],wordList[i].SIS_array[6],wordList[i].SIS_array[7],wordList[i].SIS_array[8],wordList[i].SIS_array[9]);
    // }





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