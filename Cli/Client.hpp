#pragma once

#include "Includes.hpp"
namespace ChatCedro
{

	using boost::asio::ip::tcp;

	//typedef std::deque<ChatMessage> ChatMessage_queue;

	class Client
	{
	public:
		Client(boost::asio::io_context& io_context,
			const tcp::resolver::results_type& endpoints, string Nick)
			: io_context_(io_context),
			socket_(io_context),
			Nickname(Nick)
		{
			do_connect(endpoints);
		}

		void write(const ChatMessage& msg)
		{
			boost::asio::post(io_context_,
				[this, msg]()
			{
				bool write_in_progress = !write_msgs_.empty();
				write_msgs_.push_back(msg);
				if (!write_in_progress)
				{
					do_write();
				}
			});
		}

		void close()
		{
			boost::asio::post(io_context_, [this]() { socket_.close(); });
		}

	private:
		void do_connect(const tcp::resolver::results_type& endpoints)
		{
			boost::asio::async_connect(socket_, endpoints,
				[this](boost::system::error_code ec, tcp::endpoint)
			{
				if (!ec)
				{
					Login();
					

					
				}
			});
		}

		void do_read_header()
		{
			boost::asio::async_read(socket_,
				boost::asio::buffer(read_msg_.data(), ChatMessage::header_length),
				[this](boost::system::error_code ec, std::size_t /*length*/)
			{
				if (!ec && read_msg_.decode_header())
				{
					do_read_body();
				}
				else
				{
					socket_.close();
				}
			});
		}

		void do_read_body()
		{
			boost::asio::async_read(socket_,
				boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
				[this](boost::system::error_code ec, std::size_t /*length*/)
			{
				if (!ec)
				{
					//std::cout.write(read_msg_.body(), read_msg_.body_length());
					if (read_msg_.GetField(COMMAND) == "exit")
					{
						socket_.close();
						exit(1);
						return;
					}

					if(read_msg_.GetField(COMMAND) == "list" || read_msg_.GetField(COMMAND) == "off")
						std::cout <<  read_msg_.GetField(MENSSAGE) << std::endl;
					else
						std::cout << read_msg_.GetField(FROM) <<" disse "<<read_msg_.GetField(MENSSAGE)  << std::endl;
					
					std::cout << ">";
	 
					do_read_header();
				}
				else
				{
					socket_.close();
				}
			});
		}

		void do_write()
		{
			boost::asio::async_write(socket_,
				boost::asio::buffer(write_msgs_.front().data(),
					write_msgs_.front().length()),
				[this](boost::system::error_code ec, std::size_t /*length*/)
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
					socket_.close();
				}
			});
		}

		void Login()
		{

			char line[ChatMessage::max_body_length + 1];
			ChatMessage msg;

 

			strcpy(line, (Nickname+"|name|msg|cmd").c_str());
			msg.body_length(std::strlen(line));
			std::memcpy(msg.body(), line, msg.body_length());
			msg.encode_header();

			write_msgs_.push_back(msg);

			string Id = "\n\n************************************************\n *******       Id: "+(Nickname)+"    ******\n************************************************\n";
				         
			std::cout  << Id << std::endl;
			boost::asio::async_write(socket_,
				boost::asio::buffer(write_msgs_.front().data(),
					msg.length()),
				[this](boost::system::error_code ec, std::size_t /*length*/)
			{
				if (!ec)
				{
					    write_msgs_.pop_front();
						do_read_header();
					 
				}
				else
				{
					socket_.close();
				}
			});
		}

		 

	private:
		boost::asio::io_context& io_context_;
		tcp::socket socket_;
		ChatMessage read_msg_;
		ChaMessageQueue write_msgs_;
		string Nickname;
	};

}
