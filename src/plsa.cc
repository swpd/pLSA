#include <fstream>
#include <iostream>

#include "util.h"
#include "plsa.h"

PLSA::PLSA() {
}

PLSA::~PLSA() {
}

void PLSA::Train(const std::string &corpora_file, const int num_topics,
        const int epoch) {
    num_topics_ = num_topics;

    LoadCorpora(corpora_file);
    RandomInit();

    for (int i = 1; i <= epoch; ++i) {
        if (i % 10 == 0) std::cout << "Epoch " << i << " ..." << std::endl;
        EStep();
        MStep();
    }
}

void PLSA::SaveModel(const std::string &model_dir) {
    std::string doc_topic_file = model_dir;
    std::string topic_word_file = model_dir;
    if (model_dir[model_dir.size() - 1] != '/') {
        doc_topic_file += '/';
        topic_word_file += '/';
    }
    doc_topic_file += "doc_topic.txt";
    topic_word_file += "topic_word.txt";

    std::ofstream doc_topic_fout(doc_topic_file);
    for (size_t d = 0; d < corpora_.Size(); ++ d) {
        for (size_t z = 0; z < num_topics_; ++z) {
            doc_topic_fout << doc_topic_[d][z] << " ";
        }
        doc_topic_fout << std::endl;
    }
    doc_topic_fout.close();

    std::ofstream topic_word_fout(topic_word_file);
    for (size_t z = 0; z < num_topics_; ++z) {
        for (auto w = vocabulary_.begin(); w != vocabulary_.end(); ++w) {
            topic_word_fout << topic_word_[z][*w] << " ";
        }
        topic_word_fout << std::endl;
    }
    topic_word_fout.close();
}

void PLSA::RandomInit() {
    // init doc-topic matrix
    for (size_t d = 0; d < corpora_.Size(); ++d) {
        double sum = 0;
        for (size_t z = 0; z < num_topics_; ++z) {
            doc_topic_[d][z] = Random();
            sum += doc_topic_[d][z];
        }
        for (size_t z = 0; z < num_topics_; ++z) {
            doc_topic_[d][z] /= sum;
        }
    }

    // init topic-word matrix
    for (size_t z = 0; z < num_topics_; ++z) {
        double sum = 0;
        for (auto w = vocabulary_.begin(); w != vocabulary_.end(); ++w) {
            topic_word_[z][*w] = Random();
            sum += topic_word_[z][*w];
        }
        for (auto w = vocabulary_.begin(); w != vocabulary_.end(); ++w) {
            topic_word_[z][*w] /= sum;
        }
    }
}

void PLSA::LoadCorpora(const std::string &corpora_file) {
    std::ifstream fin(corpora_file);
    size_t word_nums;
    int word_idx, word_cnt;
    char separator;
    while (fin >> word_nums) {
        // per document
        Document doc;
        for (size_t i = 0; i < word_nums; ++i) {
            fin >> word_idx >> separator >> word_cnt;
            doc.AddWord(word_idx, word_cnt);
            vocabulary_.insert(word_idx);
        }
        corpora_.AddDocument(doc);
    }
    fin.close();
}

// calculate posterior topic distribution
void PLSA::EStep() {
    for (size_t d = 0; d < corpora_.Size(); ++d) {
        for (auto w = vocabulary_.begin(); w != vocabulary_.end(); ++w) {
            double sum = 0;
            for (size_t z = 0; z < num_topics_; ++z) {
                posterior_z_[z][d][*w] = doc_topic_[d][z] * topic_word_[z][*w];
                sum += posterior_z_[z][d][*w];
            }
            for (size_t z = 0; z < num_topics_; ++z) {
                posterior_z_[z][d][*w] /= sum;
            }
        }
    }
}

void PLSA::MStep() {
    // update doc-topic matrix
    for (size_t d = 0; d < corpora_.Size(); ++d) {
        Document doc = corpora_[d];
        for (size_t z = 0; z < num_topics_; ++z) {
            double sum = 0;
            for (size_t w = 0; w < doc.Size(); ++w) {
                Word word = doc[w];
                sum += word.cnt * posterior_z_[z][d][word.idx];
            }
            doc_topic_[d][z] = sum / doc.Length();
        }
    }

    // update topic-word matrix
    for (size_t z = 0; z < num_topics_; ++z) {
        std::map<int, double> sum;
        double norm = 0;
        for (size_t d = 0; d < corpora_.Size(); ++d) {
            Document doc = corpora_[d];
            for (size_t w = 0; w < doc.Size(); ++w) {
                Word word = doc[w];
                sum[word.idx] += word.cnt * posterior_z_[z][d][word.idx];
                norm += word.cnt * posterior_z_[z][d][word.idx];
            }
        }
        for (auto w = vocabulary_.begin(); w != vocabulary_.end(); ++w) {
            topic_word_[z][*w] = sum[*w] / norm;
        }
    }
}
