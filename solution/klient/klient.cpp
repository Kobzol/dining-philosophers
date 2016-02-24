#include "stdio.h"
#include "stdlib.h"
#include <vector>

#include "../common/cmd_parser.h"
#include "../common/help_printer.h"
#include "../common/message.h"

#include "filozof.h"

void handle_args(int argc, char** argv)
{
	if (argc < 2)
	{
		HelpPrinter::print_client_help();
		exit(0);
	}
	else
	{
		ArgumentParser arg_parser(argv, argc);
		
		std::vector<ArgRequest> commands;
		commands.push_back(ArgRequest("-h", 0));
		commands.push_back(ArgRequest("-d", 0));
		commands.push_back(ArgRequest("-s", 1));
		commands.push_back(ArgRequest("-p", 1));
		commands.push_back(ArgRequest("-c", 1));
		
		std::vector<ArgResult> results = arg_parser.parse_arguments(commands);
		
		if (results[0].index != ARG_NOT_FOUND)
		{
			HelpPrinter::print_client_help();
			exit(0);
		}
		
		if (results[1].index != ARG_NOT_FOUND)
		{
			HelpPrinter::debug_mode = true;
		}
		
		int minimum_wait_time = Filozof::MINIMUM_WAIT_TIME;
		
		if (results[4].index != ARG_NOT_FOUND)
		{
			minimum_wait_time = atoi(argv[results[4].index + 1]);
		}
		
		if (results[2].index != ARG_NOT_FOUND && results[3].index != ARG_NOT_FOUND)
		{
			char* server = argv[results[2].index + 1];
			unsigned short port = atoi(argv[results[3].index + 1]);
			
			Filozof filozof(server, port, minimum_wait_time);
			
			if (filozof.is_connected())
			{
				char buf[256];
				filozof.get_socket()->get_info(buf, &port);
				
				HelpPrinter::print(LOG_INFO, "Pripojuji se z %s:%d\n", buf, port);
				
				filozof.get_socket()->get_remote_info(buf, &port);
				HelpPrinter::print(LOG_INFO, "Jsem pripojen k %s:%d\n", buf, port);
				
				if (!filozof.feast())
				{
					HelpPrinter::print(LOG_ERROR, "Spojeni se serverem bylo ukonceno neocekavane\n");
				}
				else HelpPrinter::print(LOG_INFO, "Spojeni se serverem bylo ukonceno ocekavane\n");
			}
			else
			{
				HelpPrinter::print(LOG_ERROR, "Nepodarilo se pripojit k serveru\n");
				exit(1);
			}
		}
		else
		{
			HelpPrinter::print(LOG_ERROR, "Musite zadat adresu serveru i port\n");
			exit(1);
		}
	}
}

int main(int argc, char** argv)
{	
	handle_args(argc, argv);

	return 0;
}
