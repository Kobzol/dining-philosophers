#include "cmd_parser.h"

ArgRequest::ArgRequest(const char* command, int parameter_count)
	: command(command), parameter_count(parameter_count)
{
	
}

ArgResult::ArgResult() : index(ARG_NOT_FOUND)
{
	
}

ArgumentParser::ArgumentParser(char** arguments, int argument_count) : arguments(arguments), argument_count(argument_count)
{
	
}
    
std::vector<ArgResult> ArgumentParser::parse_arguments(std::vector<ArgRequest>& commands)
{
	std::vector<ArgResult> results;
	results.reserve(commands.size());
	
	for (size_t i = 0; i < commands.size(); i++)
	{
		ArgResult result;
		// preskocime nulty argument, tj. cestu k programu
		for (int arg = 1; arg < this->argument_count; arg++)
		{
			// nalezli jsme vhodny parametr
			if (!strcmp(commands[i].command, this->arguments[arg]))
			{
				// pokud není dostatek parametru, skoncime
				if (this->argument_count - arg - 1 >= commands[i].parameter_count)
				{
					result.index = arg;
					arg += commands[i].parameter_count;
				}
			}
		}
		
		results.push_back(result);
	}
	
	return results;
}
