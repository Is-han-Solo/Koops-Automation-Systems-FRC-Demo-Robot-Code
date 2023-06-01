#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
// #include <unor>
#include <unordered_map>
#include <frc/FileSystem.h>

struct RobotData;



struct ConfigData 
{
    
};

class ConfigurationFiles 
{
public:
    void ReadFile(const RobotData &robotData, ConfigData &configData, const std::string &fileName);
private:
    void ParseLine(ConfigData &configData, const std::string &line);

    std::unordered_map<std::string, int> configMap = 
    {
        {"VoltageCompensation", 0}

        
    
    };

    int failedReadAttempts = 0;


};