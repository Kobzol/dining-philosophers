#pragma once

#include <pthread.h>
#include <stdio.h>
#include <errno.h>

#include <vector>

#include "help_printer.h"

class Threading {
private:
	std::vector<pthread_t> threads;
	
public:
	pthread_t spawn_thread(void* (*run)(void* args), void* args);
	void end_thread(pthread_t thread);
	void end_all_threads();
};
