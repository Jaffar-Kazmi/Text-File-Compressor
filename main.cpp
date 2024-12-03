#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <bitset>
#include <vector>

using namespace std;

void handle_Compression();
void handle_Decompression();
void handle_Search();

class Node
{
public:
    char ch;
    int frequency;
    Node *left;
    Node *right;

    Node(char ch, int frequency)
    {
        this->ch = ch;
        this->frequency = frequency;
        this->left = this->right = nullptr;
    }
};

class Compare
{
public:
    bool operator()(Node *left, Node *right)
    {
        return (left->frequency) > (right->frequency);
    }
};

// QuickSort implementation for sorting Huffman codes
class HuffmanSorter {
private:
    // Partition function for quicksort
    int partition(vector<pair<char, string>>& arr, int low, int high) {
        pair<char, string> pivot = arr[high];
        int i = low - 1;
        
        for (int j = low; j < high; j++) {
            if (arr[j].first <= pivot.first) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        return i + 1;
    }
    
    // Recursive quicksort function
    void quickSort(vector<pair<char, string>>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }
    
public:
    void sortHuffmanCodes(vector<pair<char, string>>& codes) {
        if (codes.size() <= 1) return;
        quickSort(codes, 0, codes.size() - 1);
    }
};

// function to count frequncy of each character and return a map
map<char, int> countFrequency(string fileName)
{
    ifstream file(fileName);  // open file
    map<char, int> frequency; // map to store frequency
    char ch;
    
    if (!file.is_open())
    {
        cerr << "Error opening file" << endl;
        return frequency; // return frequency if the file could not be opened
    }
    
    while (file.get(ch))
    { // loop continues as long as it returns a character, terminates at eof
        frequency[ch]++;
    }
    file.close();
    return frequency;
}

// function to build priority queue and return that queue
priority_queue<Node *, vector<Node *>, Compare> buildPriorityQueue(map<char, int> frequency)
{
    priority_queue<Node *, vector<Node *>, Compare> pq;
    for (auto pair : frequency)
    {
        pq.push(new Node(pair.first, pair.second));
    }
    return pq;
}

// function to build huffman tree
Node *buildHuffmanTree(priority_queue<Node *, vector<Node *>, Compare> pq)
{
    while (pq.size() > 1)
    {
        Node *left = pq.top();
        pq.pop();
        Node *right = pq.top();
        pq.pop();
        Node *newNode = new Node('\0', left->frequency + right->frequency);
        newNode->left = left;
        newNode->right = right;
        pq.push(newNode);
    }
    return pq.top();
}

// recursive function to generate huffman codes
void generateHuffmanCodes(Node *node, string code, map<char, string> &huffmanCodes)
{
    if (!node)
    {
        return;
    }
    if (!node->left && !node->right)
    {
        huffmanCodes[node->ch] = code;
    }
    generateHuffmanCodes(node->left, code + "0", huffmanCodes);
    generateHuffmanCodes(node->right, code + "1", huffmanCodes);
}

// Compress the file using Huffman codes
string compressFile(string inputFile, map<char, string> huffmanCodes)
{
    ifstream file(inputFile);
    string compressedData = "";
    char ch;

    if (!file.is_open())
    {
        cerr << "Error opening file" << endl;
        return "";
    }

    while (file.get(ch))
    {
        compressedData += huffmanCodes[ch];
    }

    file.close();
    return compressedData;
}

// Write the compressed data and Huffman codes to a binary file
void writeCompressedFile(string outputFile, string compressedData, map<char, string> huffmanCodes)
{
    ofstream write(outputFile, ios::binary);

    // Write the Huffman codes (table) to the file
    write << huffmanCodes.size() << endl; // Write the number of codes
    for (pair<char, string> pair : huffmanCodes)
    {
        write << pair.first << ":" << pair.second << "\n";
    }

    // Write the compressed binary data
    for (size_t i = 0; i < compressedData.size(); i += 8)
    {
        // takes a chunk of 8 bits from the compressed data
        string byte = compressedData.substr(i, 8);
        // convert the string to bitset
        bitset<8> bits(byte);
        // converts the bitset to unsigned long integer
        unsigned long toUnsignedLong = bits.to_ulong();
        // writes each character one by one to the file
        write.put(toUnsignedLong);
    }
    write.close();
}

// function to read compressed data from the file
void readCompressedFile(string compressedFile, map<string, char> &huffmanCodes, string &binaryData)
{
    ifstream file(compressedFile);
    if (!file.is_open())
    {
        cerr << "Error in opening " << compressedFile << " file." << endl;
        return;
    }

    // Read the total number of codes
    int numofcodes;
    file >> numofcodes;
    file.ignore();

    // Read the characters and corresponding codes from the file and store them in a file
    for (int i = 0; i < numofcodes; i++)
    {
        char ch;
        string code;
        file.get(ch);
        file.ignore();
        getline(file, code);
        huffmanCodes[code] = ch;
    }
    
    // Read binary data from file and convert it into string
    char byte;
    while (file.get(byte))
    {
        bitset<8> bits(byte);
        string toString = bits.to_string();
        binaryData = binaryData + toString;
    }

    file.close();
}

string decompressData(map<string, char> huffmanCodes, string binaryData)
{
    string decompressedData = "";
    string currentCode = "";
    // Traverse the binary data
    for (char bit : binaryData)
    {
        // Append each bit to the current code
        currentCode += bit;
        // Check if the current code is a valid Huffman code
        if (huffmanCodes.find(currentCode) != huffmanCodes.end())
        {
            decompressedData += huffmanCodes[currentCode]; // Append the original character
            currentCode = "";                              // Reset the current code
        }
    }

    return decompressedData;
}

void writeDecompressedFile(string outputFile, string decompressedData)
{
    ofstream file(outputFile);
    if (!file.is_open())
    {
        cerr << "Error opening output file" << endl;
        return;
    }

    file << decompressedData;
    file.close();
}

//=================================Searching========================================//

// Finds the frequency of a character from file linearly
bool findFrequency(string fileName, char ch)
{
    map<char, int> frequency = countFrequency(fileName);

    for (auto i = frequency.begin(); i != frequency.end(); i++)
    {
        if (i->first == ch)
        {
            cout << "\nFrequency of character '" << ch << "' is: " << i->second << endl
                 << endl;
            return true;
        }
    }
    return false;
}

// Function to find Huffman code of a character using binary search
string findHuffmanCode(map<char, string> &huffmanCodes, char ch)
{
    // Convert map to vector of pairs
    vector<pair<char, string>> sortedHuffmanCodes(huffmanCodes.begin(), huffmanCodes.end());

    // Sort the vector using quick sort
    HuffmanSorter sorter;
    sorter.sortHuffmanCodes(sortedHuffmanCodes);

    // Perform binary search
    int left = 0;
    int right = sortedHuffmanCodes.size() - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (sortedHuffmanCodes[mid].first == ch)
        {
            return sortedHuffmanCodes[mid].second; // Return the Huffman code
        }
        else if (sortedHuffmanCodes[mid].first < ch)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    return ""; // Return an empty string if character not found
}

int main()
{
    cout << "\n*******************Text File Compressor*****************\n"
         << endl;
    int choice;
    do
    {
        cout << "1. Compress File\n";
        cout << "2. Decompress File\n";
        cout << "3. Search Character\n";
        cout << "4. Exit\n" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            handle_Compression();
            break;
        case 2:
            handle_Decompression();
            break;
        case 3:
            handle_Search();
            break;
        }
    } while (choice != 4);
}

