#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

/*Constants for MYSQL connection*/
#define MAX_QUERY 512
#define HOSTNAME  "dursley.socs.uoguelph.ca"
#define USERNAME  "braham"
#define PASSWORD  "0932115"
#define DATABASE  "braham"

/*Function Prototypes*/
void error(char *msg, MYSQL *mysql);
void clrstr(char *buf);
void help();
void clear(MYSQL mysql);
void reset(MYSQL mysql);
void posts(MYSQL mysql);
void users(MYSQL mysql);
void streams(MYSQL mysql);
void checkArgs(int argc, char * argv[], int * clearFlag, int * resetFlag, int * postFlag, int * userFlag, int * streamFlag, int * helpFlag, MYSQL mysql);

int main(int argc, char * argv[])
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	char query[MAX_QUERY];
	int x;
	int i = 0;

	/*Connects to mysql server*/
	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "db");
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0))
	{
	   error("Could not connect to host.",&mysql);
	}

	/*Checks for valid number of arguments*/
	if (argc < 2)
	{
		printf("Requires atleast 1 argument!\n");
		exit(1);
	}

	/*Flags for arguments so arguments cant be used twice*/
	int clearFlag = 0;
	int resetFlag = 0;
	int postFlag = 0;
	int userFlag = 0;
	int streamFlag = 0;
	int helpFlag = 0;

	/*Depending on number of arguments calls checkArgs with different argc value*/
	if (argc >= 2)
	{
		checkArgs(1,argv,&clearFlag,&resetFlag,&postFlag,&userFlag,&streamFlag,&helpFlag,mysql);
	}
	if (argc >= 3)
	{
		checkArgs(2,argv,&clearFlag,&resetFlag,&postFlag,&userFlag,&streamFlag,&helpFlag,mysql);
	}
	if (argc >= 4)
	{
		checkArgs(3,argv,&clearFlag,&resetFlag,&postFlag,&userFlag,&streamFlag,&helpFlag,mysql);
	}
	if (argc >= 5)
	{
		checkArgs(4,argv,&clearFlag,&resetFlag,&postFlag,&userFlag,&streamFlag,&helpFlag,mysql);
	}
	if (argc >= 6)
	{
		checkArgs(5,argv,&clearFlag,&resetFlag,&postFlag,&userFlag,&streamFlag,&helpFlag,mysql);
	}
	if (argc >= 7)
	{
		checkArgs(1,argv,&clearFlag,&resetFlag,&postFlag,&userFlag,&streamFlag,&helpFlag,mysql);
	}

	/*Closes sql connection*/
	mysql_close(&mysql);

	return 0;
}

/*
	standard error clause.
*/
void error(char *msg, MYSQL *mysql)
{
	printf("%s\n%s\n",msg,mysql_error(mysql));
	exit(1);
}

/*
	clear the contents of an array of char's
*/
void clrstr(char *buf)
{
  buf[0] = '\0';
}

/*
	Displays usage for program
*/
void help()
{
	printf("----------Usage:----------\n./db <-clear> <-reset> <-posts> <-users> <-streams> <-help>\n");
	printf("Flags can be in any order\n");
	printf("-clear: Removes all of the posts, users, streams and any other information from the tables in the database.\n");
	printf("-reset: Deletes the tables from the database.\n");
	printf("-posts: Prints out all posts stored in the database.\n");
	printf("-users: Prints out all user names stored in the database.\n");
	printf("-streams: Prints out all stream names stored in the database.\n");
	printf("-help: Prints out this usage message\n");
}

/*
	Removes all data from tables
*/
void clear(MYSQL mysql)
{
	char query[MAX_QUERY];
	MYSQL_RES *res;
	MYSQL_ROW row;
	char tables[500][50];
	int i = 0;

	strcpy(query, "show tables");
	if(mysql_query(&mysql, query))
	{
	  error("Could not get tables list!",&mysql);
	}
	if (!(res = mysql_store_result(&mysql)))
	{
		error("failed store 1",&mysql);
	}
	while ((row = mysql_fetch_row(res)))
	{
		strcpy(tables[i++],row[0]);
	}

	clrstr(query);

	while (i > 0)
	{
		i--;
		char temp[256] = "";
		sprintf(temp,"truncate %s",tables[i]);
		strcpy(query, temp);
		if(mysql_query(&mysql, query))
		{
		  error("Could not get trucate table!",&mysql);
		}
	}
}

