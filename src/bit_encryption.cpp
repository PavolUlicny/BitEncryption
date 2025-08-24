#include "bit_encryption.h"

#include <algorithm>
#include <random>

constexpr uint8_t BitEncryption::signature[BitEncryption::signatureSize];

std::vector<uint8_t> BitEncryption::encrypt(const std::vector<uint8_t> &originalBytes, const std::vector<uint8_t> &keys)
{
	if (originalBytes.empty() || keys.empty())
	{
		return {};
	}

	const size_t dataSize = originalBytes.size();
	const size_t keyQuantity = keys.size();

	std::vector<uint8_t> effectiveKeys(dataSize, 0);
	if (keyQuantity >= dataSize)
	{
		for (size_t k = 0; k < keyQuantity; ++k)
		{
			effectiveKeys[k % dataSize] ^= keys[k];
		}
	}
	else
	{
		for (size_t i = 0; i < dataSize; ++i)
		{
			effectiveKeys[i] = keys[i % keyQuantity];
		}
	}

	std::vector<uint8_t> encryptedBytes(dataSize + signatureSize);

	for (size_t i = 0; i < dataSize; ++i)
	{
		encryptedBytes[i] = static_cast<uint8_t>(originalBytes[i] ^ effectiveKeys[i]);
	}

	std::reverse(encryptedBytes.begin(), encryptedBytes.begin() + dataSize);

	std::copy(signature, signature + signatureSize, encryptedBytes.end() - signatureSize);

	return encryptedBytes;
}

std::vector<uint8_t> BitEncryption::decrypt(const std::vector<uint8_t> &encryptedBytes, const std::vector<uint8_t> &keys)
{
	if (keys.empty() || encryptedBytes.size() < static_cast<size_t>(signatureSize + 1))
	{
		return {};
	}

	const size_t totalSize = encryptedBytes.size();
	const size_t dataSize = totalSize - static_cast<size_t>(signatureSize);

	if (!std::equal(encryptedBytes.begin() + dataSize, encryptedBytes.end(), signature))
	{
		return {};
	}

	std::vector<uint8_t> bytes(encryptedBytes.begin(), encryptedBytes.begin() + dataSize);
	std::reverse(bytes.begin(), bytes.end());

	const size_t keyQuantity = keys.size();
	std::vector<uint8_t> effectiveKeys(dataSize, 0);
	if (keyQuantity >= dataSize)
	{
		for (size_t k = 0; k < keyQuantity; ++k)
		{
			effectiveKeys[k % dataSize] ^= keys[k];
		}
	}
	else
	{
		for (size_t i = 0; i < dataSize; ++i)
		{
			effectiveKeys[i] = keys[i % keyQuantity];
		}
	}

	std::vector<uint8_t> originalBytes(dataSize);
	for (size_t i = 0; i < dataSize; ++i)
	{
		originalBytes[i] = static_cast<uint8_t>(bytes[i] ^ effectiveKeys[i]);
	}

	return originalBytes;
}

std::vector<uint8_t> BitEncryption::generateKeys(const size_t count)
{
	std::random_device rd;
	std::uniform_int_distribution<int> dist(0, 255);

	std::vector<uint8_t> keys;
	keys.reserve(count);
	for (size_t i = 0; i < count; ++i)
	{
		keys.push_back(static_cast<uint8_t>(dist(rd)));
	}
	return keys;
}