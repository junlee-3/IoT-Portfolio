
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
//#include <SPI.h>
#include <Wire.h>
  
const char*  ssid = "#";
const char*  password = "#";

WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  server.begin();
  Serial.println("Board Init!");
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("..."); 
  } 

  Serial.print("");
  Serial.println(WiFi.localIP());
  Serial.println("Wifi Connected");
} 
 
void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client2 = server.available();
  HTTPClient http;
 
  String jsonResponse;
  String maxTemp;
  String minTemp;
  int weatherCode;
  String weatherState;
  String wind;
  String rain;
  bool help;

  //We are getting a request for the weather from the OpenMeto API, the response will be sent in JSON.
  String requestURL = "http://api.open-meteo.com/v1/forecast?latitude=-35.2820&longitude=149.1286&daily=weathercode,temperature_2m_max,temperature_2m_min,rain_sum,windspeed_10m_max&timezone=Australia%2FSydney";
  
    http.begin(client2, requestURL.c_str());
    
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0){
      Serial.println(httpResponseCode);
      String response = http.getString();
      Serial.println(response);
      jsonResponse = response;

      DynamicJsonDocument parsedJson(1024);
      deserializeJson(parsedJson, response);

      // Assuming parsedJson is your JsonObject
      String maxTempString = String((const char*)parsedJson["daily"]["temperature_2m_max"][0]);
      //Serial.println(maxTempString);

      String minTempString = String((const char*)parsedJson["daily"]["temperature_2m_min"][0]);
      //Serial.println(minTempString);

      maxTempString = String(parsedJson["daily"]["temperature_2m_max"][0].as<float>(), 2);
      minTempString = String(parsedJson["daily"]["temperature_2m_min"][0].as<float>(), 2);


      int weatherCode = parsedJson["daily"]["weathercode"][0];
      Serial.println(weatherCode);


      //maxTemp = String(parsedJson["daily"]["temperature_2m_max"][0]);
      //minTemp = String(parsedJson["daily"]["temperature_2m_min"][0]);
      //wind = String(parsedJson["daily"]["windspeed_10m_max"][0]);
      //rain = String(parsedJson["daily"]["rain_sum"][0]);
      //weatherCode = String(parsedJson["daily"]["weathercode"][0]).toInt();

      if (weatherCode <= 1) {
        weatherState = "sunny";
      } else if (weatherCode == 2) {
        weatherState = "partly cloudy";
      } else if (weatherCode == 3) {
        weatherState = "overcast";
      } else if (weatherCode >= 61 && weatherCode < 95) {
        weatherState = "raining";
      } else if (weatherCode >= 95 && weatherCode < 99) {
        weatherState = "storming";
      } else {
        weatherState = "[WEATHERCODE_ERROR]";
      }
    
    maxTemp = maxTempString;
    minTemp = minTempString;
    

    http.end();
    //delay(2000);

    Serial.println("Weather in Canberra Today");
    Serial.println("Today it is " + weatherState);
    Serial.println("The Maximum Temprature is " + maxTemp + "Degrees Celcius");
    Serial.println("The Minimum Temprature is " + minTemp + "Degrees Celcius");

    delay(6000);

  }
}
