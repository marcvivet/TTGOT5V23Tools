
#include "OpenWeatherManager.hpp"
#include "WiFiManager.hpp"
#include "TerminalManager.hpp"


OpenWeatherManager& OpenWeatherManager::getInstance()
{
    static OpenWeatherManager instance;
    return instance;
}

OpenWeatherManager::OpenWeatherManager(void) {

}
