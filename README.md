# CLI Lossless File Compressor

---

A standalone command-line file compression and decompression tool built in C++.
The project implements Huffman coding using a min-heap to perform lossless file compression.

---

## Features

* **Lossless Compression:** Employs variable-length, prefix-free coding based on byte frequencies to compress arbitrary binary files without losing data.
* **Streaming Memory Efficiency:** Processes incoming files byte-by-byte via standard binary I/O streams (`std::ifstream`/`std::ofstream`), avoiding in-memory string concatenations and allowing large files to be processed efficiently.
* **Pre-Order Tree Serialization Header:** Custom binary serialization encodes the Huffman Tree structure directly into the file header (`1` byte marker for leaves, `0` byte marker for internal nodes), ensuring self-contained decompression without full frequency map overhead.
* **Low-Level Bitwise Packing:** Efficiently packs variable-length bit strings into 8-bit `unsigned char` buffers using bitwise shifting (`<<`) and masking (`|`, `&`).
* **Zero External Dependencies:** Built entirely using modern C++ standard library features (`std::priority_queue`, `std::unordered_map`, structured bindings).

---

## How It Works

### Compression

1. The input file is read in binary mode and the frequency of each byte is recorded.
2. Each unique byte is inserted into a min-heap based on its frequency.
3. The two nodes with the lowest frequencies are repeatedly combined to construct the Huffman tree.
4. The tree is traversed recursively to generate a unique Huffman code for each byte.
5. The Huffman tree and original file size are written to the compressed file as header information.
6. The input file is read again, and each Huffman code is packed directly into bytes and written to the output file.

### Decompression

1. The serialized Huffman tree is read from the compressed file and reconstructed.
2. The original file size is read from the header.
3. The compressed data is read byte-by-byte.
4. Each bit is extracted and used to traverse the Huffman tree (`0` = left, `1` = right).
5. When a leaf node is reached, the corresponding byte is written to the output file.
6. Decompression stops once the original number of bytes has been restored.

---

## Usage

### Compression

```bash
./main.exe compress <input-file> <output-file>
```

### Decompression

```bash
./main.exe decompress <compressed-file> <restored-file>
```

---