void handle_Compression()
{
    string inputFile, outputFile;
    cout << "Enter the file name to be compressed: ";
    cin >> inputFile;
    cout << "Enter the file name to store the compressed data: ";
    cin >> outputFile;

    // Count the frequency of each character in the file
    map<char, int> frequency = countFrequency(inputFile);

    // // To display frequencies on console
    // cout << "Frequencies are: ";
    // for (const auto &pair : frequency)
    // {
    //     cout << "'" << pair.first << "' : " << pair.second << "\n";
    // }

    // Build a priority queue to store the nodes
    priority_queue<Node *, vector<Node *>, Compare> pq = buildPriorityQueue(frequency);

    // Build the Huffman tree
    Node *root = buildHuffmanTree(pq);

    // Generate Huffman codes
    map<char, string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);

    // cout << "Huffman Codes are :";
    // for (auto pair : huffmanCodes)
    // {
    //     cout << pair.first << " : " << pair.second << "\n";
    // }

    string compressedData = compressFile(inputFile, huffmanCodes);
    // cout << compressedData;

    // Write the compressed data to output file
    writeCompressedFile(outputFile, compressedData, huffmanCodes);

    cout << "\nFile compressed successfully!\n"
         << endl;
}

void handle_Decompression()
{
    string inputFile, outputFile;
    cout << "Enter the file name to to be Decompressed: ";
    cin >> inputFile;
    cout << "Enter the file name to store the Decompressed data: ";
    cin >> outputFile;

    map<string, char> reverseHuffmanCodes;
    string binaryData;

    // Read the compressed file
    readCompressedFile(inputFile, reverseHuffmanCodes, binaryData);

    // Decompress the binary data
    string decompressedData = decompressData(reverseHuffmanCodes, binaryData);

    // Write the decompressed data to a file
    writeDecompressedFile(outputFile, decompressedData);

    cout << "\nDecompression complete. Data written to " << outputFile << endl
         << endl;
}

void handle_Search()
{
    int choice;
    do
    {
        cout << "\nSearch Options:\n";
        cout << "1. Frequency of a character\n";
        cout << "2. Huffman code of a character\n";
        cout << "3. Exit\n";

        cout << "\nEnter your choice : ";
        cin >> choice;
        cin.ignore();

        if (choice == 3) {
            cout << "\nExiting...\n";
            break;  
        }

        string file;
        cout << "\nEnter file name: ";
        getline(cin, file);
        char ch;
        cout << "Enter character to search: ";
        cin >> ch;

        switch (choice)
        {
        case 1:
        {
            if (!findFrequency(file, ch))
            {
                cout << endl
                     << ch << " is not found in " << file << endl;
            }
        }
        break;
        case 2:
        {            // First generate the frequency map
            map<char, int> frequency = countFrequency(file);
            if (frequency.empty())
            {
                cout << "\nError reading file or file is empty." << endl;
                break;
            }
            // Build the priority queue
            priority_queue<Node *, vector<Node *>, Compare> pq = buildPriorityQueue(frequency);
            // Build the Huffman tree
            Node *root = buildHuffmanTree(pq);
            // Generate Huffman codes
            map<char, string> huffmanCodes;
            generateHuffmanCodes(root, "", huffmanCodes);
            string code = findHuffmanCode(huffmanCodes, ch);     // Now search for the character's code
            if (!code.empty())
            {
                cout << "\nHuffman code for character '" << ch << "' is: " << code << endl;
            }
            else
            {
                cout << "\nCharacter '" << ch << "' not found in Huffman codes." << endl;
            }
        }
        break;
        default:
            cout << "\nInvalid choice. Please try again." << endl;
        }
    } while (choice != 3);
}