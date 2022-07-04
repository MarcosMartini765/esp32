#include <Arduino.h>
#include <my_wifi.h>
#include <ArduinoJson.h>
#include <door.h>
#include <fingerprint.h>
#include <my_keypad.h>

const uint8_t pin = 2;

MyWifi wifi = MyWifi();
MyKeypad keypad = MyKeypad();
FingerPrint finger;
Door door;

void suspendTask100ms()
{
  vTaskDelay(100 / portTICK_PERIOD_MS);
}

void suspendTask200ms()
{
  vTaskDelay(200 / portTICK_PERIOD_MS);
}

void suspendTask1000ms()
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void suspendTask2000ms()
{
  vTaskDelay(2000 / portTICK_PERIOD_MS);
}

void initializeLed()
{
  pinMode(pin, OUTPUT);
}

void turnOnLed()
{
  digitalWrite(pin, HIGH);
}

void turnOffLed()
{
  digitalWrite(pin, LOW);
}

void beginSerial115200()
{
  Serial.begin(115200);
}

void printMessage(String message)
{
  Serial.println(message);
}

void openDoor()
{
  door.setIsClosed(false);
}

void closeDoor()
{
  door.setIsClosed(true);
}

String createJsonPayload()
{
  StaticJsonDocument<200> doc;
  doc["deviceId"] = door.getDeviceId();
  doc["state"] = door.getIsClosed();
  doc["from"] = 32;

  String payload;
  serializeJson(doc, payload);

  return payload;
}

void publishDoorState()
{
  if (wifi.isConnected())
  {
    wifi.publish(createJsonPayload());
    printMessage("Payload published.");
  }
  else
  {
    printMessage("Could not publish state: No internet.");
  }
}

void loop2(void *parameter)
{
  for (;;)
  {
    const bool foundMatch = finger.read();

    if (foundMatch)
    {
      printMessage("found match");

      openDoor();

      suspendTask2000ms(); //wait for door to open

      publishDoorState();
    }
    suspendTask100ms();
  }
}

void blinkLed(void *parameter)
{
  for (;;)
  {
    turnOnLed();
    suspendTask1000ms();
    turnOffLed();
    suspendTask1000ms();
  }
}

void loop3(void *parameter)
{
  for (;;)
  {
    const int state = keypad.read();

    switch (state)
    {
    case MyKeypad::SHOULD_OPEN:
    {
      printMessage("Openning door...");

      openDoor();

      suspendTask2000ms(); //wait for door to open

      publishDoorState();
      break;
    }
    case MyKeypad::SHOULD_CLOSE:
    {
      printMessage("Closing door...");

      closeDoor();

      suspendTask2000ms(); //wait for door to close

      publishDoorState();
      break;
    }
    default:
      break;
    }
    suspendTask200ms();
  }
}

void launchFingerPrintTask()
{
  xTaskCreatePinnedToCore(loop2, "loop2", 2048, NULL, 1, NULL, 0);
}

void launchLedTask()
{
  xTaskCreate(blinkLed, "blinkLed", 1024, NULL, 0, NULL);
}

void launchKeyBoardTask()
{
  xTaskCreatePinnedToCore(loop3, "loop3", 2048, NULL, 1, NULL, 0);
}

void launchTasks()
{
  launchFingerPrintTask();
  launchLedTask();
  launchKeyBoardTask();
}

void wifiLoop()
{
  if (wifi.isConnected())
  {
    wifi.loop();
  }
}

void connectWifiIfDisconnected()
{
  if (!wifi.isConnected())
  {
    if (wifi.connect())
    {
      publishDoorState();
    }
  }
}

void messageHandler(String &topic, String &payload)
{
  Serial.println("incoming: " + topic + " - " + payload);

  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload);
  if (!error)
  {
    const bool locked = doc["state"];
    const int from = doc["from"];

    if (from != 32)
    {
      door.setIsClosed(locked);
    }
  }
}

void initializeWifi()
{
  wifi.setCallback(messageHandler);
}

void initializeDoor()
{
  door.setup(18);
}

void initializeFinger()
{
  finger.init();
}

void setup()
{
  // put your setup code here, to run once:
  beginSerial115200();

  setCpuFrequencyMhz(80);

  initializeWifi();
  initializeDoor();
  initializeFinger();
  initializeLed();

  launchTasks();
}

void loop()
{
  // put your main code here, to run repeatedly:
  connectWifiIfDisconnected();
  wifiLoop();
  suspendTask100ms();
}