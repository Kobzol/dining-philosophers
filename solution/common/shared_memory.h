#pragma once

#include <sys/mman.h>
#include <errno.h>

#include "help_printer.h"

template <typename T>
class SharedMemory {
private:
	int count;
	T* shared_region;
	
public:
	SharedMemory()
	{
		
	}
	SharedMemory(int count) : count(count)
	{
		this->shared_region = (T*) ::mmap(NULL, sizeof(T) * count, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
		
		if (this->shared_region == MAP_FAILED)
		{
			HelpPrinter::print(LOG_ERROR, "couldn't map shared memory, error %d\n", errno);
		}
	}

	T* get_data()
	{
		return this->shared_region;
	}

	T& operator[](int index)
	{
		return this->shared_region[index];
	}
	
	T& operator*()
	{
		return *this->shared_region;
	}

	void unmap()
	{
		if (::munmap(this->shared_region, sizeof(T) * this->count) != 0)
		{
			HelpPrinter::print(LOG_ERROR, "couldn't unmap shared memory, error %d\n", errno);
		}
	}
};
