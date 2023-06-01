#include "common/ConfigurationFiles.h"
#include "RobotData.h"

void ConfigurationFiles::ReadFile(const RobotData &robotData, ConfigData &configData, const std::string &fileName)
{
    frc::SmartDashboard::PutNumber("Failed Config Read Attempts", failedReadAttempts);
    frc::SmartDashboard::PutBoolean("Has config been read?", false);

    // Accessing the deploy dir
    fs::path deployDir = frc::filesystem::GetDeployDirectory();

    // Path to the config file
    fs::path robotConfigFile = deployDir / "ConfigFiles" / fileName;

    // Accessing the config file
    std::ifstream inFile;
    inFile.open(robotConfigFile.string());

    // Goes through each line of the config file
    if(inFile.is_open()) 
    {
        std::string line; 

        while (getline(inFile, line))
        {
            ParseLine(configData, line);
        }

        inFile.close();

        frc::SmartDashboard::PutBoolean("Has config been read?", true);
    }
    else
    {
        frc::SmartDashboard::PutBoolean("Has config been read?", false);
    }
    
}

/*
* @note Parses through each line to get the key and values
*/
void ConfigurationFiles::ParseLine(ConfigData &configData, const std::string &line)
{
    // If we have empty lines, return nothing
    if (line.empty() || line[0] == '#')
    {
        return;
    }

    // Creating the key and value strings
    std::string key;
    std::string value;

    // Splitting up the line into two strings
    std::size_t pos = line.find('=');

    // Grabs the key and value pairs
    if (pos != std::string::npos)
    {
        key = line.substr(0, pos);
        value = line.substr(pos + 1);
    }
    else 
    {
        key = line.substr(1);
        value = "";
    }

    // This trims all of the uneeded space for the keys and values
    key.erase(0, key.find_first_not_of(" \t"));
    key.erase(key.find_last_not_of(" \t") + 1);
    value.erase(0, value.find_first_not_of(" \t"));
    value.erase(value.find_last_not_of(" \t") + 1);


    // This is where we check all of our keys to populate the data struct
    try
    {
        switch (configMap[key])
        {
            case 1:
                break;
        }
    }
    catch (...)
    {
        throw "configs failed to read";
    }
}