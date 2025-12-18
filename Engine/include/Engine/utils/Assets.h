#pragma once
#include <string>

class Assets {
public:
    static std::string path(std::string_view rel);
};