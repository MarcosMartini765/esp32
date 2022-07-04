#include "my_keypad.h"

MyKeypad::MyKeypad()
{
}

int MyKeypad::read()
{
    const char keyChar = kpd.getKey();

    if (keyChar == NO_KEY)
    {
        return IDLE;
    }

    const String key = String(keyChar);

    if (key != NULL)
    {
        Serial.print("Key: ");
        Serial.println(key);

        if (key == "#")
        {
            Serial.println("Check password");
            const String check = input;
            input = "";

            return check == password ? SHOULD_OPEN : IDLE;
        }
        else if (key == "C")
        {
            return SHOULD_CLOSE;
        }
        input = input + key;
    }

    return IDLE;
}