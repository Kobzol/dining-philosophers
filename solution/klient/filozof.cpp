#include "filozof.h"

Filozof::Filozof(char* hostname, int port, int minimum_wait_time) : minimum_wait_time(minimum_wait_time)
{
	HelpPrinter::print(LOG_DEBUG, "filozof connecting to %s:%d\n", hostname, port);
	
	// pripojeni k serveru
	this->server.connect(hostname, port);
}

bool Filozof::send_message(Message* message)
{
	return this->message_handler.send_message(&this->server, message);
}

bool Filozof::receive_message(Message* received, Message* expected_messages, size_t expected_count)
{
	return this->message_handler.receive_message(&this->server, received, expected_messages, expected_count);
}

bool Filozof::is_connected()
{
	return this->server.is_connected();
}

bool Filozof::feast()
{
	Message msg(MSG_COMMAND, CI_Prichazim, CS_Prichazim);
	this->send_message(&msg);
	
	Message received;
	msg = Message(MSG_ANSWER, AI_ZidleX, AS_ZidleX);
	
	if (!this->receive_message(&received, &msg))
	{
		HelpPrinter::print(LOG_INFO, "Nebyl dostatek mist u stolu\n");
		return true;
	}

	int zidle = -1;
	sscanf(received.text, AS_ZidleX, &zidle);
	
	HelpPrinter::print(LOG_INFO, "Filozof byl usazen na zidli %d\n", zidle);

    while (true)
    {
        int cas = rand() % 5 + this->minimum_wait_time;
        HelpPrinter::print(LOG_INFO, "Filozof[%d] bude premyslet %ds.\n", zidle, cas);
        sleep(cas);

        HelpPrinter::print(LOG_INFO, "Filozof[%d] dostal hlad...\n", zidle );

        msg = Message(MSG_COMMAND, CI_Hladovim, CS_Hladovim);
        
        if (!this->send_message(&msg)) return false;

        HelpPrinter::print(LOG_INFO, "Filozof[%d] ceka na vidlicky...\n", zidle);

        msg = Message(MSG_ANSWER, AI_Jez, AS_Jez);
        
        if (!this->receive_message(&received, &msg)) return false;

        cas = rand() % 5 + this->minimum_wait_time;
        HelpPrinter::print(LOG_INFO, "Filozof[%d] dostal vidlicky a bude jist %ds...\n", zidle, cas);
        
        sleep(cas);
        HelpPrinter::print(LOG_INFO, "Filozof[%d] dojedl a odklada vidlicky...\n", zidle);

        msg = Message(MSG_COMMAND, CI_Premyslim, CS_Premyslim);
        
        if (!this->send_message(&msg)) return false;

        msg = Message(MSG_ANSWER, AI_Dojezeno, AS_Dojezeno);
        
        if (!this->receive_message(&received, &msg)) return false;

        HelpPrinter::print(LOG_INFO, "Filozof[%d] muze premyslet...\n", zidle);

        if (!(rand() % 5))
        {
            HelpPrinter::print(LOG_INFO, "Filozof[%d] se rozhodl odejit od stolu...\n", zidle);

            msg = Message(MSG_COMMAND, CI_Odchazim, CS_Odchazim);
            
            if (!this->send_message(&msg)) return false;

            msg = Message(MSG_ANSWER, AI_Nashledanou, AS_Nashledanou);
            if (!this->receive_message(&received, &msg)) return false;
			
            return true;
        }
    }
    
    return false;
}

Socket* Filozof::get_socket()
{
	return &this->server;
}
