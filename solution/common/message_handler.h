#pragma once

#include "help_printer.h"
#include "message.h"
#include "socket.h"

class MessageHandler {
public:
	bool send_message(Socket* socket, Message* message);
	bool receive_message(Socket* socket, Message* received, Message* expected_messages, size_t expected_count = 1);
};
