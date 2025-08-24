# BitEncryption

Lightweight C++ byte-array obfuscation using XOR, byte-reversal, and a trailing 3‑byte signature. This is a simple educational utility; it is not a cryptographically secure algorithm.

## Project structure

[src/](src/)<br>
&emsp;├─ [bit_encryption.h](src/bit_encryption.h)<br>
&emsp;└─ [bit_encryption.cpp](src/bit_encryption.cpp)<br>
[legacy/](legacy/)<br>
&emsp;└─ [bit_encryption_legacy.cpp](legacy/bit_encryption_legacy.cpp)<br>
[example/](example/)<br>
&emsp;└─ [example.cpp](example/example.cpp)<br>
[LICENSE](LICENSE)<br>
[README.md](README.md)<br>

## How it works (at a glance)

Given input bytes and one or more key bytes:

1. Builds an effective key stream of the same length as the input:

- If there are fewer keys than input bytes, keys are repeated (cycled).
- If there are more or equal keys, they are folded by XOR into positions modulo the input length.

2. XORs each input byte with the corresponding effective key byte.
3. Reverses the resulting byte order.
4. Appends a 3‑byte signature `0x3B, 0x2D, 0x29` to mark a valid ciphertext.

Decryption validates the signature, strips it, reverses the bytes, rebuilds the same effective key stream, and XORs to recover the original.

Error handling: on invalid input, methods return an empty vector.

## Public API

Class: `BitEncryption`

- `std::vector<uint8_t> encrypt(const std::vector<uint8_t>& originalBytes, const std::vector<uint8_t>& keys);`
  - Returns ciphertext with signature appended, or an empty vector on error.

- `std::vector<uint8_t> decrypt(const std::vector<uint8_t>& encryptedBytes, const std::vector<uint8_t>& keys);`
  - Returns recovered plaintext, or an empty vector on error (e.g., missing/invalid signature).

- `std::vector<uint8_t> generateKeys(size_t count);`
  - Generates `count` random bytes in `[0, 255]` using `std::random_device` with `std::uniform_int_distribution` (unbiased). On mainstream Windows/Linux, `random_device` is typically backed by the OS CSPRNG, but this isn’t guaranteed by the C++ standard.

Signature bytes and size (for reference): `0x3B, 0x2D, 0x29` (size 3).

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
- Keys may include 0 values. Allowing 0 ensures a full 8 bits of entropy per byte; XOR by 0 occurs with probability 1/256 and is expected.
- The key folding behavior when providing many keys is intentional; read the implementation if you need a different policy (e.g., truncate or hash keys).

## License

This project is released under The Unlicense (public domain). See [LICENSE](LICENSE) or <https://unlicense.org>.
