#define BUTTON_PRESSED LOW

#define BUTTON_1 2 //define button pin to D2
#define BUTTON_2 3 //define button pin to D3

#define WATER_LEVEL 4 //define water level sensor to pin D4

#define MOTOR_1 5 //define motor to pin D5
#define MOTOR_2 6 //define motor to pin D6
#define MOTOR_3 8 //define motor to pin D8

#define MOTION 7 // define motion detection to pin D7

#define BUZZER 11 // define the buzzer with pin D11

bool isButton1();
bool isButton2();
bool isMotion();
void setupDioUtil();
void setBuzzer(bool on);