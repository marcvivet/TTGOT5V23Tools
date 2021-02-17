
#include "DisplayManager.hpp"

DisplayManager &DisplayManager::getInstance()
{
    static DisplayManager instance;
    return instance;
}

DisplayManager::DisplayManager(void) : m_oIo(SPI, EPD_CS, EPD_DC, EPD_RSET),
                                       m_oDisplay(m_oIo, EPD_RSET, EPD_BUSY)
{
    m_oDisplay.init();
    m_oDisplay.setRotation(1);
    m_oDisplay.fillScreen(GxEPD_WHITE);

    m_uiScreenWidth = m_oDisplay.width();
    m_uiScreenHeight = m_oDisplay.height();
}

uint16_t DisplayManager::getWidth(void) const
{
    return m_uiScreenWidth;
}

uint16_t DisplayManager::getHeight(void) const
{
    return m_uiScreenHeight;
}

void DisplayManager::clear(void)
{
    m_oDisplay.fillRect(0, 0, m_uiScreenWidth, m_uiScreenHeight, GxEPD_WHITE);
}

GxEPD_Class &DisplayManager::getDisplay(void)
{
    return m_oDisplay;
}

void DisplayManager::update(void)
{
    m_oDisplay.update();
}

void DisplayManager::updateWindow(void)
{
    m_oDisplay.updateWindow(0, 0, m_uiScreenWidth, m_uiScreenHeight, true);
}
