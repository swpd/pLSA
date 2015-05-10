#ifndef CORPORA_H_
#define CORPORA_H_

#include <vector>

struct Word {
    int idx;
    int cnt;
    Word(int idx, int cnt) : idx(idx), cnt(cnt) {
    }
};

class Document {
public:
    Document();
    virtual ~Document();

    Word& operator[](size_t idx);
    void AddWord(const int word_idx, const int word_cnt);
    size_t Size() const {
        return words_.size();
    }
    size_t Length() const {
        return length_;
    }

private:
    size_t length_;
    std::vector<Word> words_;
};

class Corpora {
public:
    Corpora();
    virtual ~Corpora();

    Document& operator[](size_t idx);
    void AddDocument(const Document &doc);
    size_t Size() const {
        return docs_.size();
    }

private:
    std::vector<Document> docs_;
};

#endif
