void setupLCD();

void lcdShowAlarm(int value);
void lcdShowMode(int mode);
void lcdShowID(int value, bool active);

void lcdShowOriginIdx(int idx);
void lcdShowOrigin(const char *txt);

void lcdShowStateIdx(int idx);
void lcdShowValue(const char *txt);
void lcdShowSoilResult(int idx, int value);