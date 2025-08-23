// This code uses a simple XOR-based byte "encryption" and decryption mechanism.
// The encrypt function takes a vector of bytes and a vector of bytes as keys, XORs each byte with each keys (creates effective keys),
// and returns the resulting vector of encrypted bytes.
// You can generate any amount of random keys using the generateKeys function.
// The decrypt function takes the same parameters as the encrypt function, applies the same XOR-based mechanism, and returns the original bytes in a vector.
// Decrypt function return an error message if decryption fails. (FATAL ERROR)
// This is just a small personal project to demonstrate basic encryption concepts. 
// DO NOT USE FOR ACTUAL ENCRYPTION. AN ATTACKER THAT KNOWS WHAT THEY'RE DOING CAN EASILY CRACK THIS.

// includes
#include <vector>
#include <random>
#include <cstdint>
#include <algorithm>

class BitEncryption {
private:

    static constexpr short signatureSize = 3;
    static constexpr uint8_t signature[3] = { 0x3B, 0x2D, 0x29 };
    const std::vector<uint8_t> errorMessage = { 'F', 'A', 'T', 'A', 'L', ' ', 'E', 'R', 'R', 'O', 'R' };

public:

    // Encrypt using caller-provided keys, without storing any keys in the output.
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& originalBytes, const std::vector<uint8_t>& keys) {

        // Basic checks
        if (originalBytes.empty() || keys.empty() || keys.size() > static_cast<size_t>(0xFFFFFFFFull)) {
            return errorMessage;
        }

        // Define constants
        const size_t dataSize = originalBytes.size();
        const size_t keyQuantity = keys.size();

        // Create effective keys
        std::vector<uint8_t> effectiveKeys(dataSize, 0);
        if (keyQuantity >= dataSize) {
            for (size_t k = 0; k < keyQuantity; ++k) {
                effectiveKeys[k % dataSize] ^= keys[k];
            }
        } else {
            for (size_t i = 0; i < dataSize; ++i) {
                effectiveKeys[i] = keys[i % keyQuantity];
            }
        }

        // Initialize a vector to hold the encrypted bytes (data + signature)
        std::vector<uint8_t> encryptedBytes(dataSize + signatureSize);

        // XOR each byte in the vector with the effective keys
        for (size_t i = 0; i < dataSize; ++i) {
            encryptedBytes[i] = static_cast<uint8_t>(originalBytes[i] ^ effectiveKeys[i]);
        }

        // Reverse the encrypted bytes
        std::reverse(encryptedBytes.begin(), encryptedBytes.begin() + dataSize);

        // Add signature at the end
        std::copy(signature, signature + signatureSize, encryptedBytes.end() - signatureSize);
        
        return encryptedBytes;
    }

    // Decrypt bytes produced by encrypt (caller supplies the same keys).
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& encryptedBytes, const std::vector<uint8_t>& keys) {

        // Basic checks
        if (keys.empty() || encryptedBytes.size() < static_cast<size_t>(signatureSize + 1)) {
            return errorMessage;
        }

        // Validate signature at the end
        const size_t totalSize = encryptedBytes.size();
        const size_t dataSize = totalSize - static_cast<size_t>(signatureSize);

        if (!std::equal(encryptedBytes.begin() + dataSize, encryptedBytes.end(), signature)) {
            return errorMessage;
        }

        // Extract the encrypted bytes (without signature) and reverse it back
        std::vector<uint8_t> bytes(encryptedBytes.begin(), encryptedBytes.begin() + dataSize);
        std::reverse(bytes.begin(), bytes.end());

        // Recreate effective keys exactly as in the encrypt function
        const size_t keyQuantity = keys.size();
        std::vector<uint8_t> effectiveKeys(dataSize, 0);
        if (keyQuantity >= dataSize) {
            for (size_t k = 0; k < keyQuantity; ++k) {
                effectiveKeys[k % dataSize] ^= keys[k];
            }
        } else {
            for (size_t i = 0; i < dataSize; ++i) {
                effectiveKeys[i] = keys[i % keyQuantity];
            }
        }

        // XOR with effective keys to recover the original bytes
        std::vector<uint8_t> originalBytes(dataSize);
        for (size_t i = 0; i < dataSize; ++i) {
            originalBytes[i] = static_cast<uint8_t>(bytes[i] ^ effectiveKeys[i]);
        }

        return originalBytes;
    }

    // Function to generate a set amount of random keys in range [1, 255] using a C++ RNG. (fancy random number generator)
    std::vector<uint8_t> generateKeys(const size_t count) {

        // Set up the random number generator
        static thread_local std::mt19937 rng{std::random_device{}()};
        std::uniform_int_distribution<int> dist(1, 255);

        // Generate the keys
        std::vector<uint8_t> keys;
        keys.reserve(count);
        for (size_t i = 0; i < count; ++i) {
            keys.push_back(static_cast<uint8_t>(dist(rng)));
        }

        // Return the vector of generated keys
        return keys;
    }
};