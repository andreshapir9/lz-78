#include "lz78.h"

//compress with blocks
void lz78:: compress(string fileToRead, string fileToOutput){
    //conent of file that will be compressed
    string conentOfFile = readFile(fileToRead);

    //we open the file that we will be writing the compressed data to
    ofstream writingFile(fileToOutput, ofstream::out | ofstream::trunc);

    //current character we are checking if we can compress
    char currentCharacter;

    //character set that we have found a match in the dictionary for
    string PreviousCharacters;
    
    //dictionary of previously created entries
    unordered_map<string, int> dictionary;

    //codeword that will be passed in to compression
    int codeWord =1;
    
    //max codeword before we reset the dictionary
    int MaxCode = 255, startCode =1;

    //traverse every character in the file we are compressing
     for (int i = 0; i < conentOfFile.size(); i++){

        //set current character to the current index of the data we are compressing
        currentCharacter = conentOfFile[i];

        //if the combination of characters already found in the dictionary plus our new character 
        //already exists we will add our character to the string of characters and go to the next index
         if(dictionary.count(PreviousCharacters + currentCharacter) && i != conentOfFile.size()-1){
             PreviousCharacters+= currentCharacter;

        //if that string buildup does not exist in the dictionary
         }else{
            //we create a new pair to compress
            pair<int, char> curPair;

            //if we have no previous reference to the dictionary
            //we will set the reference number to 0
            if(PreviousCharacters.empty())
                curPair.first = 0;

            //if we do have a reference we will set the integer to the codeword corresponding
            //to the previous characters found in the dictionary
            else
                curPair.first = dictionary.find(PreviousCharacters)->second;
            
            //we will set the character of pair to our current character
            curPair.second = currentCharacter;
            
            //we will then write the pair to the compressed file
            writingFile << char( bitset<8>(curPair.first).to_ulong()) << curPair.second;
            writingFile.flush();

            //then we will insert the previously found characters and the new characters referenced to 
            //our new codeword 
            dictionary.insert(pair<string, int>((PreviousCharacters+currentCharacter), codeWord));
            
            //cout<<"( "<< codeWord<< " , "<< PreviousCharacters + currentCharacter << " )"<<"    We encode as :";
            //cout<<"( "<< curPair.first  << " , "<< curPair.second << " ) "<<endl;
            //cout<< char( bitset<8>(curPair.first).to_ulong()) << curPair.second;
            PreviousCharacters.clear();
            //we will increase the codeword and if we have reached our maxcode we will reset our dictionary
            codeWord++;
            if(codeWord == MaxCode){
                codeWord = startCode;
                dictionary.clear();
                //cout<<"we reset the block"<<endl;
            }
        }

    }

    //we then close the file we wrote the compressed conetnt to
    //and print the difference in size to calculate the compression ratio
    writingFile.close();
    cout<<"the compression ratio is : "<< 100 - ((getsizeOfFile(fileToOutput)/ getsizeOfFile(fileToRead)) * 100)<< "%"<<endl;
}

//decompression with blocks 
void lz78::decompress(const string fileToDecompress, const string outputFileName)
{
    //we open the file that we will be writing the decompressed data to
    ofstream writingFile(outputFileName, ofstream::out | ofstream::trunc);

    //dictionary of previously created entries
	unordered_map<int, string> dictionary;

    //data we will read and decompress
	string dataToDecompress = readFile(fileToDecompress);
    //current character we are reading
	string character;
    
    //current codeword we are reading
	int codeword =0;

    //counter To check we are still in the same block
    int counter =1;

    //max block number and starting block number
    int MaxCode = 255, startCode =1;

    //create an empty pair to reference no previous dictionary entry
    dictionary.insert(pair<int, string>(0,""));

    //traverse every character in the file we are decompressing analyze two characters at once
    for (int i = 0; i < dataToDecompress.size(); i+=2){

        //set current codeword to the current index of the data we are decompressing
        //and then convert the character to a decimal
        codeword = convertToDecimal(dataToDecompress[i]);

        //set current character to the next index of the data we are decompressing
		character = dataToDecompress[i+1];

        //create a dictionary entry of the size of the counter and the dictionary string 
        //referenced by the given codeword plus the current character
        dictionary.insert(pair<int, string>(counter, dictionary[codeword] + character));

        //we will write the dictionary entry at the given codeword plus the current character
        writingFile<< dictionary[codeword]<<character;
        writingFile.flush();
        
        //cout<<" We read as :    ( "<<codeword << " , "<< character<< " )    we write it as :    ";
        //cout<<"( "<< counter<< " , "<< dictionary[codeword] + character << " )"<<"  We decode it as :   ";
        //cout<<"( "<< dictionary[codeword]  << " +  "<< character << " ) "<<endl;

        //we will increase the codeword and if we have reached our maxcode we will reset our dictionary
        counter++;
        if(counter == MaxCode){
            counter = startCode;
            dictionary.clear();
        }     

    }
    //close the file we wrote into 
    writingFile.close();
}