/*
	Removes all tables from database
*/
void reset(MYSQL mysql)
{
	char query[MAX_QUERY];
	MYSQL_RES *res;
	MYSQL_ROW row;
	char tables[500][50];
	int i = 0;

	strcpy(query, "show tables");
	if(mysql_query(&mysql, query))
	{
	  error("Could not get tables list!",&mysql);
	}
	if (!(res = mysql_store_result(&mysql)))
	{
		error("failed store 1",&mysql);
	}
	while ((row = mysql_fetch_row(res)))
	{
		strcpy(tables[i++],row[0]);
	}

	clrstr(query);

	while (i > 0)
	{
		i--;
		char temp[256] = "";
		sprintf(temp,"drop table %s",tables[i]);
		strcpy(query, temp);
		if(mysql_query(&mysql, query))
		{
		  error("Could not drop table!",&mysql);
		}
	}
}

/*
	Displays all posts from tables
*/
void posts(MYSQL mysql)
{
	char query[MAX_QUERY];
	MYSQL_RES *res;
	MYSQL_ROW row;
	char tables[500][50];
	int i = 0;

	strcpy(query, "show tables");
	if(mysql_query(&mysql, query))
	{
	  error("Could not get tables list!",&mysql);
	}
	if (!(res = mysql_store_result(&mysql)))
	{
		error("failed store 1",&mysql);
	}
	while ((row = mysql_fetch_row(res)))
	{
		/*Checks if table is a post table, rather then a user table or data table*/
		if (row[0][(strlen(row[0])-1)] == 'm')
		{
			strcpy(tables[i++],row[0]);
		}
	}

	clrstr(query);

	printf("----------All Posts----------\n");

	while (i > 0)
	{
		i--;
		char temp[256] = "";
		sprintf(temp,"select post from %s",tables[i]);
		strcpy(query, temp);
		if(mysql_query(&mysql, query))
		{
		  error("Could not select posts!",&mysql);
		}
		if (!(res = mysql_store_result(&mysql)))
		{
			error("failed store 1",&mysql);
		}
		printf("----------From %s----------\n", tables[i]);
		while ((row = mysql_fetch_row(res)))
		{
			printf("%s\n", row[0]);
		}
	}
}

/*
	Displays all users in tables
*/
void users(MYSQL mysql)
{
	char query[MAX_QUERY];
	MYSQL_RES *res;
	MYSQL_ROW row;
	char tables[500][50];
	int i = 0;

	strcpy(query, "show tables");
	if(mysql_query(&mysql, query))
	{
	  error("Could not get tables list!",&mysql);
	}
	if (!(res = mysql_store_result(&mysql)))
	{
		error("failed store 1",&mysql);
	}
	while ((row = mysql_fetch_row(res)))
	{
		if (row[0][(strlen(row[0])-1)] == 's')
		{
			strcpy(tables[i++],row[0]);
		}
	}

	clrstr(query);

	printf("----------All Usernames----------\n");

	while (i > 0)
	{
		i--;
		char temp[256] = "";
		sprintf(temp,"select username from %s",tables[i]);
		strcpy(query, temp);
		if(mysql_query(&mysql, query))
		{
		  error("Could not select usernames!",&mysql);
		}
		if (!(res = mysql_store_result(&mysql)))
		{
			error("failed store 1",&mysql);
		}
		printf("----------From %s----------\n", tables[i]);
		while ((row = mysql_fetch_row(res)))
		{
			printf("%s\n", row[0]);
		}
	}
}

/*
	Displays all tables
*/
void streams(MYSQL mysql)
{
	char query[MAX_QUERY];
	MYSQL_RES *res;
	MYSQL_ROW row;
	char tables[500][50];
	int i = 0;

	strcpy(query, "show tables");
	if(mysql_query(&mysql, query))
	{
	  error("Could not get tables list!",&mysql);
	}
	if (!(res = mysql_store_result(&mysql)))
	{
		error("failed store 1",&mysql);
	}
	printf("----------All Streams----------\n");
	while ((row = mysql_fetch_row(res)))
	{
		printf("%s\n", row[0]);
	}
}

/*
	Checks arguments for proper flags and calls associated functions above
*/
void checkArgs(int argc, char * argv[], int * clearFlag, int * resetFlag, int * postFlag, int * userFlag, int * streamFlag, int * helpFlag, MYSQL mysql)
{
	if (strcmp(argv[argc],"-clear") == 0 && *clearFlag == 0)
	{
		clear(mysql);
		*clearFlag = 1;
	}
	else if (strcmp(argv[argc],"-reset") == 0 && *resetFlag == 0)
	{
		reset(mysql);
		*resetFlag = 1;
	}
	else if (strcmp(argv[argc],"-posts") == 0 && *postFlag == 0)
	{
		posts(mysql);
		*postFlag = 1;
	}
	else if (strcmp(argv[argc],"-users") == 0 && *userFlag == 0)
	{
		users(mysql);
		*userFlag = 1;
	}
	else if (strcmp(argv[argc],"-streams") == 0 && *streamFlag == 0)
	{
		streams(mysql);
		*streamFlag = 1;
	}
	else if (strcmp(argv[argc],"-help") == 0 && *helpFlag == 0)
	{
		help();
		*helpFlag = 1;
	}
}
