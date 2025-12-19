#include <DHT.h>

int const soilPin = 36;

#define tempPin 15
#define DHTTYPE DHT11
DHT dht(tempPin, DHTTYPE);

void setup() {
  Serial.begin(115200);

  dht.begin();

  pinMode(soilPin, INPUT);
  analogReadResolution(12);

  pinMode(tempPin, INPUT);
}

void loop()
{
  float value = analogRead(soilPin);
  float voltage = value * (5.0 / 4095);
  float percentage = value / 2700;

  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.print("Value: ");
  Serial.println(value);
  Serial.print("Percentage (Higher = dryer): ");
  Serial.println(percentage);
  
  delay(1500);
}
