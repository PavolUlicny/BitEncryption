//this code uses a simple XOR bit encryption and decryption mechanism.
//the encryption function takes a vector of bytes, generates a random key, and returns a vector of encrypted bytes.
//the decryption function takes a vector of encrypted bytes and returns the original vector of bytes.

//include modules
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <cstdint>

class BitEncryption {
    //function to encrypt text
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& originalBytes) {

        //check if the input text is empty
        if (originalBytes.empty()) {
            return {};
        }

        //generate a random key
        srand(time(NULL));
        uint8_t key = (rand() % 255) + 1;

        //initialize a vector to hold the encrypted bytes
        std::vector<uint8_t> encryptedByteVector;

        //encrypt each byte in the vector by adding the key
        for (uint8_t byte : originalBytes) {
            encryptedByteVector.push_back(byte ^ key);
        }

        //add the key to the vector
        encryptedByteVector.push_back(key);

        //return the encrypted text
        return encryptedByteVector;
    }

    //function to decrypt text
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& encrypted) {

        //check if the encrypted vector is empty or if its only the key
        if (encrypted.size() < 2) {
            return {};
        }

        //get the key from the last byte
        uint8_t key = encrypted.back();

        std::vector<uint8_t> decrypted;

        // XOR each byte (except last) with the key to get original bytes
        for (size_t i = 0; i < encrypted.size() - 1; ++i) {
            decrypted.push_back(static_cast<uint8_t>(encrypted[i] ^ key));
        }

        //return the decrypted vector
        return decrypted;
    }
};

int main() {

	return 0;
}