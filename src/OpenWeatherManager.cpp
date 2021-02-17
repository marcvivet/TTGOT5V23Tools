
#include "config.hpp"
#include "OpenWeatherManager.hpp"
#include "WiFiManager.hpp"
#include "TerminalManager.hpp"


#include <HTTPClient.h>


void OpenWeatherManager::Weather::setValuesFromJSON(const JsonObject& p_oInput, const bool p_bVervose) {
    
    lon = p_oInput["coord"]["lon"].as<float>();
    lat = p_oInput["coord"]["lat"].as<float>();
    Main0 = p_oInput["weather"][0]["main"].as<char *>();
    Forecast0 = p_oInput["weather"][0]["description"].as<char *>();
    Forecast1 = p_oInput["weather"][1]["description"].as<char *>();
    Forecast2 = p_oInput["weather"][2]["description"].as<char *>();
    Icon = p_oInput["weather"][0]["icon"].as<char *>();
    IconName = convertIconToName(Icon);
    Temperature = p_oInput["main"]["temp"].as<float>();
    Pressure = p_oInput["main"]["pressure"].as<float>();
    Humidity = p_oInput["main"]["humidity"].as<float>();
    Low = p_oInput["main"]["temp_min"].as<float>();
    High = p_oInput["main"]["temp_max"].as<float>();
    Windspeed = p_oInput["wind"]["speed"].as<float>();
    Winddir = p_oInput["wind"]["deg"].as<float>();
    Cloudcover = p_oInput["clouds"]["all"].as<int>();
    Visibility = p_oInput["visibility"].as<int>();
    Rainfall = p_oInput["rain"]["1h"].as<float>();
    Snowfall = p_oInput["snow"]["1h"].as<float>();
    Country = p_oInput["sys"]["country"].as<char *>();
    Sunrise = convertUnixTime(p_oInput["sys"]["sunrise"].as<int>());
    Sunset = convertUnixTime(p_oInput["sys"]["sunset"].as<int>());
    Timezone = p_oInput["timezone"].as<int>();
    
    if (p_bVervose) {
        show();
    }
}

void OpenWeatherManager::Weather::show(void) const {
    TerminalManager &oTM = TerminalManager::getInstance();

    oTM.println(" Lon: " + String(lon));
    oTM.println(" Lat: " + String(lat));
    oTM.println("Main: " + String(Main0));
    oTM.println("For0: " + String(Forecast0));
    oTM.println("For1: " + String(Forecast1));
    oTM.println("For2: " + String(Forecast2));
    oTM.println("Icon: " + String(Icon));
    oTM.println("IconName: " + String(IconName));
    oTM.println("Temp: " + String(Temperature));
    oTM.println("Pres: " + String(Pressure));
    oTM.println("Humi: " + String(Humidity));
    oTM.println("TLow: " + String(Low));
    oTM.println("THig: " + String(High));
    oTM.println("WSpd: " + String(Windspeed));
    oTM.println("WDir: " + String(Winddir));
    oTM.println("CCov: " + String(Cloudcover)); // in % of cloud cover
    oTM.println("Visi: " + String(Visibility)); // in metres
    oTM.println("Rain: " + String(Rainfall));
    oTM.println("Snow: " + String(Snowfall));
    oTM.println("Ctry: " + String(Country));
    oTM.println("SRis: " + String(Sunrise));
    oTM.println("SSet: " + String(Sunset));
    oTM.println("TZon: " + String(Timezone));
}

