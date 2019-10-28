/************************************************
  > File Name: uniform_ecode.h
  > Author: Philo
  > Mail: 
  > Created Time: Wed 23 Oct 2019 11:09:20 AM CST
*************************************************/
#ifndef UNIFORM_ECODE_H
#define UNIFORM_ECODE_H
#include <stddef.h>

typedef enum en_uniform_error_code_t
{
    UNE_OK = 0,
    UNE_UNKNOWN,            // unknown
    UNE_NOTSUPPORT,         // not supported
    UNE_NOSERVLET,          // no servlet param
    UNE_NOPARAM,            // in params, lack some key-value
    UNE_NOPOSTDATA,         // no post data in POST request
    UNE_MAX
} uniform_ecode_t;

/* @desc:   convert into uniform error code 
 * @param:  code -
 * @return: uniform error code
 */
size_t uniform_ecode(uniform_ecode_t code);

/* @desc:   convert into uniform error code info 
 * @param:  code - 
 * @return: uniform error code information
 */
const char *uniform_ecode_info(uniform_ecode_t code);

#endif // end of UNIFORM_ECODE_H
