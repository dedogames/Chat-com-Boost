#pragma once
#include "includes.hpp"
namespace ChatCedro
{

	class Person
	{
	public:
		virtual ~Person() {}
		virtual void broadcast( const ChatMessage& msg) = 0;
		virtual void sendTo(std::string who,std::string to, const ChatMessage& msg) = 0;
 

		std::string NickName;
		std::string SendTo;
		std::string Message;
		std::string Command;
	};

	typedef std::shared_ptr<Person> Person_ptr;
}

