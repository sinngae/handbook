/*************************************************************************
	> File Name: client.cpp
	> Author: 
	> Mail: 
	> Created Time: Mon 02 Apr 2018 06:08:41 AM EDT
 ************************************************************************/

#include <iostream>
#include <string.h>
#include <sstream>
#include <iostream>
#include <stdio.h>

using namespace std;

#include "mimetic/mimetic.h"
#include "curl/curl.h"

typedef struct email_user_data
{
    stringstream ss;
    size_t total;
    email_user_data()
    :total(0)
    {
    }
}mail_data;

static size_t curl_readdata_cb(void *ptr, size_t size, size_t nmemb, void *userp)
{
    //cout << "curl_readdata_cb.size: " << size << " nmemb: " << nmemb << endl;
    mail_data *pstream = (mail_data*)userp;
    if (pstream->ss.eof())
        return 0;

    size_t before = pstream->ss.tellg();
    pstream->ss.read((char*)ptr, size*nmemb);
    size_t after = pstream->ss.tellg();
    cout << "before: " << before << " after: " << after << endl;
    if (pstream->ss.eof())
    {
        cout << "ss.gcount: " << pstream->ss.gcount() << endl;
        return pstream->ss.gcount();
    }
    return after - before;
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

    int count = 10;
    if (argc > 1)
        count = atoi(argv[1]);
    cout << "count = " << count << endl;

    string content;
    content.append("<div>发射点发</div>\r\n");
    content.append("<table border=\"1\">");
    content.append("<tr><td><div>列A</div></td>"
                  "<td><div>列B</div></td>"
                  "<td><div>列C</div></td></tr>\r\n");

    for (int i = 0; i < count; i++)
    {
        content.append("<tr><td><div>大风roiud阿的江</div></td>");
        content.append("<td><div>李娜地方</div></td>");
        content.append("<td><div>大风roiud阿的江与i获利机会</div>");
        content.append("<div>阿双方i紧密结合分iuyiu</div>");
        content.append("<div>额度限制的产能</div></td></tr>\r\n");
    }
    content.append("</table>");

    mail_data mail;
    mimetic::MimeEntity mime;
    mime.header().from("renzq@foxmail.com");
    mime.header().to("renzq@foxmail.com");
    mime.header().subject("this is the title");
    mime.body().assign(content.c_str());
    mime.header().push_back(mimetic::Field("Content-Type:text/html;charset=UTF-8"));

    mail.ss << mime;
    mail.total = mail.ss.str().size();
    cout << "mail.total = " << mail.total << endl;

    CURL *curl = NULL;
    CURLcode res;
    struct curl_slist *recipients = NULL;
    curl = curl_easy_init();
    if (curl == NULL)
    {
        cout << " create curl failed" << endl;
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_USERNAME, "zqren@foxmail.com");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "12345678");

    curl_easy_setopt(curl, CURLOPT_URL, "smtp://www.foxmail.com:25");
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, "renzq@foxmail.com");
    recipients = curl_slist_append(recipients, "zqren@foxmail.com");
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    curl_easy_setopt(curl, CURLOPT_READFUNCTION, curl_readdata_cb);
    curl_easy_setopt(curl, CURLOPT_READDATA, &mail);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    
    res = curl_easy_perform(curl);
    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);
    if (res != CURLE_OK)
    {
        cout << "send email failed, res: " << res
            << " strerr: " << curl_easy_strerror(res) << endl;
    }
    else
    {
        cout << "send email ok" << endl;
    }

    return 0;
}
