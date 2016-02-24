#pragma once

#include <sys/wait.h>
#include <vector>

#include "../common/help_printer.h"
#include "../common/message.h"
#include "../common/message_handler.h"
#include "../common/semaphore.h"
#include "../common/shared_memory.h"
#include "../common/socket.h"
#include "../common/server_socket.h"
#include "../common/threading.h"

#include "../../filozof.h"	// makra pro identifikaci zprav

#define STATE_THINKING (0)
#define STATE_HUNGRY (1)
#define STATE_EATING (2)

#define CLIENT_DISCONNECTED (0)
#define CLIENT_CONNECTED (1)

class DiningTable {
private:
	struct ThreadData {
		public:
			DiningTable* instance;
			Socket* client;
	};

	Threading thread_manager;

	Semaphore mutex;
	Semaphore* philosophers;
	MessageHandler message_handler;

	SharedMemory<int> states;
	SharedMemory<int> clients;
	
	ServerSocket server;
	
	int table_count;
	bool use_threads;
	
	void reset();

	int left(int client);
	int right(int client);

	void test_surroundings(int client);
	void take_forks(int client);
	void put_forks(int client);
	
	int seat_client(Socket* client);
	void reset_client(int client);
	
	bool client_feast(Socket* client);
	static void* accept_thread(void* args);
	void accept_client(Socket* client);
	
public:
	DiningTable(int table_count, bool use_threads = false);
	~DiningTable();
	
	void start_feast(int port);
	void stop_feast();
};
