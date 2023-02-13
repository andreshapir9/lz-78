#ifndef LZ78_H
#define LZ78_H

#include <string>
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <iterator>

using namespace std;


class lz78{
    public:
        void compress(string fileToRead, string fileToOutput);
        void decompress(string fileToRead, string fileToOutput);
        void compress16(string fileToRead, string fileToOutput);
        void decompress16(string fileToRead, string fileToOutput);
        double getsizeOfFile(string filename);
    private:
        string readFile(string fileToRead);
        void WriteToFile(string filename,string input );
        string convertBinary(int input);
        string convertToAscii(char inpput);
        void WriteToFileBinary(string filename,string input );
        int convertToDecimal(char input);
        char convertToChar(string input);
        int binaryToDecimal(long n);

};

#endif