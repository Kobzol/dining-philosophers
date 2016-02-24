#pragma once

#include <sys/select.h>

#include "socket.h"
#include "../common/threading.h"
#include "../common/help_printer.h"

#include <vector>

class ServerSocket : public Socket {
private:
	struct ListenInfo {
		ServerSocket* server_instance;
		void* client_instance;
		void (*fn)(void* client_instance, Socket* client);
	};

	bool listening;
	pthread_t listening_thread;
	Threading threading;
	std::vector<Socket*> clients;
	
	static void* listen_fn(void* arg);
	
public:
	struct ListenArgument {
		void* arg;
		void (*fn)(void* client_instance, Socket* client);
	};

	ServerSocket();
	~ServerSocket();

	bool listen(void (*client_callback)(void* client_instance, Socket* socket), void* client_instance, int backlog = 10);
	void stop_listening();
	
	bool is_listening();
};
