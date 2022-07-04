#include <WiFiClientSecure.h>
#include <MQTT.h>

class MyWifi
{

public:
    MyWifi();
    bool connect();
    bool isConnected();
    void setCredentials(const char *ssid, const char *password);
    void publish(String message);
    void loop();
    void setCallback(void handler(String &topic, String &payload));

private:
    char *ssid;
    char *password;
    int connectDelay[6] = {10, 60, 120, 300, 600, 1200};
    int index = 0;
    MQTTClient client;
    WiFiClientSecure secure;
    bool connectAWS();
};