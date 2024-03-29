/************************************
*			SERVER 		           *
*************************************/

#include "includes.hpp"
 
namespace ChatCedro
{
	void StartServer(string port)
	{
		try
		{
			 
			boost::asio::io_context io_context;


			std::list<Server> servers;
				tcp::endpoint endpoint(tcp::v4(), boost::lexical_cast<unsigned short>(port));
			 
				Server serv(io_context, endpoint);

				const char* help =  "**********************************\n"
									"*          Servidor 1.0          *\n"
									"*                                *\n"
									"*    Servidor Iniciado           *\n"
									"**********************************\b";
				std::cout << help << std::endl;
				io_context.run();

			
		}
		catch (std::exception& e)
		{
			std::cerr << "Exception: " << e.what() << "\n";
		}

	}
}

 
int main(int argc, char* argv[])
{
	//
 
	logging::add_file_log
	(
		keywords::file_name = "Server.log",
		keywords::format =
		(
			expr::stream	<< " [ " << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S") << "]"
			<< expr::smessage
		)
	);

	logging::add_console_log
	(
		std::clog,
		keywords::format =
		(
			expr::stream 	<< " [ " << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S") << "]"
			<< expr::smessage
		)

	);

	logging::add_common_attributes();

	ChatCedro::StartServer(argv[1]);
 
	return 0;
}