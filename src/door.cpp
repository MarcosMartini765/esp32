#include "door.h"
#include <ESP32Servo.h>

Door::Door(bool closed)
{
    isClosed = closed;
}

void Door::setIsClosed(bool closed)
{
    isClosed = closed;

    write(closed ? 180 : 0);
}

void Door::setup(int pin)
{
    attach(pin);
    write(isClosed ? 180 : 0);
}

bool Door::getIsClosed()
{
    double distance = distanceSensor.measureDistanceCm();
    while (distance < 2 || distance > 10)
    {
        Serial.print("Trash value :P ");
        Serial.println(distance);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        distance = distanceSensor.measureDistanceCm();
    }

    Serial.print("Good value :) ");
    Serial.println(distance);

    return distance >= 2 && distance <= 4.8;
}

String Door::getDeviceId()
{
    return deviceId;
}