String OpenWeatherManager::Weather::convertUnixTime(const int unix_time) const
{
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

String OpenWeatherManager::Weather::convertIconToName(const String &IconName) const
{
    if (IconName == "01d" || IconName == "01n")
        return "Sunny";
    else if (IconName == "02d" || IconName == "02n")
        return "MostlySunny";
    else if (IconName == "03d" || IconName == "03n")
        return "Cloudy";
    else if (IconName == "04d" || IconName == "04n")
        return "MostlySunny";
    else if (IconName == "09d" || IconName == "09n")
        return "ChanceRain";
    else if (IconName == "10d" || IconName == "10n")
        return "Rain";
    else if (IconName == "11d" || IconName == "11n")
        return "Tstorms";
    else if (IconName == "13d" || IconName == "13n")
        return "Snow";
    else if (IconName == "50d")
        return "Haze";
    else if (IconName == "50n")
        return "Fog";
    else
        return "Nodata";
}

OpenWeatherManager &OpenWeatherManager::getInstance()
{
    static OpenWeatherManager instance;
    return instance;
}

OpenWeatherManager::OpenWeatherManager(void)
{
}

bool OpenWeatherManager::update(const OpenWeatherManager::RequestType p_eRequestType, const bool p_bVervose)
{
    TerminalManager &oTM = TerminalManager::getInstance();
    WiFiManager &oWM = WiFiManager::getInstance(); 

    String oRequest = (p_eRequestType == RequestType::WEATHER)? "weather" : "forecast";

    oTM.println("Quering for -> " + oRequest);
   
    String uri = "/data/2.5/" + oRequest + "?q=" + OPEN_WEATHER_CITY + "," + OPEN_WEATHER_COUNTRY + "&APPID=" + OPEN_WEATHER_KEY + "&mode=json&units=" + OPEN_WEATHER_UNITS + "&lang=" + OPEN_WEATHER_LANGUAGE;

    if (p_eRequestType == RequestType::FORECAST)
    {
        uri += "&cnt=" + String(FORECAST_DAYS);
    }

    String response = oWM.httpGET(OPEN_WEATHER_SERVER, uri);
    if (response.isEmpty()) return false;
    parseWeather(response, p_eRequestType, p_bVervose);

    return true;
}

bool OpenWeatherManager::update(const bool p_bVervose)
{
    bool bResponse = true;
    bResponse &= update(RequestType::WEATHER, p_bVervose);
    bResponse &= update(RequestType::FORECAST, p_bVervose);

    return bResponse;
}

bool OpenWeatherManager::parseWeather(const String &p_oResponse, const OpenWeatherManager::RequestType p_eRequestType, const bool p_bVervose)
{
    TerminalManager &oTM = TerminalManager::getInstance();
    oTM.println(F("\nCreating object..."));
    // allocate the JsonDocument
    DynamicJsonDocument doc(35 * 1024);
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, p_oResponse);
    // Test if parsing succeeds.
    if (error)
    {
        oTM.print(F("deserializeJson() failed: "));
        oTM.println(error.c_str());
        return false;
    }
    // convert it to a JsonObject
    JsonObject root = doc.as<JsonObject>();
    oTM.println(" Decoding data");
    if (p_eRequestType == RequestType::WEATHER)
    {
        m_oCurrentWather.setValuesFromJSON(root);
    } else {
        oTM.println("Receiving Forecast"); 
        JsonArray list = root["list"];
        for (uint8_t r = 0; r < FORECAST_DAYS; ++r)
        {
            oTM.printf("Day %d\n", r + 1); 
            m_oForecastWeather[r].setValuesFromJSON(list[r], p_bVervose);
        }
        //------------------------------------------
        float pressure_trend = m_oForecastWeather[0].Pressure - m_oForecastWeather[2].Pressure; // Measure pressure slope between ~now and later
        pressure_trend = ((int)(pressure_trend * 10)) / 10.0;                   // Remove any small variations less than 0.1
        m_oCurrentWather.Trend = "0";
        if (pressure_trend > 0)
            m_oCurrentWather.Trend = "+";
        if (pressure_trend < 0)
            m_oCurrentWather.Trend = "-";
        if (pressure_trend == 0)
            m_oCurrentWather.Trend = "0";

        //if (Units == "I") Convert_Readings_to_Imperial();
    }
    return true;
}

const OpenWeatherManager::Weather& OpenWeatherManager::getWeather(void) const {
    return m_oCurrentWather;
}

const OpenWeatherManager::Forecast& OpenWeatherManager::getForecast(void) const {
    return m_oForecastWeather;
}