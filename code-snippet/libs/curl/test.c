#include <curl/curl.h>
#include <string.h>

int main()
{
    curl_global_init(CURL_GLOBAL_ALL);

    //set_file_cid_gcid("/media/sda1/xxx.mp4", "adfadsfasdfewwrqweref", "dafqwereqwerwe");

    CURL *curl = curl_easy_init();
    if (!curl) return -1;

    //char data[] = "! #$%&'()+,-.;=@[]^_`{}~";
    char data[] = "a+b";
    char *data_encode = curl_easy_escape(curl, data, strlen(data));
    printf(data_encode);

    return 0;
}
