#include "score.h"

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

void writeScores(const char *filename, struct Score *scores, size_t numScores)
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

    fclose(file);  // Close the file
}

void readScores(const char *filename, struct Score *scores, size_t numScores) 
{
    FILE *file = fopen(filename, "rb");  // Open file for reading in binary mode
    if (file == NULL) 
	{
        perror("Error opening file for reading");
        return;
    }

    // Read the array of scores from the file
    size_t elements_read = fread(scores, sizeof(struct Score), numScores, file);
    if (elements_read != numScores) 
	{
        perror("Error reading from file");
    }

    fclose(file);  // Close the file
}
