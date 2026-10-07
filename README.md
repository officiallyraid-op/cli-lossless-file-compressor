# CLI Lossless File Compressor

---

A standalone command-line file compression and decompression tool built in C++.
The project implements Huffman coding using a min-heap to perform lossless file compression.
---

## Features

- Lossless Huffman compression
- Streaming bit encoding
- Huffman tree serialization
- Bitwise packing
- Binary file I/O
- No external dependencies
---
## How It Works

### Compression

1. Reads the input file and counts the frequency of each byte.
2. Stores the bytes in a min-heap based on frequency.
3. Builds a Huffman tree by repeatedly combining the two least-frequent nodes.
4. Generates a Huffman code for each byte.
5. Serializes the Huffman tree and stores the original file size.
6. Reads the input again and packs the Huffman codes directly into bytes.

### Decompression

1. Reads the serialized Huffman tree from the compressed file.
2. Reconstructs the original Huffman tree.
3. Reads the compressed data bit-by-bit.
4. Traverses the tree using each bit.
5. Writes a byte whenever a leaf node is reached.
6. Stops once the original number of bytes has been restored.
---
## Building

Requires a C++17-compatible compiler or newer.

Using g++:

```bash
g++ -std=c++17 main.cpp -o main.exe