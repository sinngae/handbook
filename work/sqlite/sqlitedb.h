/************************************************
  > File Name: sqlitedb.h
  > Author: Philo
  > Mail: 
  > Created Time: Tue 30 Apr 2019 06:01:13 PM CST
******************************************/
#include <sqlite3.h>

sqlite3* open_db(const char *filename);

void default_backup_cb(int remaining, int pagecount);

int backup_db(sqlite3 *pdb, const char *filename, void (*backup_db)(int, int));

int print_test();
