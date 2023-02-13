#include <iostream>
#include "lz78.h"
int main(){
    lz78 trial;
    string filename = "test5.txt";
    if(trial.getsizeOfFile(filename) < UINT8_MAX){
        cout<<"using 16 byte compression"<<endl;
        trial.compress(filename,"testOutput.txt");
        trial.decompress("testOutput.txt","decompressionOutput.txt");
    }else{
        cout<<"using 64 byte compression"<<endl;
        cout<<filename<<endl;
        trial.compress16(filename,"testOutput.txt");
        trial.decompress16("testOutput.txt","decompressionOutput.txt");
    }
    //we compare decompressionOutput.txt to filename and make sure it is the same
    //if it is not the same, we know something went wrong
    ifstream file1,file2;
    file1.open(filename);
    file2.open("decompressionOutput.txt");
    string line1,line2;
    while(getline(file1,line1)){
        getline(file2,line2);
        if(line1 != line2){
            cout<<"something went wrong"<<endl;
            return 1;
        }
    }
    cout<<"everything went well"<<endl;
    return 0;
}