
#include <iostream>
#include <vector>
#include <cstdint>
#include <windows.h>

// Helper function to convert a pattern string into a byte array
std::vector<int> parsePattern(const std::string& pattern) {
    std::vector<int> bytePattern;
    const char* start = pattern.c_str();
    const char* end = start + pattern.length();

    while (start < end) {
        if (*start == '?') {
            bytePattern.push_back(-1);
            start++;
            if (*start == '?') start++;
        } else {
            bytePattern.push_back(std::strtoul(start, nullptr, 16));
            start += 2;
        }
        while (*start == ' ') start++;
    }

    return bytePattern;
}

// Function to perform the pattern scan in a memory block
uintptr_t patternScan(uintptr_t start, uintptr_t size, const std::string& pattern) {
    std::vector<int> bytePattern = parsePattern(pattern);
    const auto* data = reinterpret_cast<const uint8_t*>(start);
    size_t patternSize = bytePattern.size();

    for (size_t i = 0; i < size - patternSize; i++) {
        bool found = true;
        for (size_t j = 0; j < patternSize; j++) {
            if (bytePattern[j] != -1 && data[i + j] != static_cast<uint8_t>(bytePattern[j])) {
                found = false;
                break;
            }
        }
        if (found) {
            return start + i;
        }
    }
    return 0;
}

// Example usage
int main() {
    // Memory block (test array)
    uint8_t testMemory[] = { 0x90, 0x90, 0xDE, 0xAD, 0xBE, 0xEF, 0x90, 0x90 };
    uintptr_t start = reinterpret_cast<uintptr_t>(testMemory);
    uintptr_t size = sizeof(testMemory);

    // Pattern to search for
    std::string pattern = "DE AD BE EF";

    // Start scanning
    uintptr_t result = patternScan(start, size, pattern);
    if (result != 0) {
        std::cout << "Pattern found at address: 0x" << std::hex << result << std::endl;
    } else {
        std::cout << "Pattern not found!" << std::endl;
    }

    return 0;
}
