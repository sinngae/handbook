#include <curl/curl.h>
#include <string.h>

struct curl_http_args_st
{
    int data_len; // 文件数据保存在内存中的长度
    char *data;   // 文件数据保存在内存中的指针, 用完后手动free
};

size_t curl_write_data_cb(void *buffer, size_t size, size_t nmemb, void *stream)
{
    int len = size * nmemb;
    struct curl_http_args_st *args = (struct curl_http_args_st *)stream;

    if (stream)
    {
        args->data = realloc(args->data, args->data_len + len + 1); // 多分配一个字节, 以保存\0
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

int set_file_cid_gcid(const char *pathname, const char *cid, const char *gcid)
{
    //创建curl对象
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

    sprintf(url, "http://127.0.0.1:8801/fdrawer?opt=setfilecidgcid&path=%s,cid=%s,gcid=%s", pathname, cid, gcid);

    //curl初始化
    curl = curl_easy_init();
    if (!curl)
    {
        XL_DEBUG(EN_PRINT_ERROR, "%s[%d]: curl easy init failed\n", __FUNCTION__, __LINE__);
        return ret;
    }

    curl_easy_setopt(curl, CURLOPT_HEADER, 0);                         //设置httpheader 解析, 不需要将HTTP头写传入回调函数
    curl_easy_setopt(curl, CURLOPT_URL, url);                          //设置远端地址
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);                       // TODO: 打开调试信息
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);                 //设置允许302  跳转
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_data_cb); //执行写入文件流操作 的回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &args);                  // 设置回调函数的第4 个参数
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);      //设置为ipv4类型
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);                //设置连接超时，单位s, CURLOPT_CONNECTTIMEOUT_MS 毫秒
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);                        // 整个CURL 执行的时间, 单位秒, CURLOPT_TIMEOUT_MS毫秒
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);                       //linux多线程情况应注意的设置(防止curl被alarm信号干扰)

    return_code = curl_easy_perform(curl);
    if (CURLE_OK != return_code)
    {
        XL_DEBUG(EN_PRINT_ERROR, "curl_easy_perform() failed: %s\n", curl_easy_strerror(return_code));
        ret = 0;
    }

    if (args.data) // 若要对返回的内容进行处理, 可在此处理
    {
        XL_DEBUG(EN_PRINT_ERROR, "set_file_cid_gcid %s !", args.data);
        free(args.data);
        args.data = NULL;
    }

    curl_easy_cleanup(curl);

    return ret;
}

int main()
{
    curl_global_init(CURL_GLOBAL_ALL);

    set_file_cid_gcid("/media/sda1/xxx.mp4", "adfadsfasdfewwrqweref", "dafqwereqwerwe");

    return 0;
}
