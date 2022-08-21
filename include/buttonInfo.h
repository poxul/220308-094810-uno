#ifndef BUTTON_INFO_CLASS
#define BUTTON_INFO_CLASS

class ButtonInfo
{
public:        
    ButtonInfo();
    void read(bool v);

    bool isPressed();
    bool isToggled();

private:
    bool state;
    int toggeled;
    bool lastState;
};

#endif