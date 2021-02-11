#ifndef __CONFIG_MANAGER__
#define __CONFIG_MANAGER__


class ConfigManager {
public:
    static ConfigManager& getInstance(void);

private:
    ConfigManager(void);
};

#endif
