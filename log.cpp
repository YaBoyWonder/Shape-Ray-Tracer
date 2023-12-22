#include "log.hpp"

int Log::LEVEL = 0;
std::ostream *Log::stream = &std::cout;

void Log::set_output(std::ostream& stream) {
    Log::stream = &stream;
}

std::ostream& Log::os() {
    return *Log::stream;
}


