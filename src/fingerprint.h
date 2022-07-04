#include <Adafruit_Fingerprint.h>

class FingerPrint : Adafruit_Fingerprint
{

public:
    FingerPrint();
    uint8_t getId();
    uint8_t enroll();
    void setId(uint8_t newId);
    void init();
    bool read();

private:
    uint8_t id = 1;
};
