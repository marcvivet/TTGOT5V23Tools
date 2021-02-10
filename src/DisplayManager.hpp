#ifndef __DISPLAY_MANAGER__
#define __DISPLAY_MANAGER__

#define LILYGO_T5_V213

// https://github.com/lewisxhe/GxEPD.git

#include <boards.h>
#include <GxEPD.h>
#include <GxGDEH0213B73/GxGDEH0213B73.h>  // 2.13" b/w newer panel
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>


class DisplayManager {
public:
    static DisplayManager& getInstance(void);

    GxEPD_Class& getDisplay(void);
    void clear(void);
    void update(void);
    void updateWindow(void);

    uint16_t getWidth(void) const;
    uint16_t getHeight(void) const;

private:
    uint16_t m_uiScreenWidth;
    uint16_t m_uiScreenHeight;

    GxIO_Class m_oIo;
    GxEPD_Class m_oDisplay;

    DisplayManager(void);
};

#endif
