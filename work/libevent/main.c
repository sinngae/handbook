#include <pthread.h>

#define XL_DEBUG(level, format, args...) \
    fprintf(stderr, "%s %d %s() -debug- " format"\n", __FILE__, __LINE__, __FUNCTION__, ##args)


/**
 * HTTP请求回调处理
 * @param[in]
 * @return        void
 * @ref
 * @see
 * @note
 */
static void
http_request_cb(struct evhttp_request *v_req, void *v_arg)
{
    job_t *job = NULL;
    int idx = 0;
    int ret = 0;
    struct evkeyvalq http_query = {0};
    const char *pReqPara = NULL;
    pthhandler_t *pthd = NULL;
    cJSON *pObj = NULL;
    char *pRsp = NULL;

    DO_FMGR_REPORT_INIT(v_req);

    if (NULL == v_req)
    {
        XL_DEBUG(EN_PRINT_ERROR, "para null ptr");
        return;
    }

    pObj = cJSON_CreateObject();
    if (NULL)
    {
        XL_DEBUG(EN_PRINT_ERROR, "json obj create fail");
        return;
    }

    // 解析请求uri
    ret = evhttp_parse_query(evhttp_request_get_uri(v_req), &http_query);
    if (ret)
    {
        XL_DEBUG(EN_PRINT_ERROR, "parse http request fail ret=%d", ret);
        cJSON_AddNumberToObject(pObj, "rtn", ECODE_FMGR_FAIL);
        goto RSP_OUT;
    }

    pReqPara = evhttp_find_header(&http_query, "opt");
    if (NULL == pReqPara)
    {
        XL_DEBUG(EN_PRINT_ERROR, "para opt not found");
        cJSON_AddNumberToObject(pObj, "rtn", ECODE_FMGR_FAIL);
        goto RSP_OUT;
    }

    for (idx = 0; g_fmgr_prot_list[idx].handler; idx++)
    {
        if (strcmp(g_fmgr_prot_list[idx].name, pReqPara) == 0)
        {
            if (PROT_IMM == g_fmgr_prot_list[idx].flag)
            {
                g_fmgr_prot_list[idx].handler((void *)v_req);
                break;
            }
            else
            {
                /* Create a job object and add it to the work queue. */
                if ((job = malloc(sizeof(*job))) == NULL)
                {
                    XL_DEBUG(EN_PRINT_ERROR, "malloc job fail");
                    break;
                }

                pthd = (pthhandler_t *)malloc(sizeof(pthhandler_t));
                if (NULL == pthd)
                {
                    XL_DEBUG(EN_PRINT_ERROR, "malloc pthhandler_t fail");

                    free(job);
                    break;
                }

                memset(pthd, 0, sizeof(pthhandler_t));
                pthd->handler = g_fmgr_prot_list[idx].handler;
                pthd->http = v_req;

                job->job_function = server_job_function;
                job->user_data = pthd;

                workqueue_add_job(&workqueue, job);

                break;
            }
        }
    }

    evhttp_clear_headers(&http_query);
    cJSON_Delete(pObj);

    return;

RSP_OUT:

    pRsp = cJSON_Print(pObj);
    if (NULL == pRsp)
    {
        XL_DEBUG(EN_PRINT_ERROR, "json print fail");
        goto DEL_OUT;
    }

    fmgr_http_rsp_send(v_req, pRsp);
    free(pRsp);

DEL_OUT:
    evhttp_clear_headers(&http_query);
    cJSON_Delete(pObj);

    return;
}

/**
 * HTTP上传文件请求数据包处理回调函数
 * @param[in]     req
 * @return        void
 * @ref
 * @see
 * @note
 */
void http_request_chunked_cb(struct evhttp_request *req, void *arg)
{
    fmgr_prot_upload_chunk_hndl(req);
    return;
}

/**
 * HTTP上传文件请求header处理回调函数
 * @param[in]     req
 * @return        int 返回<0将关闭当前HTTP连接
 * @ref
 * @see
 * @note
 */
int http_request_header_cb(struct evhttp_request *req, void *arg)
{
    return fmgr_prot_upload_hdr_hndl(req);
}

/** 为上传文件协议的http request设置回调
*/
void http_request_arrive_cb(struct evhttp_request *req, void *arg)
{
    if (NULL == req)
    {
        XL_DEBUG(EN_PRINT_ERROR, "evhttp req is null");
        return;
    }

    if (NULL == req->chunk_cb)
    {
        evhttp_request_set_chunked_cb(req, http_request_chunked_cb);
    }
    else
    {
        XL_DEBUG(EN_PRINT_WARN, "evhttp req->chunk_cb not null");
    }

    if (NULL == req->header_cb)
    {
        evhttp_request_set_header_cb(req, http_request_header_cb);
    }
    else
    {
        XL_DEBUG(EN_PRINT_WARN, "evhttp req->header_cb not null");
    }

    return;
}

/**
 * 接收处理nginx的HTTP协议请求
 * @param[in]
 * @return
 */
void fmgr_http_service_thread(void)
{
    struct event_base *base = NULL;
    struct evhttp *http = NULL;
    struct evhttp_bound_socket *handle = NULL;
    char sock_addr[256] = {0};
    int ret = 0;

    snprintf(sock_addr, sizeof(sock_addr), "%s", "/tmp/nginx/socket/fmgr.sock");

    /* 支持多线程 */
    ret = evthread_use_pthreads();
    if (ret)
    {
        XL_DEBUG(EN_PRINT_ERROR, "use multi-thread fail");
        return;
    }

    base = event_base_new();
    if (NULL == base)
    {
        XL_DEBUG(EN_PRINT_ERROR, "create event base fail");
        return;
    }

/*  // Initialize work queue.
    if (workqueue_init(&workqueue, NUM_THREADS))
    {
        XL_DEBUG(EN_PRINT_ERROR, "create work queue fail");
        workqueue_shutdown(&workqueue);
        return;
    }
    */

    /* Create a new evhttp object to handle requests. */
    http = evhttp_new(base);
    if (NULL == http)
    {
        XL_DEBUG(EN_PRINT_ERROR, "create event http fail");
        return;
    }

    /* 设置请求包的回调函数 */
    ret = evhttp_set_cb(http, "/fmgr", http_request_cb, NULL);
    if (ret)
    {
        XL_DEBUG(EN_PRINT_ERROR, "set callback fail");
        return;
    }

    /* 为http请求设置回调 */
    evhttp_set_req_arrive_cb("/fmgr", http_request_arrive_cb, NULL);

    handle = evhttp_bind_unix_socket(http, sock_addr, 1);
    if (NULL == handle)
    {
        XL_DEBUG(EN_PRINT_ERROR, "bind socket[%s] fail", sock_addr);
        return;
    }

    event_base_dispatch(base);

    if (http)
    {
        evhttp_free(http);
    }

    if (base)
    {
        event_base_free(base);
    }

    return;
}

int main(int argc, char *argv[])
{
    int retcode = 0;

    // 创建模块APP协议处理线程
    pthread_t http_thread = 0;
    retcode = pthread_create(&http_thread, NULL, (void *)fmgr_http_service_thread, NULL);
    if (retcode != 0)
    {
        XL_DEBUG(EN_PRINT_ERROR, "http service thread create fail!");
    }

    while(1)
    {
        sleep(1);
    }

    return 0;
}
