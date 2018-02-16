#pragma once

#include "Sequence.hpp"

/**
 * The ID of a permutation for a particular n.
 */
typedef int PermId;

/**
 * Maintains a list of permutations and overlaps between all pairs of
 * permutations.
 */
class PermCache {
public:
    PermCache(int n);
   
    int getN() const { return mN; }
    const std::vector<IntSequence>& getPerms() { return mPerms; }
    int getOverlap(PermId left, PermId right);

private:
    int mN;
    std::vector<IntSequence> mPerms;
    std::vector<int> mOverlaps;
};

