#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

class BitEncryption {
private:
    static constexpr short signatureSize = 3;
    static constexpr uint8_t signature[signatureSize] = { 0x3B, 0x2D, 0x29 };
    const std::vector<uint8_t> errorMessage = { 'F', 'A', 'T', 'A', 'L', ' ', 'E', 'R', 'R', 'O', 'R' };

public:
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& originalBytes, const std::vector<uint8_t>& keys);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& encryptedBytes, const std::vector<uint8_t>& keys);
    std::vector<uint8_t> generateKeys(const size_t count);
};