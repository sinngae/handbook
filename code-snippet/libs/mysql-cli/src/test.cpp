/*************************************************************************
	> File Name: test.cpp
	> Author: 
	> Mail: 
	> Created Time: Thu 04 Jan 2018 04:58:35 AM EST
 ************************************************************************/

#include <iostream>
#include <string>

#include <mysql.h>

using namespace std;

int main(int argc, char *argv[])
{
    MYSQL mysql;
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql, "localhost", "root", "root1234", "run", 3306, NULL, 0))
    {
        cout << "failed to connect to database, error: " << mysql_error(&mysql) << endl;
    }

    cout << "connect to database" << endl;

    string sql = "select * from test";
    if (mysql_real_query(&mysql, sql.c_str(), sql.length()))
    {
        cout << "failed to query " << sql.c_str() << endl;
    }
    else
    {
        cout << "query " << endl;
        MYSQL_RES *res = mysql_store_result(&mysql);
        if (res)
        {
            int num_fields = mysql_num_fields(res);
            int num_rows = mysql_num_rows(res);
            cout << "rows: " << num_rows << " fields: " << num_fields << endl;

            MYSQL_FIELD *fields;
            fields = mysql_fetch_fields(res);
            for (int i = 0; i < num_fields; i++)
            {
                cout << fields[i].name << "\t";
            }
            cout << endl;

            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)))
            {
                //unsigned long *lengths = mysql_fetch_lengths(res);
                for (int i = 0; i < num_fields; i++)
                {
                    cout << (row[i]?row[i]:"NULL") << "\t";
                }
                cout << endl;
            }
            mysql_free_result(res);
        }
        else
        {
            int ret = mysql_field_count(&mysql);
            if (ret == 0)
            {
                int ret = mysql_affected_rows(&mysql);
                cout << ret << " rows affected " << endl;
            }
            else
            {
                cout << "error: " << mysql_error(&mysql);
            }
        }
    }
    mysql_close(&mysql);

    return 0;
}
