/************************************************
  > File Name: sqlitedb.h
  > Author: Philo
  > Mail: 
  > Created Time: Tue 30 Apr 2019 06:01:13 PM CST
******************************************/
#include <sqlite3.h>

typedef struct st_sqlitedb sqlitedb;

sqlitedb *sqlitedb_alloc(const char *filename);

int sqlitedb_free(sqlitedb *db);

int backup_db(sqlitedb *db, const char *filename, void (*backup_db)(int, int));

int print_test();
