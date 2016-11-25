#pragma once
#include "../src/server_http.hpp"

using namespace SimpleWeb;

template<class socket_type>
class suServer : public ServerBase<socket_type> {};

typedef boost::asio::ip::tcp::socket HTTP;

template<>
class suServer<HTTP> : public ServerBase<HTTP> {
public:
	suServer(unsigned short port, size_t num_threads = 1, long timeout_request = 5, long timeout_content = 300) :
		ServerBase<HTTP>::ServerBase(port, num_threads, timeout_request, timeout_content) { init(); }

	void init();

protected:
	void accept() {
		//Create new socket for this connection
		//Shared_ptr is used to pass temporary objects to the asynchronous functions
		auto socket = std::make_shared<HTTP>(*io_service);

		acceptor->async_accept(*socket, [this, socket](const boost::system::error_code& ec) {
			//Immediately start accepting a new connection (if io_service hasn't been stopped)
			if (ec != boost::asio::error::operation_aborted)
				accept();

			if (!ec) {
				boost::asio::ip::tcp::no_delay option(true);
				socket->set_option(option);

				read_request_and_content(socket);
			}
		});
	}
};
typedef suServer<HTTP> HttpServer;

