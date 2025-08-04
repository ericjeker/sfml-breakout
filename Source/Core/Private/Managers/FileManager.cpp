// Copyright (c) Eric Jeker 2025.

#include "Core/Managers/FileManager.h"

#include "Core/Logger.h"
#include "nlohmann/json.hpp"
#include "yaml-cpp/yaml.h"

#include <format>
#include <fstream>

json FileManager::LoadJSON(const std::string& filePath)
{
    std::ifstream f(filePath);
    return json::parse(f);
}

yaml FileManager::LoadYAML(const std::string& filePath)
{
    YAML::Node node;

    try
    {
        node = YAML::LoadFile(filePath);
    } catch (const YAML::Exception& e)
    {
        LOG_ERROR(std::format("Failed to load scene file {}: {}", filePath, e.what()));
    }

    return node;
}
