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
