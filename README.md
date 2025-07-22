# 🗜️ Huffman File Compressor in C++

A simple and efficient **file compression and decompression tool** built from scratch in C++ using **Huffman Coding**.

This project compresses a text file by replacing characters with binary Huffman codes based on their frequency and decompresses it back to the original file.

---

##  Features

-  Compress any plain text file
-  Decompress it back to original
-  Uses Huffman coding (lossless compression)
-  Saves and reads Huffman code maps
-  Easy to understand, modular C++ code

---

## 🧠 Algorithms & Data Structures Used

| Concept         | Usage                                      |
|-----------------|---------------------------------------------|
| **Huffman Tree**| Binary Tree for assigning binary codes      |
| **Min Heap**    | Efficiently fetch two least frequent nodes |
| **Recursion**   | Tree traversal for code assignment          |
| **Bit Manipulation** | Efficient binary file encoding         |
| **Hash Maps**   | Character → Code and Code → Character maps  |

---



## ⚙️ How It Works

### 1. **Compression**
- Read `input.txt` and count character frequencies
- Build a Huffman Tree using a min heap
- Assign binary codes to characters
- Replace characters in file with binary codes
- Write result to `compressed.bin` and save codes to `codes.txt`

### 2. **Decompression**
- Read `compressed.bin` bit-by-bit
- Use `codes.txt` to reverse map binary codes to characters
- Write the original content to `decompressed.txt`

---

## 🛠️ How to Compile and Run

Make sure you have a C++17+ compiler installed.

### 🔧 Compile
```bash
g++ main.cpp -o compressor
