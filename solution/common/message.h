#pragma once

#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include "math.h"

#include "utility.h"

#define MAX_MESSAGE_LENGTH (256U)
#define MIN_MESSAGE_LENGTH (3)
#define NN_NOT_SET (-1)

#define MSG_COMMAND     ('C')
#define MSG_ANSWER      ('A')
#define MSG_WARNING     ('W')
#define MSG_ERROR       ('E')
#define MSG_INFORMATION ('I')

static char allowed_types[] = {	MSG_COMMAND, MSG_ANSWER, MSG_WARNING, MSG_ERROR, MSG_INFORMATION };

struct Message {
private:
	void init(char type, int NN, const char* text);

public:
	// vytvori zpravu z retezce (bez odradkovani), vraci true, pokud byla zprava vytvorena
	static bool create_from_string(char* buffer, size_t length, Message* result);

	char type;
	int NN;
	char text[MAX_MESSAGE_LENGTH];

	Message(char type, int NN, const char* text);
	Message(char type, const char* text);
	Message(char type, int NN);
	Message();
    
    // zformatovani zpravy do bufferu, vrati pocet zapsanych bytu
    int format(char* buffer);
};
