#define BUTTON_PRESSED LOW

#define BUTTON_1 2 //define button pin D2
#define BUTTON_2 3 //define button pin D3

#define MOTION 7 // define motion to pin D1

#define BUZZER 11 // define the buzzer with pin D11

bool isButton1();
bool isButton2();
bool isMotion();
void setupDioUtil();
void setBuzzer(bool on);