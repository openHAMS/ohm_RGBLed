#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <AsyncMqttClient.h>
#include <stdlib.h>

#include "Color.hpp"
#include "RGBLed.hpp"

#define DEBUG		false

#define WIFI_SSID	"<SSID>"
#define WIFI_PASS	"<PASS>"

#define LED_RED		12
#define LED_GREEN	14
#define LED_BLUE	13


AsyncMqttClient mqttClient;
RGBLed led = RGBLed(LED_RED, LED_GREEN, LED_BLUE);


void onMqttConnect()
{
	Serial.println("** Connected to the broker **");
	// [home]/[room]/[device]/[node]/[property]
	uint16_t packetIdSub = mqttClient.subscribe("rcr/rcr/desk/rgbled/set", 1);
	Serial.print("Subscribing at QoS 1, packetId: ");
	Serial.println(packetIdSub);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
	Serial.println("** Disconnected from the broker **");
	Serial.println("Reconnecting to MQTT...");
	mqttClient.connect();
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos)
{
	Serial.println("** Subscribe acknowledged **");
	Serial.print("  packetId: ");
	Serial.println(packetId);
	Serial.print("  qos: ");
	Serial.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId)
{
	Serial.println("** Unsubscribe acknowledged **");
	Serial.print("  packetId: ");
	Serial.println(packetId);
}

void onMqttMessage(char* topic, char* payload, uint8_t qos, size_t len, size_t index, size_t total)
{
	if (DEBUG)
	{
		Serial.println("** Publish received **");
		Serial.print("  payload: ");
		Serial.println(payload);
		Serial.print("  color: ");
		Color color = ColorParse(payload);
		Serial.print(color.R);
		Serial.print(":");
		Serial.print(color.G);
		Serial.print(":");
		Serial.print(color.B);
		Serial.print(":");
		Serial.println(color.A);
		Serial.print("  topic: ");
		Serial.println(topic);
		Serial.print("  qos: ");
		Serial.println(qos);
		Serial.print("  len: ");
		Serial.println(len);
		Serial.print("  index: ");
		Serial.println(index);
		Serial.print("  total: ");
		Serial.println(total);
	}
	led.setColor(ColorParse(payload));
	mqttClient.publish("rcr/rcr/desk/rgbled/status", 1, true, payload);
}

void onMqttPublish(uint16_t packetId)
{
	if (DEBUG)
	{
		Serial.println("** Publish acknowledged **");
		Serial.print("  packetId: ");
		Serial.println(packetId);
	}
}

void setup()
{
	pinMode(LED_RED, OUTPUT);
	pinMode(LED_GREEN, OUTPUT);
	pinMode(LED_BLUE, OUTPUT);

	Serial.begin(115200);
	Serial.println();
	Serial.println();
	WiFi.persistent(false);
	WiFi.mode(WIFI_STA);
	Serial.print("Connecting to Wi-Fi");
	WiFi.begin(WIFI_SSID, WIFI_PASS);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}

	Serial.println(" OK");

	mqttClient.onConnect(onMqttConnect);
	mqttClient.onDisconnect(onMqttDisconnect);
	mqttClient.onSubscribe(onMqttSubscribe);
	mqttClient.onUnsubscribe(onMqttUnsubscribe);
	mqttClient.onMessage(onMqttMessage);
	mqttClient.onPublish(onMqttPublish);
	mqttClient.setServer(IPAddress(192, 168, 1, 111), 1883);
	mqttClient.setKeepAlive(5).setWill("topic/online", 2, true, "no").setCredentials("username", "password").setClientId("myDevice");
	Serial.print("Connecting to MQTT...");
	mqttClient.connect();
	Serial.println("Connected!");
}

void loop()
{

}
