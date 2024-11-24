# Text-File-Compressor

A C++ implementation of text file compression using Huffman coding algorithm. This project provides functionality for compressing text files, decompressing them back to their original form, and searching for specific character information within files.

## Author
- Syed Jaffar Raza Kazmi
  
## Features

- **File Compression**: Compress text files using Huffman coding
- **File Decompression**: Restore compressed files to their original form
- **Character Search**:
  - Find frequency of a specific character in a file
  - Look up Huffman codes for individual characters
- **Optimized Implementation**:
  - Priority Queue-based Huffman tree construction
  - Uses QuickSort for efficient code sorting
  - Implements Binary Search for character lookup
  

## Prerequisites

- C++ compiler with C++11 support or higher
- Standard Template Library (STL)

## Installation

1. Clone the repository:
```bash
git clone [repository-url]
```

2. Navigate to the project directory:
```bash
cd text-file-compressor
```

3. Compile the program:
```bash
g++ main.cpp -o compressor
```

### Menu Options

1. **Compress File**
   - Enter source file name
   - Enter destination file name for compressed data
   
2. **Decompress File**
   - Enter compressed file name
   - Enter destination file name for decompressed data
   
3. **Search Character**
   - Choose between:
     - Finding character's frequency
     - Looking up Huffman code
   - Enter file name and character to search

4. **Exit Program**

### Example Usage

```bash
# To compress a file
1
Enter the file name to be compressed: input.txt
Enter the file name to store the compressed data: compressed.bin

# To decompress a file
2
Enter the file name to be Decompressed: compressed.bin
Enter the file name to store the Decompressed data: output.txt

# To search for character information
3
Search Options:
1. Frequency of a character
2. Huffman code of a character
3. Exit
```

## Code Structure

### Key Classes

- `Node`: Represents a node in the Huffman tree
  ```cpp
  class Node {
      char ch;
      int frequency;
      Node *left, *right;
  };
  ```

- `Compare`: Custom comparator for priority queue
- `HuffmanSorter`: Implements QuickSort for Huffman codes

### Key Functions

- `countFrequency()`: Counts character frequencies in input file
- `buildPriorityQueue()`: Creates priority queue for Huffman tree construction
- `buildHuffmanTree()`: Constructs the Huffman tree
- `generateHuffmanCodes()`: Creates Huffman codes for each character
- `compressFile()`: Performs file compression
- `writeCompressedFile()`: Stores Compressed data in a file
- `readCompressedFile()`: Reads Compressed data from a file
- `decompressData()`: Reconstructs original data from compressed file
- `writeDecompressedFile()`: Stores Decompressed data in a file
- `findFrequency()`: Searches for Frequency of a specific character in a file using Linear Search
- `findHuffmanCode()`: Searches for Huffman Code of a specific character using Binary Search
- `sortHuffmanCodes()`: Sorts vector containing Huffan Codes using Quick Sort

## File Format

### Compressed File Structure
1. Number of Huffman codes
2. Huffman coding table (character:code pairs)
3. Compressed binary data

## Performance

- Achieves almost 50% compression ratios 
- Efficient search operations using binary search
- Lossless compression guaranteeing exact data recovery

## Acknowledgments

- Based on David Huffman's data compression algorithm
- Inspired by the need for efficient text file compression
- Developed as part of 3rd Semester's DSA project

## Links
- <a href = "https://docs.google.com/presentation/d/1Vd9tOkWKgV6LlxFzwZKk70X3TCPGADSu9Zwc-rSKlBU/edit?usp=sharing">Huffman Coding ppt</a>

## Future Improvements

- GUI implementation
- Support for directory compression
- Progress bar for compression/decompression
- Additional file format support
- Compression ratio statistics
- Parallel processing for large files



