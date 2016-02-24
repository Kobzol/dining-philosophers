#include "threading.h"

pthread_t Threading::spawn_thread(void* (*run)(void* args), void* args)
{
	pthread_t thread;
	
	int result = pthread_create(&thread, NULL, run, args);
	
	if (result == 0)
	{
		this->threads.push_back(thread);
		return thread;
	}
	else
	{
		HelpPrinter::print(LOG_ERROR, "couldn't create thread, error %d\n", errno);
		
		return -1;
	}
}

void Threading::end_thread(pthread_t thread)
{
	for (size_t i = 0; i < this->threads.size(); i++)
	{
		if (pthread_equal(this->threads[i], thread))
		{
			pthread_join(thread, NULL);
			this->threads.erase(this->threads.begin() + i);
			return;
		}
	}
}

void Threading::end_all_threads()
{
	for (size_t i = 0; i < this->threads.size(); i++)
	{
		pthread_join(this->threads[i], NULL);
	}
	
	this->threads.clear();
}
