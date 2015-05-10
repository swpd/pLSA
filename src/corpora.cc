#include <assert.h>
#include "corpora.h"

Document::Document() : length_(0) {
}

Document::~Document() {
}

Word& Document::operator[](size_t idx) {
    assert(idx >= 0 && idx < words_.size());
    return words_[idx];
}

void Document::AddWord(const int word_idx, const int word_cnt) {
    words_.push_back(Word(word_idx, word_cnt));
    length_ += word_cnt;
}

Corpora::Corpora() {
}

Corpora::~Corpora() {
}

Document& Corpora::operator[](size_t idx) {
    assert(idx >= 0 && idx < docs_.size());
    return docs_[idx];
}

void Corpora::AddDocument(const Document &doc) {
    docs_.push_back(doc);
}
