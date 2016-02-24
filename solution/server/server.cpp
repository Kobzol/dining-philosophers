#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include "../common/cmd_parser.h"
#include "dining_table.h"

void handle_args(int argc, char** argv, DiningTable** dining_table, int* port)
{
	if (argc < 2)
	{
		HelpPrinter::print_server_help();
		exit(0);
	}
	else
	{
		ArgumentParser arg_parser(argv, argc);
		
		std::vector<ArgRequest> commands;
		commands.push_back(ArgRequest("-h", 0));
		commands.push_back(ArgRequest("-d", 0));
		commands.push_back(ArgRequest("-s", 1));
		commands.push_back(ArgRequest("-t", 0));
		commands.push_back(ArgRequest("-p", 1));
		
		std::vector<ArgResult> results = arg_parser.parse_arguments(commands);
		
		if (results[0].index != ARG_NOT_FOUND)
		{
			HelpPrinter::print_server_help();
			exit(0);
		}
		
		if (results[1].index != ARG_NOT_FOUND)
		{
			HelpPrinter::debug_mode = true;
		}
		
		int table_count = 5;
		bool use_threads = false;
		
		if (results[2].index != ARG_NOT_FOUND)
		{
			table_count = atoi(argv[results[2].index + 1]);
			
			if (table_count < 1)
			{
				HelpPrinter::print(LOG_ERROR, "Musite zadat kladne cislo\n");
				exit(1);
			}
		}
		
		if (results[3].index != ARG_NOT_FOUND)
		{
			use_threads = true;
		}
		
		if (results[4].index != ARG_NOT_FOUND)
		{
			*port = atoi(argv[results[4].index + 1]);
		
			*dining_table = new DiningTable(table_count, use_threads);
			(*dining_table)->start_feast(*port);
		}
		else
		{
			HelpPrinter::print(LOG_ERROR, "Musite zadat adresu port serveru\n");
			exit(1);
		}
	}
}


int main(int argc, char** argv)
{
	DiningTable* dining_table;
	int port;
	
	handle_args(argc, argv, &dining_table, &port);
	
	HelpPrinter::print(LOG_INFO, "Hostina byla zapocata na portu %d\n", port);
	
	while (true)
	{
		char buf[256];
		int len = read(STDIN_FILENO, buf, sizeof(buf));

		if (len < 1) break;

		buf[len - 1] = '\0';	// vymazani \n
		
		if (!strcmp(buf, "end"))
		{
			dining_table->stop_feast();
			HelpPrinter::print(LOG_INFO, "Hostina byla ukoncena\n");
		}
		else if (!strcmp(buf, "start"))
		{
			dining_table->start_feast(port);
			HelpPrinter::print(LOG_INFO, "Hostina byla obnovena\n");
		}
		else if (!strcmp(buf, "quit"))
		{
			dining_table->stop_feast();
			HelpPrinter::print(LOG_INFO, "Vypinam program\n");
			break;
		}
	}
	
	delete dining_table;

	return 0;
}
