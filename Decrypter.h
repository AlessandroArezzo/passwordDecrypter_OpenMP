//
// Created by Alessandro Arezzo on 24/11/2019.
//

#ifndef PASSWORDDECRYPTER_DECRYPTER_H
#define PASSWORDDECRYPTER_DECRYPTER_H
#include <iostream>
#include <vector>

using namespace std;

class Decrypter {
private:
    vector<string> vectorPass;
    string salt;

public:
    Decrypter(string pathInputFile,string saltCharacters);

    // Methods for accessing attributes
    string getWord(int index);
    int getSize();

    // Methods to decrypt
    vector<long> decryptSequential(vector<string> passwordsToCrack,int numTest);
    vector<long> decryptParallel(vector<string> passwordsToCrack, int numTest, int numThreads);
};


#endif //PASSWORDDECRYPTER_DECRYPTER_H
