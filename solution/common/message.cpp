#include "message.h"

Message::Message(char type, int NN, const char* text)
{
	this->init(type, NN, text);
}

Message::Message(char type, const char* text)
{
	this->init(type, NN_NOT_SET, text);
}

Message::Message(char type, int NN)
{
	this->init(type, NN, NULL);
}

Message::Message()
{
	this->init(0, NN_NOT_SET, NULL);
}

bool Message::create_from_string(char* buffer, size_t length, Message* result)
{
	if (length < MIN_MESSAGE_LENGTH) return false;
	
	char type = buffer[0];
	int NN = NN_NOT_SET;
	size_t read = 0;
	
	// zkontrolování typu zprávy
	bool found_valid = false;
	for (int i = 0; i < sizeof(allowed_types); i++)
	{
		if (allowed_types[i] == type)
		{
			found_valid = true;
			break;
		}
	}
	
	if (!found_valid)
	{
		return false;
	}
	
	read++;
	
	if (buffer[read] != ':')
	{
		NN = 0;
		for (int i = 0; i < 2; i++)
		{
			if (!isdigit(buffer[read + i]))
			{
				return false;
			}
			else NN += (buffer[read + i] - '0') * (pow(10, 1 - i));
		}
		
		if (buffer[read + 2] != ':')
		{
			return false;
		}
		
		read += 3;	// preskocime cislice i dvojtecku
		
		if (length < 4) return false;	// neni misto pro dvojtecku
	}
	else read++;	// preskocime dvojtecku
	
	if (NN == NN_NOT_SET && buffer[read] == '\n')	// chybi text
	{
		return false;
	}
	
	size_t to_copy = min(static_cast<size_t>(MAX_MESSAGE_LENGTH - 1), length - read - 1);
	
	strncpy(result->text, buffer + read, to_copy);
	result->text[to_copy] = '\0';
	
	result->type = type;
	result->NN = NN;
	
	return true;
}

void Message::init(char type, int NN, const char* text)
{
	this->type = type;
	this->NN = NN;
	
	if (text != NULL)
	{
		// zkopirujeme retezec, ponechame 1 byte pro NULL terminator
		strncpy(this->text, text, min(static_cast<size_t>(MAX_MESSAGE_LENGTH - 1), strlen(text) + 1));
		this->text[MAX_MESSAGE_LENGTH - 1] = 0;
	}
	else this->text[0] = 0;
}

int Message::format(char* buffer)
{
	size_t written = 1;
	
	sprintf(buffer, "%c", this->type);
	
	if (this->NN != NN_NOT_SET)
	{
		sprintf(buffer + written, "%02d", this->NN);
		written += 2;
	}
	
	sprintf(buffer + written, ":");
	written++;
	
	if (this->text[0] != 0)
	{
		size_t write_count = min(strlen(this->text), MAX_MESSAGE_LENGTH - written - 1) + 1;
		snprintf(buffer + written, write_count, "%s", this->text);
		written += write_count - 1;	// -1 because of NULL terminator
	}
	
	sprintf(buffer + written++, "\n");
	
	// NULL terminator is not counted
	return written;
}
