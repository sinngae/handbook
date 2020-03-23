#include <curl/curl.h>
#include <string.h>
/*
struct curl_http_args_st
{
    int data_len; // �ļ����ݱ������ڴ��еĳ���
    char *data;   // �ļ����ݱ������ڴ��е�ָ��, ������ֶ�free
};

size_t curl_write_data_cb(void *buffer, size_t size, size_t nmemb, void *stream)
{
    int len = size * nmemb;
    struct curl_http_args_st *args = (struct curl_http_args_st *)stream;

    if (stream)
    {
        args->data = realloc(args->data, args->data_len + len + 1); // �����һ���ֽ�, �Ա���\0
        if (!args->data)
        {
            XL_DEBUG(EN_PRINT_ERROR, "%s[%d]: realloc failed!!\n", __FUNCTION__, __LINE__);
            return 0;
        }
        memcpy(args->data + args->data_len, buffer, len);
        args->data_len += len;
    }

    return len;
}

int set_file_md5sum(const char *pathname, const char *cid, const char *gcid)
{
    //����curl����
    CURL *curl;
    CURLcode return_code;
    char url[1024] = "/0";
    int ret = -1;
    struct json_object *infor_object = NULL;
    struct json_object *result_object = NULL;
    int rtn = 0;
    int total = 0;

    struct curl_http_args_st args;
    memset(&args, 0x00, sizeof(args));

    sprintf(url, "http://127.0.0.1:8801/filedrawer?opt=setfilemd5&path=%s,md5sum=%s", pathname, cid, gcid);

    //curl��ʼ��
    curl = curl_easy_init();
    if (!curl)
    {
        XL_DEBUG(EN_PRINT_ERROR, "%s[%d]: curl easy init failed\n", __FUNCTION__, __LINE__);
        return ret;
    }

    curl_easy_setopt(curl, CURLOPT_HEADER, 0);                         //����httpheader ����, ����Ҫ��HTTPͷд����ص�����
    curl_easy_setopt(curl, CURLOPT_URL, url);                          //����Զ�˵�ַ
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);                       // TODO: �򿪵�����Ϣ
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);                 //��������302  ��ת
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_data_cb); //ִ��д���ļ������� �Ļص�����
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &args);                  // ���ûص������ĵ�4 ������
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);      //����Ϊipv4����
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);                //�������ӳ�ʱ����λs, CURLOPT_CONNECTTIMEOUT_MS ����
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);                        // ����CURL ִ�е�ʱ��, ��λ��, CURLOPT_TIMEOUT_MS����
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);                       //linux���߳����Ӧע�������(��ֹcurl��alarm�źŸ���)

    return_code = curl_easy_perform(curl);
    if (CURLE_OK != return_code)
    {
        XL_DEBUG(EN_PRINT_ERROR, "curl_easy_perform() failed: %s\n", curl_easy_strerror(return_code));
        ret = 0;
    }

    if (args.data) // ��Ҫ�Է��ص����ݽ��д���, ���ڴ˴���
    {
        XL_DEBUG(EN_PRINT_ERROR, "set_file_cid_gcid %s !", args.data);
        free(args.data);
        args.data = NULL;
    }

    curl_easy_cleanup(curl);

    return ret;
}
*/

int main()
{
    curl_global_init(CURL_GLOBAL_ALL);

    //set_file_cid_gcid("/media/sda1/xxx.mp4", "adfadsfasdfewwrqweref", "dafqwereqwerwe");

    CURL *curl = curl_easy_init();
    if (!curl) exit(-1);

    char data[] = "! #$%&'()+,-.;=@[]^_`{}~";
    char *data_encode = curl_easy_escape(curl, data, strlen(data));
    printf(data_encode);

    return 0;
}
