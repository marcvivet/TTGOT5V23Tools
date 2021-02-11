
#include "config.hpp"
#include "OpenWeatherManager.hpp"
#include "WiFiManager.hpp"
#include "TerminalManager.hpp"

#include <ArduinoJson.h>
#include <HTTPClient.h>

typedef struct { // For current Day and Day 1, 2, 3, etc
  int      Dt;
  String   Period;
  String   Icon;
  String   Trend;
  String   Main0;
  String   Forecast0;
  String   Forecast1;
  String   Forecast2;
  String   Description;
  String   Time;
  String   Country;
  float    lat;
  float    lon;
  float    Temperature;
  float    Feelslike;
  float    Humidity;
  float    High;
  float    Low;
  float    Winddir;
  float    Windspeed;
  float    Rainfall;
  float    Snowfall;
  float    Pop;
  float    Pressure;
  int      Cloudcover;
  int      Visibility;
  String      Sunrise;
  String      Sunset;
  int      Timezone;
} Forecast_record_type;

const int max_readings = 5;
Forecast_record_type  WxConditions[1];
Forecast_record_type  WxForecast[max_readings];

OpenWeatherManager& OpenWeatherManager::getInstance()
{
    static OpenWeatherManager instance;
    return instance;
}

OpenWeatherManager::OpenWeatherManager(void) {

}

bool OpenWeatherManager::getData(void) {
    TerminalManager& oTM = TerminalManager::getInstance();
    WiFiClient client;   // wifi client object

    String oRequest = "weather";
    //String oRequest = "forecast";

    const String units = "metric";//(OPEN_WEATHER_UNITS == "M" ? "metric" : "imperial");
    client.stop(); // close connection before sending a new request

    HTTPClient http;
    String uri = "/data/2.5/" + oRequest + "?q=" + OPEN_WEATHER_CITY + "," + OPEN_WEATHER_COUNTRY + "&APPID=" + OPEN_WEATHER_KEY + "&mode=json&units=" + OPEN_WEATHER_UNITS + "&lang=" + OPEN_WEATHER_LANGUAGE;
    if(oRequest != "weather")
    {
        uri += "&cnt=" + String(max_readings);
    }
    //http.begin(uri,test_root_ca); //HTTPS example connection
    http.begin(client, OPEN_WEATHER_SERVER, 80, uri);
    int httpCode = http.GET();
    if(httpCode == HTTP_CODE_OK) {
        if (!parseWeather(http.getStream(), oRequest)) return false;
        client.stop();
        http.end();
        return true;
    }
    else
    {
        oTM.printf("connection failed, error: %s", http.errorToString(httpCode).c_str());
        client.stop();
        http.end();
        return false;
    }
    http.end();
    return true;
}


