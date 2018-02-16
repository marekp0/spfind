#pragma once
#include <vector>

/**
 * Basically just an std::vector with a few extras
 */
template<class T>
class Sequence : public std::vector<T> {
public:
    using std::vector<T>::vector;

    /**
     * Returns the index of n, or -1 if it's not in the vector, because
     * using std::find is a pain
     */
    int find(T n);

    /**
     * Returns true if the argument is a subsequence of this sequence
     */
    bool contains(const Sequence& subseq);

    /**
     * Returns how many of other's first characters correspond to this
     * sequence's last characters. For example:
     * {1,2,3,4,5}.overlap({4,5,6,7}) returns 2
     */
    int overlap(const Sequence& other);

    /**
     * Prints the contents of the sequence to stdout (for debugging mostly)
     */
    void print() const;

    /**
     * Returns all permutations of this sequence
     */
    std::vector<Sequence> permute();

private:
    void permuteImpl(Sequence<int>& prefix, std::vector<Sequence>& perms);
};

typedef Sequence<int> IntSequence;

#include "Sequence.ipp"

