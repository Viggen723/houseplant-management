#include <LCD_I2C.h>
#include <DHT.h>

int const soilPin = 36;
int const dryLedPin = 2;
int const wetLedPin = 0;

#define tempPin 15
#define DHTTYPE DHT11
DHT dht(tempPin, DHTTYPE);

LCD_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);

  lcd.begin();
  lcd.backlight();

  dht.begin();

  pinMode(soilPin, INPUT);
  analogReadResolution(12);

  pinMode(dryLedPin, OUTPUT);
  pinMode(wetLedPin, OUTPUT);
  pinMode(tempPin, INPUT);
}

void loop()
{
  lcd.clear();
  float value = analogRead(soilPin);
  float voltage = value * (5.0 / 4095);
  float percentage = value / 2700;

  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (percentage < .75)
  {
    digitalWrite(dryLedPin, 0);
    digitalWrite(wetLedPin, 1);
  }
  else
  {
    digitalWrite(dryLedPin, 1);
    digitalWrite(wetLedPin, 0);
  }

  Serial.print("Value: ");
  Serial.println(value);
  Serial.print("Percentage (Higher = dryer): ");
  Serial.println(percentage);

  lcd.print("Percentage: ");
  lcd.print(percentage);
  lcd.setCursor(0, 2);

  lcd.print(temp);
  lcd.print("C ");
  lcd.print(humidity);
  lcd.print("%");
  

  delay(1500);
}
