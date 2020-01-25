//
// Created by Alessandro Arezzo on 24/11/2019.
//

#include "Decrypter.h"
#include <unistd.h>
#include <fstream>
#include "omp.h"
#include <openssl/des.h>

using namespace std;

Decrypter::Decrypter(string pathInputFile,string saltCharacters) {
    salt=saltCharacters;

    // Reads input file and inserts passwords into the vector
    ifstream dict;
    string line;
    dict.open(pathInputFile);
    if( dict.fail() )
    {
        std::cout << "Error: Password dict file did not open\n";
        exit(1);
    }
    int i = 0;
    while (getline(dict, line)) {
        vectorPass.push_back(line);
        i++;
    }
    dict.close();

}

// Returns word in vector by index
string Decrypter::getWord(int index){
    return vectorPass[index];
}

// Returns size of the vector
int Decrypter::getSize() {
    return vectorPass.size();
}

// Sequential decryption
vector<long> Decrypter::decryptSequential(vector<string> passwordsToCrack,int numTest) {
    vector<long> timeDecrypt;
    string password;
    long sumTimeDecrypt=0, meanTimeForOnePassword=0;
    timeDecrypt.reserve(passwordsToCrack.size());

    for(int i=0;i<passwordsToCrack.size();i++) {
        sumTimeDecrypt=0;
        cout<<"Starting experiments on password #"<<i<<" with hash: "<<passwordsToCrack[i]<<" --> Executing "<<numTest<<" tests...\n";
        for (int j = 0; j < numTest; j++) {
            auto start = chrono::steady_clock::now();
            for(int k=0;k<vectorPass.size();k++){
                string encryption=crypt(vectorPass[k].c_str(), salt.c_str());
                if( strcmp(encryption.c_str(), passwordsToCrack[i].c_str()) == 0){
                    //cout<<"Hash found: "<<encryption<<" --> Password: "<<vectorPass[k]<<endl;
                    break;
                }
            }
            auto stop = chrono::steady_clock::now();
            auto elapsed = chrono::duration_cast<chrono::nanoseconds>(stop - start).count();
            sumTimeDecrypt += elapsed;
        }
        meanTimeForOnePassword=sumTimeDecrypt/numTest;
        timeDecrypt.push_back(meanTimeForOnePassword);
    }
    return timeDecrypt;
}


// Parallel decryption
vector<long> Decrypter::decryptParallel(vector<string> passwordsToCrack, int numTest, int numThreads) {
    vector<long> timeDecrypt;
    char *encryption;
    long sumTimeDecrypt=0, meanTimeForOnePassword=0;
    string hash;
    volatile bool flag;
    int threadsElements = static_cast<int>((double)vectorPass.size() / (double)numThreads);
    timeDecrypt.reserve(passwordsToCrack.size());
    for (int i = 0; i < passwordsToCrack.size(); i++) {
        hash=passwordsToCrack[i];
        sumTimeDecrypt = 0;
        for (int j = 0; j < numTest; j++) {
            flag = false;
            auto start = chrono::steady_clock::now();
#pragma omp parallel num_threads(numThreads) shared(flag) private(encryption) firstprivate(threadsElements,hash)
            {
                encryption = (char *) malloc(sizeof(string));
                int numThread = omp_get_thread_num();
                for (int index = threadsElements * numThread;
                     index < (numThread + 1) * threadsElements; index++) {
                    if (flag==false && index < vectorPass.size()) {

                        DES_fcrypt(vectorPass[index].c_str(), salt.c_str(),encryption);
                        if (strcmp(encryption, hash.c_str()) == 0) {
                            flag = true;
                            break;
                        }
                    }
                    else{
                        break;
                    }
                }
            }
            auto stop = chrono::steady_clock::now();
            auto elapsed= chrono::duration_cast<chrono::nanoseconds>(stop - start).count();
            sumTimeDecrypt += elapsed;
        }
        meanTimeForOnePassword = sumTimeDecrypt / numTest;
        timeDecrypt.push_back(meanTimeForOnePassword);
    }
    return timeDecrypt;
}
