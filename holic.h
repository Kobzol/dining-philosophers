//***************************************************************************
//
// Petr Olivka (c) 03/2013 pro projekt do predmetu OSY.
// Viz. http://poli.cs.vsb.cz/edu/osy
//
// Priklad klienta pro IPC problem 'Spici holic'
// 
//***************************************************************************

// Prikazy protokolu
#define CI_Vstup        80
#define CS_Vstup        "Prichazi zakaznik"
#define CI_Strihat      81
#define CS_Strihat      "Pojdte se strihat"
#define CI_Hotovo       82
#define CS_Hotovo       "Dostrihano, muzete odejit"

// Odpovedi protokolu
#define AI_MistoX       70
#define AS_MistoX       "Misto %d je v cekarne volne"
#define AI_Zakazka      71
#define AS_Zakazka      "Chci strihat po dobu %d"
#define AI_Nashledanou  72
#define AS_Nashledanou  "Nashledanou"

// Chyby
#define EI_Klient       90
#define ES_Klient       "Obecna chyba klienta"
#define EI_Server       91
#define ES_Server       "Obecna chyba serveru"
#define EI_Obsazeno     92
#define ES_Obsazeno     "Holicstvi je obsazene"


