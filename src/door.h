#include <ESP32Servo.h>
#include <HCSR04.h>

class Door : public Servo
{
public:
  Door(bool isClosed = true);

  String getDeviceId();
  void setup(int pin);
  void setIsClosed(bool isOpen);
  bool getIsClosed();

private:
  bool isClosed;
  String deviceId = "one_last_train_arthur";
  UltraSonicDistanceSensor distanceSensor = UltraSonicDistanceSensor(13, 9);
};