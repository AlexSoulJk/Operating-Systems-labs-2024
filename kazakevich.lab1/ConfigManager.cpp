#include "ConfigManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>


ConfigManager* ConfigManager::instance_ptr = nullptr;

ConfigManager* ConfigManager::getInstance() {
	if (!instance_ptr) {
        ConfigManager::instance_ptr = new ConfigManager();
	}
	return ConfigManager::instance_ptr;
}

// ������� ��� �������� �������� � ������ � ����� ������
std::string ConfigManager::trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}

bool ConfigManager::loadConfig()
{
	std::ifstream configReader(configPath);
	if (!configReader.is_open()) {
        std::cerr << "�� ������� ������� ���� ������������: " << configPath << std::endl;
        return false;
	}

    std::unordered_map<std::string, std::string> configMap;
    std::string line;

    while (configReader >> line) {
        // ���������� ������ ������ � �����������
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // ���� ������ "="
        size_t delimiterPos = line.find('=');
        if (delimiterPos == std::string::npos) {
            std::cerr << "������������ ������ ������: " << line << std::endl;
            continue;
        }

        // ��������� ������ �� ��� ��������� � ��������
        std::string paramName = trim(line.substr(0, delimiterPos));
        std::string paramValue = trim(line.substr(delimiterPos + 1));

        // ��������� � �����
        configMap[paramName] = paramValue;
    }

    configReader.close();

    // ����������� �������� �� ����� � ���������
    if (configMap.find("pidfilePath") != configMap.end()) {
        configParams.pidfilePath = configMap["pidfilePath"];
    }
    if (configMap.find("workingdirPath") != configMap.end()) {
        configParams.workingdirPath = configMap["workingdirPath"];
    }
    if (configMap.find("interval") != configMap.end()) {
        try {
            configParams.interval = std::stoi(configMap["interval"]);
        }
        catch (const std::exception& e) {
            std::cerr << "������ �������������� ��������� interval: " << e.what() << std::endl;
        }
    }
    return true;
}

ConfigParams ConfigManager::get() const
{
	return configParams;
}

void ConfigManager::setConfigPath(std::string path)
{
	configPath = path;
}
