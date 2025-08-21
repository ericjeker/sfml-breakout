// Copyright (c) Eric Jeker 2025.

#pragma once

#include <nlohmann/json_fwd.hpp>
#include <string>

using json = nlohmann::json;

namespace FileManager
{

json LoadJSON(const std::string& filePath);

}


