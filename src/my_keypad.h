#include <Keypad.h>

#define numOfColsAndRows 4

class MyKeypad
{
private:
    const uint8_t greenLedPin = 4;
    const uint8_t redLedPin = 0;

    const String password = "0345";
    String input;

    byte rowPins[numOfColsAndRows] = {23, 12, 14, 27};
    byte colPins[numOfColsAndRows] = {26, 25, 33, 32};
    char keys[numOfColsAndRows][numOfColsAndRows] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'},
    };

    Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, numOfColsAndRows, numOfColsAndRows);

    void redLed();
    void greenLed();

public:
    MyKeypad();

    int read();

    static const int SHOULD_OPEN = 12;
    static const int SHOULD_CLOSE = 13;
    static const int IDLE = 14;
};