bool OpenWeatherManager::parseWeather(WiFiClient& json, const String& Type) {
    TerminalManager& oTM = TerminalManager::getInstance();
    oTM.print(F("\nCreating object...and "));
  // allocate the JsonDocument
  DynamicJsonDocument doc(35 * 1024);
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);
  // Test if parsing succeeds.
  if (error) {
    oTM.print(F("deserializeJson() failed: "));
    oTM.println(error.c_str());
    return false;
  }
  // convert it to a JsonObject
  JsonObject root = doc.as<JsonObject>();
  oTM.println(" Decoding " + Type + " data");
  if (Type == "weather") {
    // All Serial.println statements are for diagnostic purposes and not required, remove if not needed
    WxConditions[0].lon         = root["coord"]["lon"].as<float>();              oTM.println(" Lon: "+String(WxConditions[0].lon));
    WxConditions[0].lat         = root["coord"]["lat"].as<float>();              oTM.println(" Lat: "+String(WxConditions[0].lat));
    WxConditions[0].Main0       = root["weather"][0]["main"].as<char*>();        oTM.println("Main: "+String(WxConditions[0].Main0));
    WxConditions[0].Forecast0   = root["weather"][0]["description"].as<char*>(); oTM.println("For0: "+String(WxConditions[0].Forecast0));
    WxConditions[0].Forecast1   = root["weather"][1]["description"].as<char*>(); oTM.println("For1: "+String(WxConditions[0].Forecast1));
    WxConditions[0].Forecast2   = root["weather"][2]["description"].as<char*>(); oTM.println("For2: "+String(WxConditions[0].Forecast2));
    WxConditions[0].Icon        = root["weather"][0]["icon"].as<char*>();        oTM.println("Icon: "+String(WxConditions[0].Icon));
    WxConditions[0].Temperature = root["main"]["temp"].as<float>();              oTM.println("Temp: "+String(WxConditions[0].Temperature));
    WxConditions[0].Pressure    = root["main"]["pressure"].as<float>();          oTM.println("Pres: "+String(WxConditions[0].Pressure));
    WxConditions[0].Humidity    = root["main"]["humidity"].as<float>();          oTM.println("Humi: "+String(WxConditions[0].Humidity));
    WxConditions[0].Low         = root["main"]["temp_min"].as<float>();          oTM.println("TLow: "+String(WxConditions[0].Low));
    WxConditions[0].High        = root["main"]["temp_max"].as<float>();          oTM.println("THig: "+String(WxConditions[0].High));
    WxConditions[0].Windspeed   = root["wind"]["speed"].as<float>();             oTM.println("WSpd: "+String(WxConditions[0].Windspeed));
    WxConditions[0].Winddir     = root["wind"]["deg"].as<float>();               oTM.println("WDir: "+String(WxConditions[0].Winddir));
    WxConditions[0].Cloudcover  = root["clouds"]["all"].as<int>();               oTM.println("CCov: "+String(WxConditions[0].Cloudcover)); // in % of cloud cover
    WxConditions[0].Visibility  = root["visibility"].as<int>();                  oTM.println("Visi: "+String(WxConditions[0].Visibility)); // in metres
    WxConditions[0].Rainfall    = root["rain"]["1h"].as<float>();                oTM.println("Rain: "+String(WxConditions[0].Rainfall));
    WxConditions[0].Snowfall    = root["snow"]["1h"].as<float>();                oTM.println("Snow: "+String(WxConditions[0].Snowfall));
    WxConditions[0].Country     = root["sys"]["country"].as<char*>();            oTM.println("Ctry: "+String(WxConditions[0].Country));
    WxConditions[0].Sunrise     = convertUnixTime(root["sys"]["sunrise"].as<int>());              oTM.println("SRis: "+String(WxConditions[0].Sunrise));
    WxConditions[0].Sunset      = convertUnixTime(root["sys"]["sunset"].as<int>());               oTM.println("SSet: "+String(WxConditions[0].Sunset));
    WxConditions[0].Timezone    = root["timezone"].as<int>();                    oTM.println("TZon: "+String(WxConditions[0].Timezone));  }
  if (Type == "forecast") {
    //Serial.println(json);
    oTM.print(F("\nReceiving Forecast period - ")); //------------------------------------------------
    JsonArray list                    = root["list"];
    for (byte r = 0; r < max_readings; r++) {
      Serial.println("\nPeriod-" + String(r) + "--------------");
      WxForecast[r].Dt                = list[r]["dt"].as<int>();                          oTM.println("DTim: "+String(WxForecast[r].Dt));
      WxForecast[r].Temperature       = list[r]["main"]["temp"].as<float>();              oTM.println("Temp: "+String(WxForecast[r].Temperature));
      WxForecast[r].Low               = list[r]["main"]["temp_min"].as<float>();          oTM.println("TLow: "+String(WxForecast[r].Low));
      WxForecast[r].High              = list[r]["main"]["temp_max"].as<float>();          oTM.println("THig: "+String(WxForecast[r].High));
      WxForecast[r].Pressure          = list[r]["main"]["pressure"].as<float>();          oTM.println("Pres: "+String(WxForecast[r].Pressure));
      WxForecast[r].Humidity          = list[r]["main"]["humidity"].as<float>();          oTM.println("Humi: "+String(WxForecast[r].Humidity));
      WxForecast[r].Forecast0         = list[r]["weather"][0]["main"].as<char*>();        oTM.println("For0: "+String(WxForecast[r].Forecast0));
      WxForecast[r].Forecast1         = list[r]["weather"][1]["main"].as<char*>();        oTM.println("For1: "+String(WxForecast[r].Forecast1));
      WxForecast[r].Forecast2         = list[r]["weather"][2]["main"].as<char*>();        oTM.println("For2: "+String(WxForecast[r].Forecast2));
      WxForecast[r].Icon              = list[r]["weather"][0]["icon"].as<char*>();        oTM.println("Icon: "+String(WxForecast[r].Icon));
      WxForecast[r].Description       = list[r]["weather"][0]["description"].as<char*>(); oTM.println("Desc: "+String(WxForecast[r].Description));
      WxForecast[r].Cloudcover        = list[r]["clouds"]["all"].as<int>();               oTM.println("CCov: "+String(WxForecast[r].Cloudcover)); // in % of cloud cover
      WxForecast[r].Windspeed         = list[r]["wind"]["speed"].as<float>();             oTM.println("WSpd: "+String(WxForecast[r].Windspeed));
      WxForecast[r].Winddir           = list[r]["wind"]["deg"].as<float>();               oTM.println("WDir: "+String(WxForecast[r].Winddir));
      WxForecast[r].Rainfall          = list[r]["rain"]["3h"].as<float>();                oTM.println("Rain: "+String(WxForecast[r].Rainfall));
      WxForecast[r].Snowfall          = list[r]["snow"]["3h"].as<float>();                oTM.println("Snow: "+String(WxForecast[r].Snowfall));
      WxForecast[r].Pop               = list[r]["pop"].as<float>();                       oTM.println("Pop:  "+String(WxForecast[r].Pop));
      WxForecast[r].Period            = list[r]["dt_txt"].as<char*>();                    oTM.println("Peri: "+String(WxForecast[r].Period));
    }
    //------------------------------------------
    float pressure_trend = WxForecast[0].Pressure - WxForecast[2].Pressure; // Measure pressure slope between ~now and later
    pressure_trend = ((int)(pressure_trend * 10)) / 10.0; // Remove any small variations less than 0.1
    WxConditions[0].Trend = "0";
    if (pressure_trend > 0)  WxConditions[0].Trend = "+";
    if (pressure_trend < 0)  WxConditions[0].Trend = "-";
    if (pressure_trend == 0) WxConditions[0].Trend = "0";

    //if (Units == "I") Convert_Readings_to_Imperial();
  }
  return true;
}


String OpenWeatherManager::convertUnixTime(const int unix_time) const {
  // Returns either '21:12  ' or ' 09:12pm' depending on Units mode
  time_t tm = unix_time;
  struct tm *now_tm = localtime(&tm);
  char output[40];
  //if (Units == "M") {
  strftime(output, sizeof(output), "%H:%M %d/%m/%y", now_tm);
  /*}
  else {
    strftime(output, sizeof(output), "%I:%M%P %m/%d/%y", now_tm);
  }*/
  return output;
}