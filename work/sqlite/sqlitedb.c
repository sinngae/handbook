#include <stdio.h>

#include "sqlitedb.h"

sqlite3 *open_db(const char *filename)
{
    int rc;

    sqlite3 *p_file = NULL;
    rc = sqlite3_open(filename, &p_file);
    if (rc != SQLITE_OK)
    {
        printf("open %s failed: %s\n", filename);
    }

    return p_file;
}

void default_backup_cb(int remaining, int pagecount)
{
    printf("backuping [remaining:%d, pagecount:%d]\n", remaining, pagecount);
}

int backup_db(sqlite3 *pdb, const char *filename, void (*backup_cb)(int, int))
{
    int rc;
    sqlite3 *p_file = NULL;
    rc = sqlite3_open(filename, &p_file);
    if (rc != SQLITE_OK)
    {
        printf("open %s failed: %s\n", filename);
        goto __end;
    }

    sqlite3_backup *p_backup = NULL;
    p_backup = sqlite3_backup_init(p_file, "main", pdb, "main");
    if (! p_backup)
    {
        printf("sqlite3_backup_init %s failed: %s\n", filename);
        goto __end;
    }

    do
    {
        rc = sqlite3_backup_step(p_backup, 5);
        backup_cb(sqlite3_backup_remaining(p_backup), sqlite3_backup_pagecount(p_backup));
        if (rc == SQLITE_OK || rc == SQLITE_BUSY || rc == SQLITE_LOCKED)
        {
            sqlite3_sleep(250);
        }
        else
        {
            printf("backup ret=%d\n", rc);
        }
    }
    while (rc == SQLITE_OK || rc == SQLITE_BUSY || rc == SQLITE_LOCKED);

    (void)sqlite3_backup_finish(p_backup);
    rc = sqlite3_errcode(p_file);

__end:
    if (p_file)
      sqlite3_close(p_file);
    return rc;
}

int print_test()
{
	int rc = 0;
	
	sqlite3 *p_file = NULL;
	rc = sqlite3_open("test.db", &p_file);
	if (rc != SQLITE_OK)
	{
		printf("open test.db failed: %s\n", sqlite3_errmsg(p_file));
		goto __end;;
	}

	char *errmsg;
	char *sqlstr = "select * from test";
	char **p_result;
	int nrow;
	int ncol;
	rc = sqlite3_get_table(p_file, sqlstr, &p_result, &nrow, &ncol, &errmsg);
	if (rc != SQLITE_OK)
	{
		printf("sql exec failed: %s\n", errmsg);
		goto __end;
	}
	printf("sql exec nrow=%d, ncol=%d\n", nrow, ncol);

	int index, i, j;
	index = 0;
	for (i=0; i<=nrow; i++)
	{
		for (j=0; j<ncol; j++)
		{
			if (p_result[index] == 0)
				printf("there is a null");
			printf("%s\t", p_result[index]);
			++index;
		}
		printf("\n");
	}

__end:
    if (p_result)
      sqlite3_free_table(p_result);
    if (p_file)
      sqlite3_close(p_file);
	return rc;
}
