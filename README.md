# BitEncryption

Lightweight C++ byte-array obfuscation using XOR, byte-reversal, and a trailing 3‑byte signature. This is a simple educational utility; it is not a cryptographically secure algorithm.

## Project structure

BitEncryption/
&emsp;├─ [LICENSE](LICENSE)
&emsp;├─ [README.md](README.md)
&emsp;├─ [src/](src/)<br>
&emsp;│&emsp;├─ [bit_encryption.h](src/bit_encryption.h)
&emsp;│&emsp;└─ [bit_encryption.cpp](src/bit_encryption.cpp)
&emsp;└─ [legacy/](legacy/)<br>
&emsp;&emsp; └─ [bit_encryption_legacy.cpp](legacy/bit_encryption_legacy.cpp)

## How it works (at a glance)

Given input bytes and one or more key bytes:

1. Builds an effective key stream of the same length as the input:
	 - If there are fewer keys than input bytes, keys are repeated (cycled).
	 - If there are more or equal keys, they are folded by XOR into positions modulo the input length.
2. XORs each input byte with the corresponding effective key byte.
3. Reverses the resulting byte order.
4. Appends a 3‑byte signature `0x3B, 0x2D, 0x29` to mark a valid ciphertext.

Decryption validates the signature, strips it, reverses the bytes, rebuilds the same effective key stream, and XORs to recover the original.

Error handling: on invalid input, methods return the literal byte vector for the ASCII string `"FATAL ERROR"`.

## Public API

Class: `BitEncryption`

- `std::vector<uint8_t> encrypt(const std::vector<uint8_t>& originalBytes, const std::vector<uint8_t>& keys);`
	- Returns ciphertext with signature appended, or `{"F","A","T","A","L"," ","E","R","R","O","R"}` on error.

- `std::vector<uint8_t> decrypt(const std::vector<uint8_t>& encryptedBytes, const std::vector<uint8_t>& keys);`
	- Returns recovered plaintext, or `{"F","A","T","A","L"," ","E","R","R","O","R"}` on error (e.g., missing/invalid signature).

- `std::vector<uint8_t> generateKeys(size_t count);`
	- Generates `count` random bytes in `[1, 255]` using a thread‑local Mersenne Twister.

Signature bytes and size (for reference): `0x3B, 0x2D, 0x29` (size 3).

## Quick example

```cpp
#include <cstdint>
#include <vector>
#include <iostream>
#include "src/bit_encryption.h"

int main() {
		BitEncryption be;

		std::vector<uint8_t> data{ 'H','e','l','l','o' };
		std::vector<uint8_t> keys = be.generateKeys(3); // or provide your own keys

		auto cipher = be.encrypt(data, keys);
		if (cipher == std::vector<uint8_t>{'F','A','T','A','L',' ','E','R','R','O','R'}) {
				std::cerr << "Encrypt failed" << std::endl;
				return 1;
		}

		auto plain = be.decrypt(cipher, keys);
		if (plain == std::vector<uint8_t>{'F','A','T','A','L',' ','E','R','R','O','R'}) {
				std::cerr << "Decrypt failed" << std::endl;
				return 1;
		}

		std::cout << std::string(plain.begin(), plain.end()) << std::endl; // Hello
		return 0;
}
```

## Build and run

Compile and link `src/bit_encryption.cpp` with your application and include `src/bit_encryption.h`.

- MSVC (Developer Command Prompt / PowerShell):

```
cl /std:c++17 /EHsc your_main.cpp src\bit_encryption.cpp /Fe:app.exe
```

- g++/clang++ (MinGW or Unix-like environments):

```
g++ -std=c++17 your_main.cpp src/bit_encryption.cpp -o app
```

## Notes and limitations

- This is not secure encryption; it’s a simple XOR-based obfuscation with a fixed signature. Do not use for sensitive data.
- Returning the literal `FATAL ERROR` vector is a simplistic error signal; consider wrapping this class and replacing it with exceptions or an error code in production code.
- The key folding behavior when providing many keys is intentional; read the implementation if you need a different policy (e.g., truncate or hash keys).

## License

This project is released under The Unlicense (public domain). See [LICENSE](LICENSE) or https://unlicense.org.