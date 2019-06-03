#pragma once
#include "includes.hpp"

namespace ChatCedro
{

	class Server
	{
	public:
		Server(boost::asio::io_context& io_context,
			const tcp::endpoint& endpoint)
			: acceptor_(io_context, endpoint)
		{


		

			do_accept();
			
			
		}

	private:
		void do_accept()
		{
			acceptor_.async_accept(
				[this](boost::system::error_code ec, tcp::socket socket)
			{
				if (!ec)
				{

					
				
					std::make_shared<Connection>(std::move(socket), room_)->start();
				}

				do_accept();
			});
		}

		tcp::acceptor acceptor_;
		Room room_;
	};
}