#include "uniform_ecode.h"

typedef struct s_uniform_ecode_info_t
{
    size_t code;
    const char *info;
} uniform_ecode_info_t;

const uniform_ecode_info_t g_uniform_ecode_info[] =
{
    [UNE_OK]            = {.code = 0,       .info = "OK"},
    [UNE_UNKNOWN]       = {.code = 1,       .info = "Unknown"},
    [UNE_NOTSUPPORT]    = {.code = 2,       .info = "Not Support"},
    [UNE_NOSERVLET]     = {.code = 100,     .info = "No Servlet Param"},
    [UNE_NOPARAM]       = {.code = 101,     .info = "LACK Some Param"},
    [UNE_NOPOSTDATA]    = {.code = 102,     .info = "No Post Data"},
};

size_t uniform_ecode(uniform_ecode_t code)
{
    return g_uniform_ecode_info[code].code;
}

const char *uniform_ecode_info(uniform_ecode_t code)
{
    return g_uniform_ecode_info[code].info;
}
