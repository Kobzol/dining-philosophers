#pragma once

#include "../../filozof.h"	// spolecny hlavickovy soubor

#include "../common/help_printer.h"
#include "../common/message.h"
#include "../common/message_handler.h"
#include "../common/socket.h"

class Filozof {
private:
	Socket server;
	MessageHandler message_handler;
	
	int minimum_wait_time;
	
public:
	static const int MINIMUM_WAIT_TIME = 2;

	Filozof(char* hostname, int port, int minimum_wait_time = Filozof::MINIMUM_WAIT_TIME);
	
	bool send_message(Message* msg);
	bool receive_message(Message* received, Message* expected_messages, size_t expected_count = 1);
	
	bool is_connected();
	
	bool feast();
	
	Socket* get_socket();
};
