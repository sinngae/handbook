# nginx handler
nginx 插件开发
多个handler模块关联同一个location，handler链（只有一个起作用？）

第三方开发者可以开发handler/filter/load-balancer
结果：成功，失败，拒绝

## 模块的数据结构
三个作用域：main/server/location

每个模块定义三个数据结构去存储

模块提供配置指令，用于控制该模块行为（配置文件）

模块配置命名惯例：ngx_http_[module_name]_[main/srv/loc]_conf_t
```c
typedef struct {
    ngx_str_t hello_string;
    ngx_int_t hello_counter;
} ngx_http_hello_loc_conf_t; // 配置信息结构

static ngx_command_t ngx_http_hello_commands[] = {
    {
        ngx_string("hello"),
        NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS | NGX_CONF_TAKE1,
        ngx_http_hello_string,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hello_loc_conf_t, hello_string),
        NULL
    },
    {
        ngx_string("hello_counter"),
        NGX_HTTP_LOC_CONF | NGX_CONF_FLAG,
        ngx_http_hello_counter,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hello_loc_conf, hello_conter),
        NULL
    },
    ngx_null_command    // 配置指令定义变量
};

// nginx_command_t define
struct ngx_command_t {
    ngx_str_t   name;   // 指令名
    ngx_uint_t  type;   // 配置类型
    /*
        set函数：nginx解析配置时，遇到该指令，把读到值传给该函数处理
    */
    char *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);   // 函数指针
    ngx_uint_t  conf;   // 类型
    ngx_uint_t  offset; // 偏移
    void        *post;
};

```