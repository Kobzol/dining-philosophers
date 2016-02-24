//***************************************************************************
//
// Petr Olivka (c) 03/2013 pro projekt do predmetu OSY.
// Viz. http://poli.cs.vsb.cz/edu/osy
//
// Priklad klienta pro IPC problem 'Vecerici filozofove'
// 
//***************************************************************************

// Prikazy protokolu
#define CI_Prichazim    11
#define CS_Prichazim   "Chci si sednout"
#define CI_Hladovim     22
#define CS_Hladovim     "Chci jist"
#define CI_Premyslim    33
#define CS_Premyslim    "Dojedl jsem"
#define CI_Odchazim     44
#define CS_Odchazim     "Odchazim"

// Odpovedi protokolu
#define AI_ZidleX       12
#define AS_ZidleX       "Sedis na zidli %d"
#define AI_Jez          23
#define AS_Jez          "Muzes jist"
#define AI_Dojezeno     34
#define AS_Dojezeno     "Vidlicky polozeny"
#define AI_Nashledanou  45
#define AS_Nashledanou  "Nashledanou"

// Chyby
#define EI_Klient       99
#define ES_Klient       "Obecna chyba klienta"
#define EI_Server       98
#define ES_Server       "Obecna chyba serveru"
#define EI_Obsazeno     97
#define ES_Obsazeno     "Neni misto"


