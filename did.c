/***********************************************************************
* did - Simple command-line task logging
* Copyright (c) 2013 Tim Park. Licensed under the MIT License
* "did -h" - Show usage (see printUsage function)
***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define FILENAME "did.txt"
#define VERSION "1.1"
#define ON '@'
#define OFF '-'
#define BORDER '#'
#define MAXLINE 128
#define NUMDATES 70
#define DATESIZE 11

struct Task {
	struct Task *next;
	struct Task *prev;
	char task[MAXLINE];
	char done[NUMDATES + 1];
	int count[NUMDATES];
} head, tail;

//*** Print how to use the program and the current default filename.
void printUsage(char *cmd)
{
	printf("did v%s - Simple command-line task logging\n\n", VERSION);
	printf("%s               : Show report\n", cmd);
	printf("%s -h            : Show usage\n", cmd);
	printf("%s -d YYYY/MM/DD : Show report ending at date\n", cmd);
	printf("%s task          : Log task as done today\n", cmd);
	printf("%s task yest     : Log task as done yesterday\n", cmd);
	printf("     Default file : %s\n", FILENAME);
}

//*** Print error and exit. Don't really need variable args for our use.
void errorExit(char *error, char *arg)
{
	struct Task *task, *currentTask;
	for (task = head.next; task != &tail; task = currentTask)
	{
		currentTask = task->next;
		free(task);
	}
	fprintf(stderr, error, arg);
	exit(1);
}

//*** Get character to print for count.
char countChar(int count)
{
	return (count > 35) ? '+' :
		(count > 9) ? ((count - 10) + 'A') :
		(count > 1) ? ((count - 2) + '2') :
		(count == 1) ? ON : OFF;
}

//*** Return the day of the week. (0 = Sunday)
int dayOfWeek(int y, int m, int d)
{
	struct tm tm1 = {0,0,0,0,0,0,0,0,0};
	tm1.tm_year = y - 1900;
	tm1.tm_mon = m - 1;
	tm1.tm_mday = d;
	tm1.tm_hour = tm1.tm_min = tm1.tm_sec = 1;
	mktime(&tm1);
	return tm1.tm_wday;
}

//*** Convert "YYYY/MM/DD" to time_t. (seconds) Return false on invalid date.
bool stringToSecs(char *buf, time_t *time1)
{
	struct tm tm1 = {0,0,0,0,0,0,0,0,0};
	char days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
//Example log entry:
//2013/03/11 stuff

	if ((buf[4] != '/') || (buf[7] != '/'))
		return false;

	// Convert and validate date
	sscanf(buf, "%d/%d/%d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday);
	if ((tm1.tm_year % 400 == 0) || ((tm1.tm_year % 4 == 0) && (tm1.tm_year % 100 != 0)))
		days[1] = 29; // Adjust for leap year
	tm1.tm_year -= 1900;
	tm1.tm_mon--;
	if ((tm1.tm_year < 0) ||
		(tm1.tm_mon < 0) || (tm1.tm_mon > 11) ||
		(tm1.tm_mday < 1) || (tm1.tm_mday > days[tm1.tm_mon]))
		return false;

	tm1.tm_hour = tm1.tm_min = tm1.tm_sec = 1;
	tm1.tm_isdst = -1; // unknown DST status

	*time1 = mktime(&tm1);
	if (*time1 == -1)
		return false;
	return true;
}

//*** Generate a report of all tasks
void printReport(char *date, char *cmd)
{
	struct Task *task, *currentTask;
	FILE *file;
	time_t time1;
	struct tm *tm1;
	int i, j, y, m, d, total;
	static char days[8] = "SMTWTFS";
	char *c, line[MAXLINE], buf[MAXLINE], dates[NUMDATES][DATESIZE];

	printf("did - Simple command-line task logging (\"%s -h\" for usage)\n\n", cmd);

	// Create date strings for comparison
	stringToSecs(date, &time1);
	for (i = NUMDATES - 1; i >= 0; i--)
	{
		tm1 = localtime(&time1);
		strftime(dates[i], DATESIZE, "%Y/%m/%d", tm1);
		time1 -= (time_t)(24*60*60);
	}

	// Read in tasks and see if they match date range
	file = fopen(FILENAME, "r");
	if (file == NULL)
	{
		// Create empty file
		printf("Creating file: %s\n", FILENAME);
		file = fopen(FILENAME, "w");
		if (file == NULL)
			errorExit("Can't open file for writing: %s\n", FILENAME);
		fclose(file);
		return;
	}
	while (fgets(line, MAXLINE, file) != NULL) {
		// Strip trailing whitespace
		c = line + strlen(line) - 1;
		while ((c > line) && (isspace(*c)))
			c--;
		*(c + 1) = 0;

		// Skip leading whitespace
		c = line;
		while (isspace(*c))
			c++;

		// Skip if line is blank
		if (*c == 0)
			continue;

		// Check if date matches desired range
		c[10] = 0;
		for (i = 0; i < NUMDATES; i++)
			if (strcmp(dates[i], c) == 0)
				break;
		if (i == NUMDATES)
			continue;

		// Check if task is in list
		for (task = head.next; task != &tail; task = task->next)
			if (strcmp(task->task, &c[11]) == 0)
				break;

		// Add task to list
		if (task == &tail)
		{
			// Insert task alphabetically into the list
			for (task = head.next; task != &tail; task = task->next)
				if (strcmp(task->task, &c[11]) > 0)
					break;
			currentTask = calloc(1, sizeof(struct Task));
			if (currentTask == NULL)
			{
				fclose(file);
				errorExit("Couldn't allocate memory\n", "");
			}
			currentTask->prev = task->prev;
			currentTask->next = task->prev->next;
			task->prev->next = currentTask;
			task->prev = currentTask;

			strcpy(currentTask->task, &c[11]);
			for (j = 0; j < NUMDATES; j++) {
				currentTask->count[j] = 0;
				currentTask->done[j] = countChar(currentTask->count[j]);
			}
			currentTask->done[NUMDATES] = 0;
			task = currentTask;
		}
		task->count[i]++;
		task->done[i] = countChar(task->count[i]);
	}
	fclose(file);

	// Print top border
	printf("      ");
	for (i = 0; i < NUMDATES + 2; i++)
		printf("%c", BORDER);
	printf("\n");

	// Print tasks
	for (task = head.next; task != &tail; task = task->next)
	{
		total = 0;
		for (i = 0; i < NUMDATES; i++)
			total += task->count[i];
		task->task[5] = 0;
		printf("%5s %c%s%c %s (%d)\n", task->task, BORDER, task->done, BORDER, task->task, total);
	}

	// Print bottom border
	printf("      ");
	for (i = 0; i < NUMDATES + 2; i++)
		printf("%c", BORDER);
	printf(" (%d)\n", NUMDATES);

	// Print bottom labels - year, first digit of day
	strcpy(buf, dates[NUMDATES - 1]);
	buf[4] = 0;
	printf("%5s  ", buf);
	for (i = 0; i < NUMDATES; i++)
		printf("%c", dates[i][8]);
	printf("\n");

	// Print bottom labels - months, second digit of day
	strcpy(buf, &dates[0][5]);
	buf[2] = '-';
	strcpy(&buf[3], &dates[NUMDATES - 1][5]);
	buf[5] = 0;
	printf("%5s  ", buf);
	for (i = 0; i < NUMDATES; i++)
		printf("%c", dates[i][9]);
	printf("\n");

	// Print bottom labels - weekday
	printf("       ");
	for (j = 0; j < NUMDATES; j++)
	{
		sscanf(dates[j], "%d/%d/%d", &y, &m, &d);
		printf("%c", days[dayOfWeek(y,m,d)]);
	}
	printf("\n");
}

int main(int argc, char **argv)
{
	struct Task *task, *currentTask;
	FILE *file;
	time_t time1;
	struct tm *tm1;
	char now[DATESIZE];

	head.next = &tail;
	tail.prev = &head;

	if (time(&time1) == -1)
		errorExit("Couldn't get current time\n", "");
	if ((argc > 2) && (strncmp(argv[2], "yest", 4) == 0))
		time1 -= (time_t)(24*60*60);
	tm1 = localtime(&time1);
	strftime(now, DATESIZE, "%Y/%m/%d", tm1);

	if (argc <= 1)
		printReport(now, argv[0]);
	else if ((argc >= 2) && (strcmp(argv[1], "-h") == 0))
		printUsage(argv[0]);
	else if ((argc >= 2) && (strcmp(argv[1], "-d") == 0))
	{
		if (argc == 2)
			errorExit("No date given", "");
		else if (!stringToSecs(argv[2], &time1))
			errorExit("Invalid date", "");
		else
			printReport(argv[2], argv[0]);
	}
	else
	{
		file = fopen(FILENAME, "a");
		if (file == NULL)
			errorExit("Can't open file for appending: %s\n", FILENAME);
		fprintf(file, "%s %s\n", now, argv[1]);
		fclose(file);
		printf("\"%s %s\" added\n", now, argv[1]);
	}

	for (task = head.next; task != &tail; task = currentTask)
	{
		currentTask = task->next;
		free(task);
	}
	return 0;
}
