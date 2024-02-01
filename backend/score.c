#include "score.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static struct Score defaultLeaderboard[MAX_SCORERS] = 
{
	{" ", 0, 0},
	{" ", 0, 0},
	{" ", 0, 0},
	{" ", 0, 0},
	{" ", 0, 0},
	{" ", 0, 0},
	{" ", 0, 0},
	{" ", 0, 0},
	{" ", 0, 0},
	{" ", 0, 0}
};

static struct Score leaderboard[MAX_SCORERS];

// https://harddrop.com/wiki/Scoring
int howMuchScore(int level,  int lines)
{
	if (lines == 1)
		return 4 * (level + 1);
	
	if (lines == 2)
		return 10 * (level + 1);

	if (lines == 3)
		return 30 * (level + 1);

	if (lines == 4)
		return 120 * (level + 1);

	else
		return 0;
}

static void writeScores(const char *filename, struct Score *scores, size_t numScores)
{
    FILE *file = fopen(filename, "wb");  // Open file for writing in binary mode
    if (file == NULL)
	{
        perror("Error opening file for writing");
        return;
    }

    // Write the array of scores to the file
    size_t elements_written = fwrite(scores, sizeof(struct Score), numScores, file);
    if (elements_written != numScores) 
	{
        perror("Error writing to file");
    }

    fclose(file);  
}

static void readScores(const char *filename, struct Score *scores, size_t numScores) 
{
    FILE *file = fopen(filename, "rb");  // Open file for reading in binary mode
    if (file == NULL) 
	{
        printf("\nLegendary scroll of all time best not found, creating new one...\n");
		writeScores(filename, defaultLeaderboard, MAX_SCORERS);
        return;
    }

    // Read the array of scores from the file
    size_t elements_read = fread(scores, sizeof(struct Score), numScores, file);
    if (elements_read != numScores) 
	{
        perror("Error reading from file");
    }

    fclose(file);  
}

// descending order
static int compareScores(const void *a, const void *b)
{
    return ((struct Score *)b)->score - ((struct Score *)a)->score;
}

void updateLeaderboard(const char *filename, int currentScore, int currentLevel, 
					char *currentName)
{
	readScores(filename, leaderboard, MAX_SCORERS);

	for (int i = 0; i < MAX_SCORERS; i++)
	{
		if (currentScore > leaderboard[i].score)	
		{
			// change the current players data with the last on the top score list			
			leaderboard[MAX_SCORERS - 1].score = currentScore;
			leaderboard[MAX_SCORERS - 1].level = currentLevel;
			strcpy(leaderboard[MAX_SCORERS - 1].name, currentName); 

			// sort the top score in descending order
			qsort(leaderboard, MAX_SCORERS, sizeof(struct Score), compareScores);

			// write the updated top score list to the local file
			writeScores(filename, leaderboard, MAX_SCORERS);
			return;
		}
	}
}

struct Score *getLeaderboard(int i)
{
	if (i >= MAX_SCORERS)
		fprintf(stderr, "Index in getLeaderboard out of reach.\n");
	return &leaderboard[i];
}


