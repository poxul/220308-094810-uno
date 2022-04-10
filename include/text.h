
#define MAX_ORIGIN_STRING_SIZE 12

enum OriginText
{
	ORIGIN_TEMPERATURE,
	ORIGIN_HUMIDITY,
	ORIGIN_PLANT,
	ORIGIN_WATER,
	ORIGIN_PUMP,
	ORIGIN_DEVICE,
	ORIGIN_MAX
};

const char origin[ORIGIN_MAX][MAX_ORIGIN_STRING_SIZE] =
	{"Temperatur",
	 "Luftfeuchte",
	 "Pflanze",
	 "Wasser",
	 "Pumpe",
	 "Geraet"};


#define MAX_TEXT_STRING_SIZE 12

enum ValueText
{
	TEXT_DRY,
	TEXT_HUMID,
	TEXT_OK,
	TEXT_WET,
	TEXT_X,
	TEXT_WATERING,
	TEXT_FILL,
	TEXT_RUNNING,
	TEXT_STOPPED,
	TEXT_UNKNOWN,
	TEXT_UNIT_DEGR_C,
	TEXT_UNIT_PERCENT,
	TEXT_MAX
};

const char text[TEXT_MAX][MAX_TEXT_STRING_SIZE] =
	{"trocken",
	 "feucht",
	 "ok",
	 "nass",
	 "???",
	 "bewaessern!",
	 "auffuellen!",
	 "laeuft!",
	 "angehalten!",
	 "unbekannt!",
	 "'C",
	 "%"
};