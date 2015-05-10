#include <iostream>
using namespace std;

#include "plsa.h"

void help() {
    cerr << "Usage: plsa <corpora file> <topic number> <epoch number> <model dir>"
        << endl;
}

int main(int argc, char *argv[])
{
    if (argc != 5) {
        help();
        return 1;
    }

    string corpora_file(argv[1]);
    int num_topics = atoi(argv[2]);
    int epoch = atoi(argv[3]);
    string model_dir(argv[4]);

    PLSA pLSA;
    cout << "Start Training pLSA Model ..." << endl;
    pLSA.Train(corpora_file, num_topics, epoch);
    cout << "Saving pLSA Model to " << model_dir << " ..." << endl;
    pLSA.SaveModel(model_dir);
    cout << "Fisished!" << endl;

    return 0;
}
