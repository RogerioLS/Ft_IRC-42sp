#include "../../includes/utils/Utils.hpp"

namespace utils {
    std::string intToString(int value) {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }
}