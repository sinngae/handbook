#include <string.h>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/system/error_code.hpp>
#include <boost/bind/bind.hpp>

using namespace std;

class server
{
	typedef boost::asio::ip::tcp::acceptor acceptor_type;
	typedef boost::asio::ip::tcp::endpoint endpoint_type;
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::asio::ip::address address_type;
	typedef boost::shared_ptr<socket_type> sock_ptr;
private:
	boost::asio::io_service m_io_service;
	acceptor_type m_acceptor;
public:
	server():m_acceptor(m_io_service, endpoint_type(boost::asio::ip::tcp::v4(), 8888)) { accept(); }
	void run() { m_io_service.run(); }
	void accept()
	{
		sock_ptr sock(new socket_type(m_io_service));
		m_acceptor.async_accept(*sock, boost::bind(&server::accept_handler, this, boost::asio::placeholders::error, sock));
	}
	void accept_handler(const boost::system::error_code &ec, sock_ptr sock)
	{
		if (ec)
			return ;
		cout << "client:";
		cout << sock->remote_endpoint().address() << endl;
		sock->async_write_some(boost::asio::buffer("hello asio"), boost::bind(&server::write_handler, this, boost::asio::placeholders::error));
		accept();
	}
	void write_handler(const boost::system::error_code &ec)
	{
		cout << "send msg complete" << endl;
	}
};


int main(int argc, char *argv[])
{
	// version
	cout << "version: 0.0.1 " << __TIME__ << __DATE__ << endl;
	
	//
	try
	{
		cout << "server start" << endl;
		server srv;
		srv.run();
	}
	catch (std::exception &e)
	{
		cout << e.what() << endl;
	}

	return 0;
}
