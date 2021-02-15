include "shared.thrift"
namespace cpp myserver

enum FuncCode
{
    LOGIN = 1,            // 登录
    LOGOUT = 2,           // 登出
    REGISTER = 3,         // 注册
}

enum RetCode
{
    OK = 0,               // 成功
    UNKNOW_ERROR = 1,     // 未知错误
    MSG_ERROR = 2,        // 异常
    NODATA_ERROR = 100,   // 没有数据
}

struct Request
{
    1:i64 msg_id,          // message id
    2:i64 session_id,      // user id
    3:FuncCode func,      // function code
}

struct Response
{
    1:i64 msg_id,
    2:i64 session_id,
    3:FuncCode func,
    4:i32 ret_code = -1,  // return code
    5:string ret_msg,     // return message
}

struct LoginRequest
{
    1:Request base,
    2:string username,
    3:string password,
}


service MyServer
{
    Response login(1:LoginRequest request),
}

service MyServer2
{
    Response logout(1:Request request)
}
