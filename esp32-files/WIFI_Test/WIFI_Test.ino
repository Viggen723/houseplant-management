#include <WiFi.h>

#define LOCAL_SSID "Cheryl and Cleo"
#define LOCAL_PASS "brace1255entire"

const char* ssid = LOCAL_SSID;
const char* password = LOCAL_PASS;

void setup()
 {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  Serial.print(".");
  Serial.println("Connected!");

  
}

void loop() 
{

}