#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/system/error_code.hpp>
#include <boost/bind.hpp>

using namespace std;

class client
{
	typedef boost::asio::ip::tcp::acceptor acceptor_type;
	typedef boost::asio::ip::tcp::endpoint endpoint_type;
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::asio::ip::address address_type;
	typedef boost::shared_ptr<socket_type> sock_ptr;
	typedef std::vector<char> buffer_type;
private:
	boost::asio::io_service m_io_service;
	buffer_type m_buffer;
	endpoint_type m_endpoint;
public:
	client():m_buffer(100, 0), m_endpoint(address_type::from_string("127.0.0.1"), 8888) { start(); }
	void run() { m_io_service.run(); }
	void start()
	{
		sock_ptr sock(new socket_type(m_io_service));
		sock->async_connect(m_endpoint, boost::bind(&client::connect_handler, this, boost::asio::placeholders::error, sock));
	}
	void connect_handler(const boost::system::error_code &ec, sock_ptr sock)
	{
		if (ec)
			return ;
		cout << "receive from " << sock->remote_endpoint().address() << ": " << endl;
		sock->async_read_some(boost::asio::buffer(m_buffer), boost::bind(&client::read_handler, this, boost::asio::placeholders::error, sock));
	}
	void read_handler(const boost::system::error_code &ec, sock_ptr sock)
	{
		if (ec)
			return ;
		sock->async_read_some(boost::asio::buffer(m_buffer), boost::bind(&client::read_handler, this, boost::asio::placeholders::error, sock));
		cout << &m_buffer[0] << endl;
	}
};

int main()
{
	try
	{
		cout << "client start" << endl;
		client cl;
		cl.run();
	}
	catch (std::exception &e)
	{
		cout << e.what() << endl;
	}

	return 0;
}
