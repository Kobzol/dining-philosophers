#include "dining_table.h"

DiningTable::DiningTable(int table_count, bool use_threads) : table_count(table_count), use_threads(use_threads)
{
	this->philosophers = new Semaphore[table_count];
	this->states = SharedMemory<int>(table_count);	// je bezpecne prekopirovat, protoze SharedMemory neobsahuje vlastni destruktor
	this->clients = SharedMemory<int>(table_count);
	this->mutex.set(1);
	
	this->reset();
}

DiningTable::~DiningTable()
{
	if (this->use_threads)
	{
		this->thread_manager.end_all_threads();
	}
	else
	{
		while (wait(NULL) != -1);	// pockat na ukonceni vsech dcerinnych procesu
	}
	
	for (int i = 0; i < this->table_count; i++)
	{
		this->philosophers[i].close();
	}
	
	delete[] this->philosophers;
	
	this->mutex.close();
	this->states.unmap();
	this->clients.unmap();
}

void DiningTable::reset()
{
	this->mutex.down();
	
	for (int i = 0; i < table_count; i++)
	{
		this->philosophers[i].set(0);
		this->states[i] = STATE_THINKING;
		this->clients[i] = CLIENT_DISCONNECTED;
	}
	
	this->mutex.up();
	
	if (this->use_threads)
	{
		this->thread_manager.end_all_threads();
	}
	else
	{
		while (wait(NULL) != -1);	// pockat na ukonceni vsech dcerinnych procesu
	}
}

int DiningTable::left(int client)
{
	return (client - 1 + this->table_count) % this->table_count;
}

int DiningTable::right(int client)
{
	return (client + 1) % this->table_count;
}

void DiningTable::test_surroundings(int client)
{
	if (this->states[client] == STATE_HUNGRY &&
		this->states[this->left(client)] != STATE_EATING &&
		this->states[this->right(client)] != STATE_EATING)
	{
		this->states[client] = STATE_EATING;
		this->philosophers[client].up();
	}
}

void DiningTable::take_forks(int client)
{
	this->mutex.down();
	
	this->states[client] = STATE_HUNGRY;
	this->test_surroundings(client);
	
	this->mutex.up();
	
	this->philosophers[client].down();
}

void DiningTable::put_forks(int client)
{
	this->mutex.down();
	
	this->states[client] = STATE_THINKING;
	
	this->test_surroundings(this->left(client));
	this->test_surroundings(this->right(client));
	
	this->mutex.up();
}

int DiningTable::seat_client(Socket* client)
{
	Message received;
	Message expected(MSG_COMMAND, CI_Prichazim, CS_Prichazim);
	
	if (!this->message_handler.receive_message(client, &received, &expected))
	{
		return -1;
	}
	
	this->mutex.down();
	
	int client_number = -1;
	
	for (int i = 0; i < this->table_count; i++)
	{
		if (this->clients[i] == CLIENT_DISCONNECTED)
		{
			client_number = i;
			this->clients[i] = CLIENT_CONNECTED;
			break;
		}
	}
	
	this->mutex.up();
	
	if (client_number == -1)
	{
		return -1;
	}
	else return client_number;
}

void DiningTable::reset_client(int client)
{
	mutex.down();
	clients[client] = CLIENT_DISCONNECTED;
	mutex.up();
	
	philosophers[client].set(0);
	states[client] = STATE_THINKING;
}

