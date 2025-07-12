// Copyright (c) Eric Jeker 2025.

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <nlohmann/json_fwd.hpp>
#include <string>
#include <yaml-cpp/node/node.h>

using json = nlohmann::json;
using yaml = YAML::Node;

namespace FileManager
{

json LoadJSON(const std::string& filePath);
yaml LoadYAML(const std::string& filePath);

};


#endif // FILEMANAGER_H
