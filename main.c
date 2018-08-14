/* 
 * CUBE-TIMER
 *
 * A simple and lightweight program to record, store, list and get statistics
 * of Rubik's Cube times
 *
 * Options:
 *          -a  Add a new time
 *          -c  Clear results
 *          -l  List all results
 *          -s  Write statistics
 */

#include <stdio.h>     /* printf, fgets, getchar, FILE */
#include <stdlib.h>    /* exit                         */
#include <sys/time.h>  /* gettimeofday                 */
#include <unistd.h>    /* sleep                        */

#define CHAR_SHIFT 48 
#define FILE_NAME  ".results.txt"
#define INSP_TIME  15
#define LINE_SIZE  11 

typedef unsigned int MILLSEC;

static void    add      ();
static void    clear    ();
static MILLSEC deformat (char *const);
static void    format   (MILLSEC, char*);
static MILLSEC get_time ();
static void    list     ();
static void    stat     ();

static FILE *f;

static void
add()
{
	MILLSEC st;  /* start time */
	char res[LINE_SIZE-1];

	printf("Inspection time (%d secs)\n",INSP_TIME);
	sleep(INSP_TIME);
	st = get_time();
	printf("Started\n");
	getchar();
	format(get_time()-st,res);
	printf("Result: %s\n",res);
	fprintf(f = fopen(FILE_NAME,"a"),"%s\n",res);
	fclose(f);
}

static void
clear()
{
	fclose(f = fopen(FILE_NAME,"w"));
}

static void
format(MILLSEC t, char *str)
{
	int sec  = t/1000;
	int frac = t%1000;
	int min  = sec/60;

	sec %= 60;

	str[0] = min<10 ? '0' : min/10+CHAR_SHIFT;
	str[1] = min<10 ? min+CHAR_SHIFT : min%10+CHAR_SHIFT;
	str[2] = ':';
	str[3] = sec<10 ? '0' : sec/10+CHAR_SHIFT;
	str[4] = sec<10 ? sec+CHAR_SHIFT : sec%10+CHAR_SHIFT;
	str[5] = '.';
	str[6] = frac/100 ? frac/100+CHAR_SHIFT : '0';
	str[7] = frac%100/10 ? frac%100/10+CHAR_SHIFT : '0';
	str[8] = frac%10 ? frac%10+CHAR_SHIFT : '0';
	str[9] = '\0';
}

static MILLSEC
deformat(char *const str)
{
	MILLSEC time = 0;

	time += (str[0]-CHAR_SHIFT) * 600000;
	time += (str[1]-CHAR_SHIFT) * 60000;
	time += (str[3]-CHAR_SHIFT) * 10000;
	time += (str[4]-CHAR_SHIFT) * 1000;
	time += (str[6]-CHAR_SHIFT) * 100;
	time += (str[7]-CHAR_SHIFT) * 10;
	time += (str[8]-CHAR_SHIFT);

	return time;
}


static MILLSEC
get_time()
{
	struct timeval tp;

	gettimeofday(&tp,NULL);
	return (MILLSEC) tp.tv_sec * 1000L + tp.tv_usec / 1000; 
}

static void
list()
{
	char line[LINE_SIZE];
	int  i = 0;

	f = fopen(FILE_NAME,"r");
	while (fgets(line,LINE_SIZE,f))
		printf("%d. %s",++i,line);
	fclose(f);
}

static void
stat()
{
	/* TODO: Add "Average of 5" and "Average of 12" */

	char    line[LINE_SIZE];
	MILLSEC time;
	MILLSEC sum   = 0;
	MILLSEC best  = 0;
	int     count = 0;

	f = fopen(FILE_NAME,"r");
	while (fgets(line,LINE_SIZE,f)) {
		time = deformat(line);
		best = time<best || !best ? time : best;
		sum += time;
		++count;
	}
	fclose(f);

	printf("Count:     %d\n",count);
	if (count) {
		format(best,line);
		printf("Best time: %s\n",line);
		format(sum/count,line);
		printf("Average:   %s\n",line);
	}
	else {
		printf("Best time: N/A\n");
		printf("Average:   N/A\n");
	}
}

int
main(int argc, char const *argv[])
{
	if (argc<2) {
		printf("Error: No argument given!\n");
		exit(1);
	}
	switch (argv[1][1]) {
	case 'a' : add();   break;
	case 'c' : clear(); break;
	case 'l' : list();  break;
	case 's' : stat();  break;
	default  : printf("Error: Illegal argument!\n");
		   exit(2);
	}
	return 0;
}

