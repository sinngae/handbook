/*************************************************************************
	> File Name: main.cpp
	> Author: renzq
	> Mail: zqren@foxmail.com
	> Created Time: Thu 10 May 2018 06:14:41 PM HKT
 ************************************************************************/

#include <iostream>
#include <string.h>
#include <sstream>

#include "log4cplus/logger.h"
#include "log4cplus/configurator.h"
#include "log4cplus/helpers/loglog.h"
#include "log4cplus/helpers/stringhelper.h"
#include "log4cplus/helpers/sleep.h"
#include "log4cplus/helpers/fileinfo.h"
#include "log4cplus/loggingmacros.h"

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

static Logger s_logger = Logger::getInstance(LOG4CPLUS_TEXT("marka"));

void print_msg(Logger &logger)
{
    LOG4CPLUS_TRACE_METHOD(s_logger, LOG4CPLUS_TEXT("this is a trace log "));
    LOG4CPLUS_DEBUG(s_logger, "this is a debug log");
    LOG4CPLUS_INFO(s_logger, "this is a info log");
    LOG4CPLUS_WARN(s_logger, "this is a warn log");
    LOG4CPLUS_ERROR(s_logger, "this is a error log");
}

log4cplus::tstring getPropertiesFileArgument(int argc, char *argv[])
{
    if (argc >= 2)
    {
        char const *arg = argv[1];
        log4cplus::tstring file = LOG4CPLUS_C_STR_TO_TSTRING(arg);
        log4cplus::helpers::FileInfo fi;
        if (getFileInfo(&fi, file) == 0)
            return file;
    }
    return LOG4CPLUS_TEXT("log4cplus.properties");
}

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

    /*
    log4cplus::initialize();
    LogLog::getLogLog()->setInternalDebuggin(true);
    Logger root = Logger::getRoot();
    try
    {
        ConfigureAndWatchThread configureThread(getPropertiesFileArgument(argc, argv), 5*1000);
        LOG4CPLUS_WARN(root, "Testing ...");

        for (int i = 0; i < 10; i++)
        {
            print_msg(logger);
            log4cplus::helpers::sleep(1);
        }
    }
    catch(...)
    {
        cout << "catch exception" << endl;
        LOG4CPLUS_FATAL(root, "exception occured ...");
    }
    cout << "end ... " << endl;
    */

    PropertyConfigurator::doConfigure("log4cplus.properties");
    while (true)
        print_msg(s_logger);

    return 0;
}