bool DiningTable::client_feast(Socket* client)
{
	Message send;
	Message received;
	Message expected[2];
	
	int client_number = this->seat_client(client);
	
	if (client_number < 0)
	{
		HelpPrinter::print(LOG_INFO, "Filozof prisel k plnemu stolu a bude odpojen\n");
		return false;
	}
	else
	{
		send = Message(MSG_ANSWER, AI_ZidleX, AS_ZidleX);
		sprintf(send.text, send.text, client_number);
		
		if (!this->message_handler.send_message(client, &send))
		{
			this->reset_client(client_number);
			return false;
		}
		
		HelpPrinter::print(LOG_INFO, "Filozof byl usazen u stolu %d\n", client_number);
	}
	
	states[client_number] = STATE_THINKING;
	
	while (true)
	{
		HelpPrinter::print(LOG_INFO, "Filozof[%d] premysli...\n", client_number);
		
		expected[0] = Message(MSG_COMMAND, CI_Hladovim, CS_Hladovim);
		expected[1] = Message(MSG_COMMAND, CI_Odchazim, CS_Odchazim);
        
        if (!this->message_handler.receive_message(client, &received, &expected[0], 2))
        {
			this->reset_client(client_number);
			return false;
		}
		
		if (received.NN == CI_Odchazim)
		{
			HelpPrinter::print(LOG_INFO, "Filozofa %d to prestalo bavit...\n", client_number);
			
			send = Message(MSG_ANSWER, AI_Nashledanou, AS_Nashledanou);
            if (!this->message_handler.send_message(client, &send))
            {
				this->reset_client(client_number);
				return false;
			} 
			
			this->reset_client(client_number);
			return true;
		}

        HelpPrinter::print(LOG_INFO, "Filozof[%d] ceka na vidlicky...\n", client_number);
        this->take_forks(client_number);

        send = Message(MSG_ANSWER, AI_Jez, AS_Jez);
        
        if (!this->message_handler.send_message(client, &send))
        {
			this->reset_client(client_number);
			return false;
		}
		
        for (int i = 0; i < this->table_count; i++)
        {
            printf("%d ", this->states[i]);
        }
        
		HelpPrinter::print(LOG_INFO, "Filozof[%d] ji...\n", client_number);
		
		expected[0] = Message(MSG_COMMAND, CI_Premyslim, CS_Premyslim);
		
		if (!this->message_handler.receive_message(client, &received, &expected[0]))
		{
			this->reset_client(client_number);
			return false;
		}
		
		HelpPrinter::print(LOG_INFO, "Filozof[%d] prestava jist...\n", client_number);
		this->put_forks(client_number);
		
		send = Message(MSG_ANSWER, AI_Dojezeno, AS_Dojezeno);
		
		if (!this->message_handler.send_message(client, &send))
		{
			this->reset_client(client_number);
			return false;
		}
	}
	
	this->reset_client(client_number);
	return false;
}

void* DiningTable::accept_thread(void* args)
{
	ThreadData* data = (ThreadData*) args;
	
	if (!data->instance->client_feast(data->client))
	{
		HelpPrinter::print(LOG_ERROR, "Filozof byl ukoncen neocekavane\n");
	}
	else HelpPrinter::print(LOG_INFO, "Filozof byl ukoncen sporadane\n");
	
	data->client->close();
	
	delete data;
}

void DiningTable::accept_client(Socket* client)
{
	char buf[256];
	unsigned short port;
	client->get_remote_info(buf, &port);
	
	HelpPrinter::print(LOG_INFO, "Pripojil se klient z %s:%d\n", buf, port); 
	
	if (this->use_threads)
	{	
		ThreadData* data = new ThreadData;
		data->instance = this;
		data->client = client;
		
		this->thread_manager.spawn_thread(&DiningTable::accept_thread, data);
	}
	else
	{
		int pid = fork();
	
		if (pid == 0)
		{
			if (!this->client_feast(client))
			{
				HelpPrinter::print(LOG_ERROR, "Filozof byl ukoncen neocekavane\n");
			}
			else HelpPrinter::print(LOG_INFO, "Filozof byl ukoncen sporadane\n");
			
			client->close();
			
			exit(0);
		}
		else client->close();
	}
}

void DiningTable::start_feast(int port)
{
	if (this->server.is_listening()) return;
	
	this->server.bind(port);
	this->server.listen((void (*)(void*, Socket*)) &DiningTable::accept_client, this);
}

void DiningTable::stop_feast()
{
	if (!this->server.is_listening()) return;
	
	this->server.stop_listening();
	this->reset();
}
