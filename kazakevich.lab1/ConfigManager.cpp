#include "ConfigManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <syslog.h>
#include <linux/limits.h>
#include <unistd.h>
#include <filesystem>

namespace fs = std::filesystem;

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

std::string ConfigManager::resolve_path(const std::string& path) {
    // std::cerr << "current dir " << current_dir << std::endl;
    if (path.front() != '/') {
        return current_dir + "/" + path;
    }
    return path;
};

bool ConfigManager::loadConfig()
{
	std::ifstream configReader(resolve_path(configPath));
	if (!configReader.is_open()) {
        //std::cerr << "Can not open conf file " << resolve_path(configPath).c_str() << std::endl;
        syslog(LOG_WARNING, "�� ������� ������� ���� ������������: %s", configPath.c_str());
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
            syslog(LOG_WARNING, "������������ ������ ������: %s", line.c_str());
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
        configParams.pidfilePath = resolve_path(configMap["pidfilePath"]);
    }
    if (configMap.find("workingdirPath") != configMap.end()) {
        configParams.workingdirPath = resolve_path(configMap["workingdirPath"]);
    }
    if (configMap.find("interval") != configMap.end()) {
        try {
            configParams.interval = std::stoi(configMap["interval"]);
        }
        catch (const std::exception& e) {
            syslog(LOG_WARNING, "������ �������������� ��������� interval: %s", e.what());
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
    char buffer[PATH_MAX];
    if (getcwd(buffer, PATH_MAX) == nullptr) {
        syslog(LOG_ERR, "������ ��������� ������� �����������");
        return exit(EXIT_FAILURE);
    }
    current_dir = std::string(buffer);
    std::cerr << std::endl << current_dir << std::endl;

    configPath = resolve_path(path);
}
