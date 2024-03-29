/************************************
*			CLIENT		           *    
*************************************/

#include "Includes.hpp"


namespace ChatCedro
{

	void StartPrompt( Client &c, std::string Nickname) 
	{

		string msg;
 

		std::cout << ">";
		char line[ChatMessage::max_body_length + 1];
		while (getline(cin, msg))
		{
			ChatMessage chatMsg;
			if (msg[0] == ':')
			{
				if (msg.find("help") != string::npos)
				{

					const char* help = "***************************************************\n"
										"*           Ajuda Para usar o chat                 *\n"
										"*                                                  *\n"
										"*    Enviar pra todos na sala:  <all|menssagem >   *\n"
										"*    Enviar no privado:  <nome|menssagem >         *\n"
										"*    Listar todos da sala:  <:list >               *\n"
										"*    Ajuda:     <:help  >                          *\n"
										"*                                                  *\n"
										"*    Exemplo:   lucas|Bom dia                      *\n"
										"***************************************************\b";

					std::cout << help << std::endl;
				}
				else

					if (msg.find("me") != string::npos)
					{
						string Id = "\n\n************************************************\n *******       Id: " + (Nickname)+"    ******\n************************************************\n";

						std::cout << Id << std::endl;
					}else
					if (msg.find("list") != string::npos)
					{
						string listUsuers = "\n\nUsuarios connectados      \n";
						std::cout << listUsuers;

						string SendtoServer = Nickname + "|to|msg|list|.";
						strcpy(line, SendtoServer.c_str());

						chatMsg.body_length(std::strlen(line));
						std::memcpy(chatMsg.body(), line, chatMsg.body_length());
						chatMsg.encode_header();
						c.write(chatMsg);
						//std::cout << GetField(SendtoServer, FROM)<<"   "<<GetField(SendtoServer, COMMAND) <<std::endl;
					}
					else
					{
						string listUsuers = "\n\n!!!!  COMANDO INVALIDO !!!!     \n";
						std::cout << listUsuers << std::endl;
					}
				std::cout << ">";

			}
			else
			{
				std::cout << ">";
				string SendtoServer = Nickname + "|" + msg + "|cmd|.";
				strcpy(line, SendtoServer.c_str()); 

				chatMsg.body_length(std::strlen(line));
				std::memcpy(chatMsg.body(), line, chatMsg.body_length());
				chatMsg.encode_header();
				c.write(chatMsg);
			}
		} 
	}


	void StartCli(string ip, string port, string Nickname)
	{
		try
		{
			boost::asio::io_context io_context;

			tcp::resolver resolver(io_context);
			auto endpoints = resolver.resolve(ip, port);
			Client c(io_context, endpoints, Nickname);

			std::thread t([&io_context]() { io_context.run(); });
			StartPrompt(c, Nickname);

			c.close();
			t.join();
		}
		catch (std::exception& e)
		{
			std::cerr << "Exception: " << e.what() << "\n";
		}
	}
}
 
int main(int argc, char* argv[])
{
	
	ChatCedro::StartCli(argv[1], argv[2], argv[3]);
 
		
	 
	return 0;
};