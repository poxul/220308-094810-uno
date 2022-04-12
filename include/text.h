#include "textUtil.h"

#define MAX_ORIGIN_STRING_SIZE 12
#define MAX_TEXT_STRING_SIZE 12

const char origin[ORIGIN_MAX][MAX_ORIGIN_STRING_SIZE] =
	{"Temperatur",
	 "Luftfeuchte",
	 "Pflanze",
	 "Wasser",
	 "Pumpe",
	 "Geraet"};

const char text[TEXT_MAX][MAX_TEXT_STRING_SIZE] =
	{
		"trocken",
		"feucht",
		"ok",
		"nass",
		"???",
		"bewaessern!",
		"auffuellen!",
		"laeuft!",
		"angehalten!",
		"unbekannt!"};