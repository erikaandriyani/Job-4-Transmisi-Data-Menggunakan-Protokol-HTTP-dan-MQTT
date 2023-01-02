#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial 
#include <CayenneMQTTESP32.h>
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
const int ledPin = 5;

// WiFi network info.
char ssid[] = "HernaningErika";
char wifiPassword[] = "TEB12345";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "86d93fd0-7f71-11ed-8d53-d7cd1025126a";
char password[] = "f3157a2b11886d0ff51aeca1113dba27aafcf34d";
char clientID[] = "8902e120-7f72-11ed-b193-d9789b2af62b";

unsigned long lastMillis = 0;

void setup() {
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);  
  dht.begin();
}

void loop() {
	Cayenne.loop();
   delay(3000);
   float h = dht.readHumidity();
float t = dht.readTemperature();
   if (isnan(h) || isnan(t)){
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);


}

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{
	// Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
	Cayenne.virtualWrite(0, millis());
	// Some examples of other functions you can use to send data.
	Cayenne.celsiusWrite(1, 22.0);
	Cayenne.luxWrite(2, 700);
	Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
}

// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN(0)
{
  digitalWrite(ledPin, !getValue.asInt());
  digitalWrite(5, !getValue.asInt());
}
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}