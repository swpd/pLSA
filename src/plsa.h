#ifndef PLSA_H_
#define PLSA_H_

#include <string>
#include <map>
#include <set>

#include "corpora.h"

class PLSA {
public:
    PLSA();
    virtual ~PLSA();

    void Train(const std::string &corpora_file, const int num_topics,
            const int epoch);
    void SaveModel(const std::string &model_dir);
    void LoadModel(const std::string &model_dir);

private:
    void RandomInit();
    void LoadCorpora(const std::string &corpora_file);

    void EStep();
    void MStep();

    size_t num_topics_;
    Corpora corpora_;
    std::set<int> vocabulary_;
    // P(z_k|d_i)
    std::map<int, std::map<int, double> > doc_topic_;
    // P(w_j|z_k)
    std::map<int, std::map<int, double> > topic_word_;
    // P(z_k|d_i, w_j)
    std::map<int, std::map<int, std::map<int, double> > > posterior_z_;
};

#endif
