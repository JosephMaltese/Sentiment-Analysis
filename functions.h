#ifndef FUNCTIONS_H
#define FUNCTIONS_H

struct words
{
    char *word;
    float score;
    float SD;
    int SIS_array[10];
};

struct sentences 
{
    char* comment;
    float avgScore;

};

void readFromLexiconFile(char filename[], struct words **wordListPtr, int capacity, int *wordCount);

void strToList(char *numStr, int *list, int lineNum);

void readFromValFile(char filename[], struct sentences **sentenceListPtr, int capacity, int *sentenceCount);

void analyzeString(struct sentences *sentenceList, int numSentences, struct words *wordList, int numWords);

void findScoreOfWord(char *word, struct words *wordList, int numWords);

void removePunctuation(char *word, char *newWord);

float findWordScore(char word[], struct words *wordList, int numWords);

#endif /* FUNCTIONS_H */