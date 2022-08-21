#include <Arduino.h>
#include <Wire.h>

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "buttonInfo.h"

ButtonInfo::ButtonInfo()
{
    state = false;
    toggeled = false;
    lastState = false;
}

void ButtonInfo::read(bool isPressed)
{
    state = isPressed;
    if (isPressed != lastState)
    {
        lastState = isPressed;
        toggeled = true;
    }
    else
    {
        toggeled = false;
    }
}
