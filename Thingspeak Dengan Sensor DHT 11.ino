#include <WiFi.h>
#include "DHT.h"
#include "ThingSpeak.h"

//-----netwrok credentials
char* ssid = "HernaningErika"; //enter SSID
char* passphrase = "TE3B12345"; // enter the password
WiFiServer server(80);
WiFiClient client;

//-----ThingSpeak channel details
unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "EIZSEVF61XGR5GUI";

//----- Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

// Float
float temperature;
float humadity;

//----DHT declarations
#define DHTPIN 4 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11

// Initializing the DHT11 sensor.
DHT dht(DHTPIN, DHTTYPE);
 
void setup()
{
Serial.begin(115200); //Initialize serial
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, passphrase);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}

// Print local IP address and start web server
Serial.println("");
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
server.begin();

//----nitialize dht11
dht.begin();
ThingSpeak.begin(client); // Initialize ThingSpeak
}

void loop()
{
if ((millis() - lastTime) > timerDelay)
{
delay(2500);

// Reading temperature or humidity takes about 250 milliseconds!
float humadity = dht.readHumidity();
float temperature = dht.readTemperature();
if (isnan(humadity) || isnan(temperature)) {
Serial.println(F("Failed to read from DHT sensor!"));
return;
}

Serial.print("Temperature (ºC): ");
Serial.print(temperature);
Serial.print("Humidity (%): ");
Serial.println(humadity);

ThingSpeak.setField(1, temperature);
ThingSpeak.setField(2, humadity);

// Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different

// pieces of information in a channel. Here, we write to field 1.
int x = ThingSpeak.writeFields(myChannelNumber,myWriteAPIKey);
if(x == 200){
Serial.println("Channel update successful.");
}
else{
Serial.println("Problem updating channel. HTTP error code " + String(x));
}
lastTime = millis();
}
}