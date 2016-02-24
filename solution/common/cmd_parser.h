#pragma once

#include "string.h"
#include <vector>

#define ARG_NOT_FOUND (-1)

// Příkaz z příkazové řádky s volitelnými parametry
struct ArgRequest {
    const char* command;
    int parameter_count;
    
    ArgRequest(const char* command, int parameter_count = 0);
};

struct ArgResult {
    int index;
    
    ArgResult();
};

// Parser příkazů z příkazové řádky
class ArgumentParser {
private:
    char** arguments;
    int argument_count;
    
public:
    ArgumentParser(char** arguments, int argument_count);
    
    // Vyparsuje argumenty podle daných příkazů
    std::vector<ArgResult> parse_arguments(std::vector<ArgRequest>& commands);
};
