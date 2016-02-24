#include "semaphore.h"

int Semaphore::semaphore_group = 0xcafecafe;

Semaphore::Semaphore()
{
	this->handle = semget(Semaphore::semaphore_group++, 1, IPC_CREAT | 0660);

	if (this->handle == -1)
	{
		HelpPrinter::print(LOG_ERROR, "couldn't create semaphore, error %d\n", errno);
	}
	else this->set();
}

sembuf Semaphore::create_sembuf(int op)
{
	sembuf smbf;
	smbf.sem_num = 0;
	smbf.sem_op = op;
	smbf.sem_flg = 0;
	
	return smbf;
}

void Semaphore::set(int value)
{
	::semctl(this->handle, 0, SETVAL, value);
}

void Semaphore::up()
{
	sembuf smbf = this->create_sembuf(1);
	semop(this->handle, &smbf, 1);
}

void Semaphore::down()
{
	sembuf smbf = this->create_sembuf(-1);
	semop(this->handle, &smbf, 1);
}

void Semaphore::close()
{
	int result = semctl(this->handle, 0, IPC_RMID);
	
	if (result != 0)
	{
		HelpPrinter::print(LOG_ERROR, "couldn't close semaphore %d, error %d\n", this->handle, errno);
	}
}
