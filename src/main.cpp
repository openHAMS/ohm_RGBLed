#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <AsyncMqttClient.h>
#include <stdlib.h>

#define WIFI_SSID	"RacerNet"
#define WIFI_PASS	"Pink.Flamingo"

#define LED_RED		12
#define LED_GREEN	14
#define LED_BLUE	13

struct Color
{
	byte R;
	byte G;
	byte B;
	byte A;
};


Color ledColor = { 0, 0, 0, 0 };
Color ColorParse(char* data)
{
	Color c;
	char r[3];
	char g[3];
	char b[3];
	char a[3];

	String s = String(data);
	s.substring(0, 2).toCharArray(r, 3);
	s.substring(2, 4).toCharArray(g, 3);
	s.substring(4, 6).toCharArray(b, 3);
	s.substring(6, 8).toCharArray(a, 3);



	c.R = strtol(r, NULL, 16); Serial.println(strtol(r, NULL, 16));
	c.G = strtol(g, NULL, 16);
	c.B = strtol(b, NULL, 16);
	c.A = strtol(a, NULL, 16);

	return c;
}

int Map(int input, int input_start, int input_end, int output_start, int output_end)
{
	int input_range = input_end - input_start;
	int output_range = output_end - output_start;
	return (input - input_start)*output_range / input_range + output_start;
}

int Led(Color color)
{
	analogWrite(LED_RED, Map(color.R * color.A, 0, 255*255, 0, PWMRANGE));
	analogWrite(LED_GREEN, Map(color.G * color.A, 0, 255*255, 0, PWMRANGE));
	analogWrite(LED_BLUE, Map(color.B * color.A, 0, 255*255, 0, PWMRANGE));

	Serial.println(Map(color.R * color.A, 0, 255*255, 0, 1023));

	return 1;
}


AsyncMqttClient mqttClient;

void onMqttConnect()
{

	uint16_t packetIdSub = mqttClient.subscribe("rcr/rgbled", 2);
	/*
	Serial.println("** Connected to the broker **");
	uint16_t packetIdSub = mqttClient.subscribe("test/lol", 2);
	Serial.print("Subscribing at QoS 2, packetId: ");
	Serial.println(packetIdSub);
	mqttClient.publish("test/lol", 0, true, "test 1");
	Serial.println("Publishing at QoS 0");
	uint16_t packetIdPub1 = mqttClient.publish("test/lol", 1, true, "test 2");
	Serial.print("Publishing at QoS 1, packetId: ");
	Serial.println(packetIdPub1);
	uint16_t packetIdPub2 = mqttClient.publish("test/lol", 2, true, "test 3");
	Serial.print("Publishing at QoS 2, packetId: ");
	Serial.println(packetIdPub2);
	*/
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
	/*
	analogWrite(LED_RED, PWMRANGE);
	analogWrite(LED_GREEN, 0);
	analogWrite(LED_BLUE, 0);
	*/
	//Serial.println("** Disconnected from the broker **");
	//Serial.println("Reconnecting to MQTT...");
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

	Serial.println(payload);
	//Color color = ColorParse(payload);
	//Serial.println("** Publish received **");
	//Serial.print(color.R);
	//Serial.print(":");
	//Serial.print(color.G);
	//Serial.print(":");
	//Serial.print(color.B);
	//Serial.print(":");
	//Serial.println(color.A);
	Led(ColorParse(payload));

	/*
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
	*/
}

void onMqttPublish(uint16_t packetId)
{

	Serial.println("** Publish acknowledged **");
	Serial.print("  packetId: ");
	Serial.println(packetId);

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
	WiFi.begin("RacerNet", "Pink.Flamingo");

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
	Serial.println("Connecting to MQTT...");
	Serial.println("Connecting to MQTT...");
	mqttClient.connect();

	Serial.println("Connected!");
}

void loop()
{

}
