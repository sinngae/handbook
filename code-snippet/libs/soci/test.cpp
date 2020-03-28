/*************************************************************************
	> File Name: test.cpp
	> Author: renzq
	> Mail: zqren@foxmail.com
	> Created Time: Fri 13 Apr 2018 11:28:38 AM HKT
 ************************************************************************/

#include <iostream>
#include <string.h>
#include <sstream>

#include <soci.h>
#include <mysql/soci-mysql.h>

using namespace std;

int main(int argc, char *argv[])
{
    ostringstream oss;
    oss << "v1.0.0.1  build@" << __TIME__<< " " << __DATE__;
    string version = oss.str();
    if (argc > 1 && strcmp(argv[1], "-v") == 0)
    {
        cout << argv[0] << "\t" << version.c_str() << endl;
        return 0;
    }

    string connect_str = "host=localhost db=run user=root password=root1234";
    try
    {
        soci::session session("mysql", connect_str);
        soci::transaction tran(session);
        
        string sql = "select * from test";

        typedef soci::rowset<soci::row> row_set;
        row_set result = session.prepare<<sql;
        for (row_set::iterator iter = result.begin();
            iter != result.end(); ++iter)
        {
            cout << iter->get<int>("id") << " | ";
            if (iter->get_indicator("name") == soci::i_ok)
            {
                cout << iter->get<string>("name") << " | ";
            }
            cout << endl;
        }

        tran.commit();
    }
    catch (soci::soci_error &e)
    {
        cout << "soci_error: " << e.what() << endl;
    }

    return 0;
}
