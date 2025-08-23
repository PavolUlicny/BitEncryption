#include <cstdint>
#include <vector>
#include <iostream>
#include "../src/bit_encryption.h"

int main()
{
	BitEncryption be;

	std::vector<uint8_t> data{'H', 'e', 'l', 'l', 'o'};
	std::vector<uint8_t> keys = be.generateKeys(3); // or provide your own keys

	auto cipher = be.encrypt(data, keys);
	if (cipher.empty())
	{
		std::cerr << "Encrypt failed" << std::endl;
		return 1;
	}

	auto plain = be.decrypt(cipher, keys);
	if (plain.empty())
	{
		std::cerr << "Decrypt failed" << std::endl;
		return 1;
	}

	std::cout << std::string(plain.begin(), plain.end()) << std::endl; // Hello
	return 0;
}