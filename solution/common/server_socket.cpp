#include "server_socket.h"

ServerSocket::ServerSocket()
{
	this->listening = false;
}

ServerSocket::~ServerSocket()
{
	this->stop_listening();
}

bool ServerSocket::listen(void (*client_callback)(void* client_instance, Socket* socket), void* client_instance, int backlog)
{
	if (this->listening)
	{
		return false;
	}
	
	int result = ::listen(this->get_handle(), backlog);
	
	#ifdef SOCKET_CHECK_ERRORS
	if (result != 0)
	{
		HelpPrinter::print(LOG_ERROR, "couldn't start listening, error %d\n", errno);
	}
	#endif
	
	if (result == 0)
	{
		this->listening = true;

		ListenInfo* info = new ListenInfo;
		info->server_instance = this;
		info->client_instance = client_instance;
		info->fn = client_callback;

		this->listening_thread = this->threading.spawn_thread((void* (*)(void*)) &ServerSocket::listen_fn, info);
		
		return true;
	}
	else return false;
}

void ServerSocket::stop_listening()
{
	for (size_t i = 0; i < this->clients.size(); i++)
	{
		this->clients[i]->close();
		delete this->clients[i];
	}
	
	this->clients.clear();
	
	this->listening = false;
	this->threading.end_thread(this->listening_thread);
	
	HelpPrinter::print(LOG_DEBUG, "server stopped listening\n");
}

void* ServerSocket::listen_fn(void* args)
{
	ListenInfo* info = (ListenInfo*) args;
	
	while (info->server_instance->listening)
	{
		timeval timeout = { 1, 0 };
		fd_set read_wait_set;
		FD_ZERO(&read_wait_set);
		FD_SET(info->server_instance->get_handle(), &read_wait_set);
		
		if (select(info->server_instance->get_handle() + 1, &read_wait_set, 0, 0, &timeout) < 0)
		{
			break;
		}
		
		if (FD_ISSET(info->server_instance->get_handle(), &read_wait_set))
		{
			int client = ::accept(info->server_instance->get_handle(), NULL, NULL);
			Socket* socket = new Socket(client);
			info->server_instance->clients.push_back(socket);
			
			info->fn(info->client_instance, socket);
		}
	}
		
	HelpPrinter::print(LOG_DEBUG, "server stopped listening on thread\n");
	delete info;
}

bool ServerSocket::is_listening()
{
	return this->listening;
}
