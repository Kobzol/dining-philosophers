#pragma once

#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <semaphore.h>

#include "help_printer.h"

class Semaphore {
private:
	static int semaphore_group;
	
	sembuf create_sembuf(int value);
	
	int handle;

public:
	Semaphore();
	
	int get_value();
	
	void set(int value = 1);
	
	void up();
	void down();
	
	void close();
};
