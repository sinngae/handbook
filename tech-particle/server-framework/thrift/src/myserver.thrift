namespace cpp myserver

struct Request
{
	1:i64 msg_id,		// message id
	2:i64 session_id,	// session id
	3:i64 func_id,		// function id
}

struct Response
{
	1:i64 msg_id,
	2:i64 session_id,
	3:i64 func_id,
	4:i64 ret_code = 0,
	5:string ret_msg,
}

struct LoginRequest
{
	1:Request req,
	2:string username,
	3:string password,
}

service MyServer
{
	Response login(1:LoginRequest req),

	Response logout(1:Request req),
}
