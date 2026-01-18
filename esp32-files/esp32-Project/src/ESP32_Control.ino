#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "HTTPClient.h"
#include "DHT.h"

const char* ssid = "";
const char* password = "";

String URL = "http:///houseplant_project/data.php";

const int ledPin = 2;
const int soilPin = 36;
const int DHTPin = 21;
const int DHTTYPE = DHT11;

DHT dht(DHTPin, DHTTYPE);

String ledState;
String soilDryness = "0.0";
String temperature = "0.0";
String humidity = "0.0";

AsyncWebServer server(80);

// This function handles the string that will be posted to the HTML
String processor(const String& var)
{
  if (var == "STATE")
  {
    if (digitalRead(ledPin))
      ledState = "ON";
    else
      ledState = "OFF";

    Serial.print("WEB LED state requested: ");
    Serial.println(ledState);

    return ledState;
  }

  if (var == "SOIL")
  {
    Serial.print("WEB Soil value requested: ");
    Serial.println(soilDryness);
    return soilDryness;
  }

  if (var == "TEMPERATURE")
  {
    Serial.print("WEB Soil value requested: ");
    Serial.println(temperature);
    return temperature;
  }

  if (var == "HUMIDITY")
  {
    Serial.print("WEB Soil value requested: ");
    Serial.println(humidity);
    return humidity;
  }

  return String();
}

void setup()
{
  Serial.begin(115200);
  delay(500);

  pinMode(ledPin, OUTPUT);
  pinMode(soilPin, INPUT);
  analogReadResolution(12);

  if (!SPIFFS.begin(true))
  {
    Serial.println("ERROR SPIFFS Mount Failed");
    return;
  }

  Serial.print("WIFI Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nWiFi connected");
  Serial.print("WIFI IP address: ");
  Serial.println(WiFi.localIP());

  // Sending the data to HTML 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, 1);
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, 0);
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/soil", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", soilDryness);
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", temperature);
  });

  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", humidity);
  });

  server.begin();
}

void loop()
{
  float rawValue = analogRead(soilPin);
  float percentage = (rawValue / 2700.0) * 100.0;
  soilDryness = String(percentage, 1);

  Serial.print("Soil dryness: ");
  Serial.print(soilDryness);
  Serial.println("%");
  
  float humidSum = 0.0;
  float tempSum = 0.0;

  // For-loops to average out any anomalies that are come from the sensor
  for (int i = 0; i < 10; i++)
  {
    humidSum += dht.readHumidity();
    delay(100);
  }

  for (int i = 0; i < 10; i++)
  {
    tempSum += dht.readTemperature();
    delay(100);
  }

  // Averaging the sums that were calculated by the for-loops
  float humid = humidSum / 10.0;
  float temp = tempSum / 10.0;

  // Ranges to reduce anomalies and defaulting to 0.0
  if (humid <= 100 || humid > 0) 
  {            
    humidity = String(humid, 1); 
  }

  if (temp <= 60 || temp > -10)
  {
    temperature = String(temp, 1);
  }

  Serial.print("Humidity: ");
  Serial.println(humidity);

  Serial.print("Temperature: ");
  Serial.println(temperature);

  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    String postData = "soil=" + soilDryness +
                      "&temp=" + temperature +
                      "&hum=" + humidity;

    Serial.print("HTTP Post data: ");
    Serial.println(postData);

    http.begin(URL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");  

    int httpResponseCode = http.POST(postData);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    http.end();
  }
  else
  {
    Serial.println("WIFI Disconnected!");
  }

  delay(300);
}
