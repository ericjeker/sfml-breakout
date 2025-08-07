// Copyright (c) Eric Jeker 2025.

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <nlohmann/json_fwd.hpp>
#include <string>

using json = nlohmann::json;

namespace FileManager
{

json LoadJSON(const std::string& filePath);

}


#endif // FILEMANAGER_H