//reads a file and returns it as a string
string lz78::readFile(string fileToRead){
    ifstream readFile(fileToRead);
    stringstream buffer;
    buffer << readFile.rdbuf();
    readFile.close();
    return buffer.str();
}
//takes in a string and writes it into a file
void lz78::WriteToFile(string filename,string input ){
    ofstream file(filename, ofstream::out | ofstream::trunc);
    file.flush();
    file<< input;
    file.close();
}

//converts integers to binary
string lz78::convertBinary(int input){
   return bitset<8>(input).to_string();
}

//coonverst an 8 bit char to decimal
int lz78::convertToDecimal(char input){
     return bitset<8>(input).to_ulong();
}

//returns the size of the given file
double lz78::getsizeOfFile(string filename){
    ifstream file(filename, ios::binary);
    file.seekg(0, ios::end);
    return file.tellg();
    file.close();
    
}

//converats an integer into a binary as an integer
int lz78::binaryToDecimal(long n)
{
    int decimalNumber = 0, i = 0, remainder;
    while (n!=0)
    {
        remainder = n%10;
        n /= 10;
        decimalNumber += remainder*pow(2,i);
        ++i;
    }
    return decimalNumber;
}


//compression with 16 bit blocks
void lz78:: compress16(string fileToRead, string fileToOutput){
    string conentOfFile = readFile(fileToRead);
    ofstream writingFile(fileToOutput, ofstream::out | ofstream::trunc);
    char currentCharacter;
    string PreviousCharacters;
    unordered_map<string, int> dictionary;
    u_int16_t codeWord =1;
    u_int16_t MaxCode = UINT16_MAX, startCode =1;
     for (int i = 0; i < conentOfFile.size(); i++){
        currentCharacter = conentOfFile.substr(i,1)[0];
         if(dictionary.count(PreviousCharacters + currentCharacter) && i != conentOfFile.size()-1){
             PreviousCharacters+= currentCharacter;
         }else{
            pair<int, char> curPair;
            if(PreviousCharacters.empty())
                curPair.first = 0;
            else
                curPair.first = dictionary.find(PreviousCharacters)->second;
            curPair.second = currentCharacter;

            //separate a 16 bit number into two 8 bit chars
            string bit16 = bitset<16>(curPair.first).to_string();
            int mid = bit16.size()/2;
            
            u_char first8bit = u_char(binaryToDecimal(stoul(bit16.substr(0,mid))));
            u_char second8bit =u_char(binaryToDecimal(stoul( bit16.substr(mid))));
            
            writingFile << first8bit<< second8bit << curPair.second;
            writingFile.flush();

            dictionary.insert(pair<string, int>((PreviousCharacters+currentCharacter), codeWord));
            PreviousCharacters.clear();
            

            
           string backTobit16 = bitset<8>(first8bit).to_string() + bitset<8>(second8bit).to_string();
            
           
            codeWord++;
            if(codeWord == MaxCode){
                codeWord = startCode;
                dictionary.clear();
                //cout<<"we reset the block"<<endl;
            }
        }

    }
    writingFile.close();
    cout<<"the compression ratio is : "<< 100 - ((getsizeOfFile(fileToOutput)/ getsizeOfFile(fileToRead)) * 100)<< "%"<<endl;
}

//decompression with blocks 
void lz78::decompress16(const string fileToDecompress, const string outputFileName)
{
    ofstream writingFile(outputFileName, ofstream::out | ofstream::trunc);
	unordered_map<int, string> dictionary;
	string DecompressedData;			
	string dataToDecompress = readFile(fileToDecompress);
	string character;
	u_int16_t codeword =0;
    u_int16_t counter =1;
    u_int16_t MaxCode = UINT16_MAX, startCode =1;
    dictionary.insert(pair<int, string>(0,""));

    for (int i = 0; i < dataToDecompress.size(); i+=3){
        string backTobit16 = bitset<8>(dataToDecompress[i]).to_string() + bitset<8>(dataToDecompress[i+1]).to_string();

        codeword = binaryToDecimal(stoul(backTobit16));
        
		character = dataToDecompress.substr(i + 2, 1);

        dictionary.insert(pair<int, string>(counter, dictionary[codeword] + character));

        writingFile<< dictionary[codeword]<<character;
        writingFile.flush();

        counter++;
        if(counter == MaxCode){
            counter = startCode;
            dictionary.clear();
        }     

    }
    writingFile.close();
}