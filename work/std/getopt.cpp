/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: Sun 04 Feb 2018 08:18:46 PM EST
 ************************************************************************/

#include <iostream>
#include <string.h>
#include <sstream>

#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
    /*ostringstream oss;
    oss << "v1.0.0.1  build@" << __TIME__<< " " << __DATE__;
    string version = oss.str();
    if (argc > 1 && strcmp(argv[1], "-v") == 0)
    {
        cout << argv[0] << "\t" << version.c_str() << endl;
        return 0;
    }*/

    char ch = '\0';
    //opterr = 0;
    while (true)
    {
        cout << "----------------------------------------------\noptind:" << optind << endl;
        if ((ch = getopt(argc, argv, "a:bcde::")) == -1)
            break;
        cout << "ch:" << ch 
            << ", opterr:" << opterr << endl;
        switch (ch)
        {
            case 'a':
                cout << "option=a, optarg=" << optarg << endl;
                break;
            case 'b':
                cout << "option=b " << endl;
                break;
            case '?':
                cout << "Unknown option=?, optopt=" << optopt << endl;
                break;
            //default:
                //cout << "option=default, optopt=" << optopt << ", optarg=" << optarg << endl;
        }
    }

    return 0;
}
