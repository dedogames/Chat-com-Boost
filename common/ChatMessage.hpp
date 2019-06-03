//
// chat_message.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP
namespace ChatCedro
{
#include <cstdio>
#include <cstdlib>
#include <cstring> 
#include <iosfwd> 
#include <string>
 
	enum EField {
		FROM = 0,
		TO,
		MENSSAGE,
		COMMAND
	};

	class ChatMessage
	{
	public:
		enum { header_length = 5 };
		enum { max_body_length = 1024 };

		ChatMessage()
			: body_length_(0)
		{
		}

		const char* data() const
		{
			return data_;
		}

		char* data()
		{
			return data_;
		}

		 

	
		std::string GetField( EField e)
		{ 
			char * newS = new char[body_length()];
			strncpy(newS, body(), body_length());
			string s(newS);
			string pipe = "|";
			std::string::size_type b = 0;
			std::vector<std::string> result;

			while ((b = s.find_first_not_of(pipe, b)) != std::string::npos) {
				auto e = s.find_first_of(pipe, b);
				result.push_back(s.substr(b, e - b));
				 
				b = e;
			}
			return result[e];
		}




		 

		std::size_t length() const
		{
			return header_length + body_length_;
		}

		const char* body() const
		{
			return data_ + header_length;
		}

		char* body()
		{
			return data_ + header_length;
		}

		std::size_t body_length() const
		{
			return body_length_;
		}

		void body_length(std::size_t new_length)
		{
			body_length_ = new_length;
			if (body_length_ > max_body_length)
				body_length_ = max_body_length;
		}

		bool decode_header()
		{
			char header[header_length + 1] = "";
			std::strncat(header, data_, header_length);
			body_length_ = std::atoi(header);
			if (body_length_ > max_body_length)
			{
				body_length_ = 0;
				return false;
			}
			return true;
		}

		void encode_header()
		{
			char header[header_length + 1] = "";
			std::sprintf(header, "%4d", static_cast<int>(body_length_));
			std::memcpy(data_, header, header_length);
		}

	private:
		char data_[header_length + max_body_length];
		std::size_t body_length_;
		std::string Nickname;
	};

	typedef std::deque<ChatMessage> ChaMessageQueue;


}

#endif // CHAT_MESSAGE_HPP