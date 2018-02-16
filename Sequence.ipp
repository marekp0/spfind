#pragma once
#include "Sequence.hpp"
#include <string>
#include <cstdio>

template<class T>
int Sequence<T>::find(T n)
{
    for (int i = 0; i < this->size(); ++i) {
        if (this->at(i) == n)
            return i;
    }
    return -1;
}

template<class T>
bool Sequence<T>::contains(const Sequence<T>& subseq)
{
    for (int i = 0; i < this->size() - subseq.size() + 1; ++i) {
        int j = 0;
        for (; j < subseq.size(); ++j) {
            if (this->at(i+j) != subseq[j])
                break;
        }
        if (j == subseq.size())
            return true;
    }
    return false;
}

template<class T>
int Sequence<T>::overlap(const Sequence<T>& other)
{
    for (int ovr = other.size(); ovr > 0; --ovr) {
        if (ovr >= this->size())
            continue;
        int i = 0;
        for (; i < ovr; ++i) {
            if (this->at(this->size() - ovr + i) != other[i])
                break;
        }
        if (i == ovr)
            return ovr;
    }
    return 0;
}

template<class T>
void Sequence<T>::print() const
{
    printf("[");
    for (auto i : *this) {
        auto str = std::to_string(i);
        printf("%s ", str.c_str());
    }
    printf("]\n");
}

template<class T>
std::vector<Sequence<T>> Sequence<T>::permute()
{
    Sequence<int> prefix;
    std::vector<Sequence> perms;
    permuteImpl(prefix, perms);
    return perms;
}

template<class T>
void Sequence<T>::permuteImpl(Sequence<int>& prefix, std::vector<Sequence<T>>& perms)
{
    if (this->size() == prefix.size()) {
        Sequence seq;
        for (int i : prefix)
            seq.push_back(this->at(i));
        perms.push_back(seq);
    }
    else {
        for (int i = 0; i < this->size(); ++i) {
            if (prefix.find(i) == -1) {
                prefix.push_back(i);
                permuteImpl(prefix, perms);
                prefix.pop_back();
            }
        }
    }
}

