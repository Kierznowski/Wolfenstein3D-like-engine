#include "../../include/Engine/utils/Assets.h"

std::string Assets::path(std::string_view rel) {
    return std::string(RESOURCE_DIR) + std::string(rel);
}