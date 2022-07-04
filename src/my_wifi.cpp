#include <my_wifi.h>
#include <WiFi.h>
#include <secrets.h>

MyWifi::MyWifi()
{
}

bool MyWifi::connect()
{
    setCredentials(MYWIFI_SSID, MYWIFI_PASSWORD);

    if (index != 0)
    {
        Serial.print("Waiting ");
        Serial.print(connectDelay[index - 1]);
        Serial.println("s");
        vTaskDelay((connectDelay[index - 1] * 1000) / portTICK_PERIOD_MS);
    }

    Serial.println("\nWIFI initialization");

    secure.setCACert(AWS_CERT_CA);
    secure.setCertificate(AWS_CERT_CRT);
    secure.setPrivateKey(AWS_CERT_PRIVATE);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    for (int i = 0; i < 20; ++i)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            vTaskDelay(500 / portTICK_PERIOD_MS);
            Serial.println("Connecting WiFi");
            continue;
        }
        break;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("WIFI Connected");
        index = 0;
        return connectAWS();
    }

    Serial.println("Failed to connect WIFI");
    if (index < 6)
    {
        ++index;
    }
    return false;
}

bool MyWifi::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

void MyWifi::setCredentials(const char *name, const char *pass)
{
    ssid = (char *)name;
    password = (char *)pass;
}

bool MyWifi::connectAWS()
{
    Serial.println("Connecting to AWS");

    client.begin(AWS_IOT_ENDPOINT, 8883, secure);

    for (int i = 0; i < 10; ++i)
    {
        if (!client.connect(THINGNAME))
        {
            vTaskDelay(500 / portTICK_PERIOD_MS);
            Serial.println("Connecting to topic");
            continue;
        }
        break;
    }

    if (client.connected())
    {
        Serial.println("AWS connected");
        client.subscribe(THING_SUBSCRIPTION_URL);
        return true;
    }

    Serial.println("Failed to connect to AWS");
    return false;
}

void MyWifi::loop()
{
    client.loop();
}

void MyWifi::publish(String message)
{
    client.publish(THING_SUBSCRIPTION_URL, message);
}

void MyWifi::setCallback(void handler(String &topic, String &payload))
{
    client.onMessage(handler);
}