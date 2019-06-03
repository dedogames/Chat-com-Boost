#pragma once

#include "includes.hpp"

namespace ChatCedro
{

	class Room
	{
	public:
		void join(std::string source, Person_ptr participant)
		{

			//Check if has user
			


			participant->NickName = source;
			users.insert(std::pair< std::string, Person_ptr>(source, participant));
			CurrentUser = source;
			ChatMessage chatMsg;


		 
			boost::this_thread::sleep_for(boost::chrono::seconds{ 1 });
			mut.lock(); 
			BOOST_LOG_TRIVIAL(info) << "Usuario "+source+" Conectado!!";
			mut.unlock();

			std::string SendtoAllCli =   source+"|to|Usuario "+source + " conectou|cmd";
			char line[ChatMessage::max_body_length + 1];
			strcpy(line, SendtoAllCli.c_str());
			chatMsg.body_length(std::strlen(line));
			std::memcpy(chatMsg.body(), line, chatMsg.body_length());
			chatMsg.encode_header();		
			 broadcast(chatMsg);

		}

		bool hasUser(std::string source)
		{
			return users.count(source) == 1 ? true : false;
			
		}

		void disconnectUser(std::string source)
		{
			string msgUsers = "";
			ChatMessage chatMsg;
			std::string SendtoCli = source + "|to|msg|exit|.";
			char line[ChatMessage::max_body_length + 1];
			strcpy(line, SendtoCli.c_str());
			chatMsg.body_length(std::strlen(line));
			std::memcpy(chatMsg.body(), line, chatMsg.body_length());
			chatMsg.encode_header();

			sendTo(source, source, chatMsg);
		}
		void leave(Person_ptr participant)
		{
			if (hasUser(participant->NickName))
			{
				std::map<std::string, Person_ptr>::iterator it = users.find(participant->NickName);
				ChatMessage chatMsg;
				std::string SendtoAllCli = participant->NickName + "|to|Usuario " + participant->NickName + " desconectou|off";
				char line[ChatMessage::max_body_length + 1];
				strcpy(line, SendtoAllCli.c_str());
				chatMsg.body_length(std::strlen(line));
				std::memcpy(chatMsg.body(), line, chatMsg.body_length());
				chatMsg.encode_header();
				broadcast(chatMsg);
			
			
			boost::this_thread::sleep_for(boost::chrono::seconds{ 1 });
		//	mut.lock();
			BOOST_LOG_TRIVIAL(trace) << "Usuario " + participant->NickName + " Desconectou!!";
		//	mut.unlock();			

			
			users.erase(it);
			}
		}

		void leave (std::string participant)
		{
			std::map<std::string, Person_ptr>::iterator it = users.find(participant);
	
			users.erase(it);
		}



		void sendTo(std::string who, std::string to, const ChatMessage& msg)
		{
			recent_msgs_.push_back(msg);
			while (recent_msgs_.size() > max_recent_msgs)
				recent_msgs_.pop_front();


			//users.find(who).second->sendTo(who, to, msg);
			std::map<std::string, Person_ptr>::iterator it = users.find(to);
			it->second->sendTo(who,to, msg);

		}

		void listAll(std::string From)
		{
			std::string msgUsers = "\n";

			for (const auto &participant : users)
			{
				if (From != participant.first)
				{
					msgUsers +=  participant.first + "  \n";
				}

			}

			msgUsers += "\n\n Total de usuarios: "+users.size();
			ChatMessage chatMsg;
			std::string SendtoAllCli = CurrentUser + "|to|" + msgUsers + "|list|.";
			char line[ChatMessage::max_body_length + 1];
			strcpy(line, SendtoAllCli.c_str());
			chatMsg.body_length(std::strlen(line));
			std::memcpy(chatMsg.body(), line, chatMsg.body_length());
			chatMsg.encode_header();

			sendTo(From, From, chatMsg);


		}
		void broadcast(  const ChatMessage& msg)
		{
			recent_msgs_.push_back(msg);
			while (recent_msgs_.size() > max_recent_msgs)
				recent_msgs_.pop_front();
			 
			for (const auto &participant : users)
				// chat_message *m = msg.GetField(FROM);
				if (CurrentUser != participant.first)
				{ 
						participant.second->broadcast(  msg); 
				}
		}

		

	private:
		std::string CurrentUser;
		std::map<std::string, Person_ptr> users;
		enum { max_recent_msgs = 100 };
		ChaMessageQueue recent_msgs_;
		boost::mutex mut;
	};
}