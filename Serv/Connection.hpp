#pragma once
#include "includes.hpp"
 
namespace ChatCedro
{

	class Connection
		: public Person,
		public std::enable_shared_from_this<Connection>
	{
	public:
		Connection(tcp::socket socket, Room& room)
			: socket_(std::move(socket)),
			room_(room)
		{
		}

		void start()
		{
			registered = false;
			do_read_header();
		}

		void Login()
		{

		}
		 


		void sendTo(std::string who, std::string to, const ChatMessage& msg)
		{ 
			bool write_in_progress = !write_msgs_.empty();
			write_msgs_.push_back(msg);
			if (!write_in_progress)
			{
				do_write();
			}
		}
		 
		void broadcast( const ChatMessage& msg)
		{

			bool write_in_progress = !write_msgs_.empty();
			write_msgs_.push_back(msg);
			if (!write_in_progress)
			{
				do_write();
			}
		}

		void ListAllUsers()
		{

		}

	private:
		void do_read_header()
		{
			auto self(shared_from_this());
			boost::asio::async_read(socket_,
				boost::asio::buffer(read_msg_.data(), ChatMessage::header_length),
				[this, self](boost::system::error_code ec, std::size_t /*length*/)
			{
				if (!ec && read_msg_.decode_header())
				{
					do_read_body();
				}
				else
				{
					room_.leave(shared_from_this());
				}
			});
		}

		void do_read_body()
		{
			auto self(shared_from_this());
			boost::asio::async_read(socket_,
				boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
				[this, self](boost::system::error_code ec, std::size_t /*length*/)
			{
				if (!ec)
				{
					if (!registered)
					{
						registered = true;
						if (room_.hasUser(read_msg_.GetField(FROM))) {
							room_.disconnectUser(read_msg_.GetField(FROM));
							 
						}else
							room_.join(read_msg_.GetField(FROM),shared_from_this());
						do_read_header();
						return;
					}

					if (read_msg_.GetField(COMMAND) == "list")
						room_.listAll(read_msg_.GetField(FROM));
					else if (read_msg_.GetField(TO) == "all")
						room_.broadcast(read_msg_);
					else
						room_.sendTo(read_msg_.GetField(FROM), read_msg_.GetField(TO), read_msg_);

						do_read_header();
					 
				}
				else
				{
					room_.leave(shared_from_this());
				}
			});
		}

		void do_write()
		{
			auto self(shared_from_this());
			boost::asio::async_write(socket_,
				boost::asio::buffer(write_msgs_.front().data(),
					write_msgs_.front().length()),
				[this, self](boost::system::error_code ec, std::size_t /*length*/)
			{
				if (!ec)
				{
					write_msgs_.pop_front();
					if (!write_msgs_.empty())
					{
						do_write();
					}
				}
				else
				{
					room_.leave(shared_from_this());
				}
			});
		}

		tcp::socket socket_;
		Room& room_;
		ChatMessage read_msg_;
		ChaMessageQueue write_msgs_;
		bool registered;
	};
}
