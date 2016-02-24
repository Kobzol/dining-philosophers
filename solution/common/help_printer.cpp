#include "help_printer.h"

bool HelpPrinter::debug_mode = false;

void HelpPrinter::print_client_help()
{
	printf(
		"Toto je napoveda ke klientovi vecericich filozofu.\n"
		"\n"
		"Pouziti: -s <server> -p <port>\n\n"
		"Parametry:\n"
		"-s Adresa serveru, ke kteremu se ma klient pripojit\n"
		"-p Port serveru, ke ktermu se ma klient pripojit\n"
		"-c Minimalni doba cekani pozadavku v sekundach\n"
		"-d Zapne debug mod\n"
		"-h Zobrazi napovedu\n"
	);
}

void HelpPrinter::print_server_help()
{
	printf(
		"Toto je napoveda k serveru vecericich filozofu.\n"
		"\n"
		"Pouziti: -p <port>\n\n"
		"Parametry:\n"
		"-p Port, na kterém má server poslouchat\n"
		"-s Pocet mist u stolu\n"
		"-t Pouzije vlakna namisto procesu\n"
		"-d Zapne debug mod\n"
		"-h Zobrazi napovedu\n\n" 
		"Prikazy:\n"
		"start - zapne server, pokud je vypnuty\n"
		"end - vypne server, pokud je zapnuty\n"
		"quit - vypne program\n"
	);
}

void HelpPrinter::print(int log_level, const char* string, ...)
{
	if (log_level == LOG_DEBUG && !HelpPrinter::debug_mode) return;
		
	va_list arg_list;	
	
	if (log_level == LOG_ERROR)
	{
		char buffer[1024];
			
		va_start(arg_list, string);
		vsnprintf(buffer, sizeof(buffer), string, arg_list);
		va_end(arg_list);
		
		printf("ERROR: %s", buffer);
	}
	else
	{
		va_start(arg_list, string);
		vprintf(string, arg_list);
		va_end(arg_list); 
	}
}
