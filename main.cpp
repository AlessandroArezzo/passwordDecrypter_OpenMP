#include <iostream>
#include <vector>
#include "Decrypter.h"
#include <unistd.h>
#include <fstream>

using namespace std;

long getMeanTime(std::vector<long> vector){
    long sum = 0;
    long mean_time = 0;
    for (int j = 0; j < vector.size(); j++) {
        sum += vector[j];
    }
    mean_time = sum / vector.size();
    return mean_time;
}

void printSpeedup(std::vector<long> vector,std::vector<long> time_seq,std::vector<string> passwords){

    for(int i=0;i<passwords.size();i++){
        cout<<"Speedup "<< passwords[i]<< ": " << (float)time_seq[i] / (float)vector[i] << endl;
    }
}

void printMeanSpeedUp(long mean_time_seq,std::vector<long> parallel_times){
    long mean_time_parallel=getMeanTime(parallel_times);
    cout << "Speedup mean:" << (float)mean_time_seq / (float)mean_time_parallel << endl;
}

vector<string> cryptData(std::vector<string> passwords,string salt){
    vector<string> data_crypted;
    data_crypted.reserve(passwords.size());
    string password_crypted;
    for (string & password : passwords) {
        password_crypted = crypt(password.c_str(), salt.c_str());
        data_crypted.push_back(password_crypted);
    }
    return data_crypted;
}

int main(int argc, char** argv) {
    int num_exp=0,num_exp_random=0,num_words_random=0;
    vector<long> vectorSequential,vectorSequentialRandom;
    long mean_time_seq_random;
    vector<string> passwords_to_crack,hash_to_crack,password_random_to_crack,random_hash_to_crack;

    // Set number of tests
    num_exp=atoi(argv[1]);
    num_exp_random=atoi(argv[2]);
    if(num_exp_random) {
        num_words_random=atoi(argv[3]);
    }

    std::vector<int> num_threads = {2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 200, 300, 400 ,500, 1000,1500};
    // Initialize decrypter
    string salt="aa";
    Decrypter* decrypter=new Decrypter("../input/passwords.txt",salt);

    cout<<"START SEQUENTIAL DECRYPTION...\n"; 
    // Sequential decription of special passwords
    if(num_exp>0) {
        passwords_to_crack={"password","jrock521","10022513"};
        hash_to_crack=cryptData(passwords_to_crack,salt);
        cout << "------Special Tests------" << endl;
        vectorSequential = decrypter->decryptSequential(hash_to_crack, num_exp);
    }
    // Sequential decription of random passwords
    if(num_exp_random>0) {
        int size=decrypter->getSize(),index;
        for(int i=0;i<num_words_random;i++){
            index = rand() % size;
            password_random_to_crack.push_back(decrypter->getWord(index));
        }
        random_hash_to_crack=cryptData(password_random_to_crack,salt);
        cout << "------Random Tests------" << endl;
        vectorSequentialRandom = decrypter->decryptSequential(random_hash_to_crack, num_exp_random);
        mean_time_seq_random=getMeanTime(vectorSequentialRandom);
    }

    cout<<"START PARALLEL DECRYPTION...\n";
    vector<long> vector;
    for(int i=0;i<num_threads.size();i++) {
        cout << "------------" << num_threads[i] << " threads------------" << endl;

        // Parallel decription of special passwords
        if(num_exp>0) {
            cout << "------Special Tests------" << endl;
            vector = decrypter->decryptParallel(hash_to_crack, num_exp, num_threads[i]);
            printSpeedup(vector, vectorSequential, passwords_to_crack);
        }

        // Parallel decription of random passwords
        if(num_exp_random>0) {
            cout << "-------Random Tests------" << endl;
            vector = decrypter->decryptParallel(random_hash_to_crack, num_exp_random, num_threads[i]);
            printMeanSpeedUp(mean_time_seq_random, vector);
        }
    }
    return 0;
}

