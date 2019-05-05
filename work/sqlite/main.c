#include <assert.h>

#include "sqlitedb.h"

int main()
{
    assert(open_db("test.db"));

    assert(print_test() == SQLITE_OK);

    assert(backup_db(open_db("test.db"), "testbk.db", default_backup_cb) == SQLITE_OK);
}
