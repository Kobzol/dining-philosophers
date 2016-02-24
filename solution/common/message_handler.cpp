#include "message_handler.h"

bool MessageHandler::send_message(Socket* socket, Message* message)
{
	char buf[256];
	size_t len = message->format(buf);
	
	HelpPrinter::print(LOG_DEBUG, "Sending %s\n", buf);
	
	socket->write(buf, len);
}

bool MessageHandler::receive_message(Socket* socket, Message* received, Message* expected_messages, size_t expected_count)
{
	char buf[MAX_MESSAGE_LENGTH] = { 0 };
	char tmp_buf[MAX_MESSAGE_LENGTH] = { 0 };
	
	while (true)
	{
		size_t len = socket->readline(buf, MAX_MESSAGE_LENGTH);
		
		if (len < 1)
		{
			break;
		}
		
		HelpPrinter::print(LOG_DEBUG, "Dostal jsem zpravu: %s\n", buf);
		
		Message tmp;
		Message::create_from_string(buf, len, &tmp);
		
		for (size_t i = 0; i < expected_count; i++)
		{
			Message* expected_message = expected_messages + i;
			expected_message->format(tmp_buf);
			
			HelpPrinter::print(LOG_DEBUG, "testing against %s", tmp_buf); 
			
			if (tmp.type == expected_message->type &&
				(tmp.NN == expected_message->NN || !strcmp(tmp.text, expected_message->text)))
			{
				*received = tmp;
				return true;
			}
		}
		
		HelpPrinter::print(LOG_DEBUG, "Zprava nebyla prijata\n", buf);
	}
	
	return false;
}
