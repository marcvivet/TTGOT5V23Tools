
#include "config.hpp"
#include "WiFiManager.hpp"
#include "TerminalManager.hpp"


WiFiManager& WiFiManager::getInstance()
{
    static WiFiManager instance;
    return instance;
}

WiFiManager::WiFiManager(void) {

}

const bool WiFiManager::connect(void) const {
    if (WiFi.status() == WL_CONNECTED) {
        return true;
    }

    TerminalManager& oTM = TerminalManager::getInstance();

    WiFi.mode(WIFI_STA);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    oTM.printAutoFlush("Connecting .");

    for (int i = 0; i < 10; ++i) {
        if (WiFi.status() != WL_CONNECTED) {
            oTM.printAutoFlush(".");
            delay(500);
            continue;
        }
        break;
    }

    if (WiFi.status() == WL_CONNECTED) {
        oTM.println(" OK");
        oTM.print("IP: ");
        oTM.println(WiFi.localIP());

        int8_t dBm = WiFi.RSSI();
        oTM.print("Strength: ");
        oTM.print(dBm);
        oTM.print("dBm ");
        // -30 dBm	Amazing
        // -67 dBm	Very Good
        // -70 dBm	Okay
        // -80 dBm	Not Good
        // -90 dBm	Unusable
        if (dBm >= -30) {
            oTM.println("Amazing");
        } else {
            if (dBm >= -67) {
                oTM.println("Very Good");
            } else {
                if (dBm >= -70) {
                    oTM.println("Okay");
                } else {
                    if (dBm >= -80) {
                        oTM.println("Not Good");
                    } else {
                        oTM.println("Unusable");
                    }
                }
            }
        }
        return true;
    }
    
    oTM.println(" FAIL");
    return false;
}

const bool WiFiManager::disconnect(void) const {
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    return true;
}