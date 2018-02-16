#include "PermCache.hpp"

PermCache::PermCache(int n) : mN(n)
{
    // calculate all permutations of {1, 2, ..., n}
    IntSequence seq;
    for (int i = 0; i < n; ++i)
        seq.push_back(i+1);
    mPerms = seq.permute();

    // calculate all overlaps
    mOverlaps.resize(mPerms.size()*mPerms.size());
    for (int i = 0; i < mPerms.size(); ++i) {
        for (int j = 0; j < mPerms.size(); ++j) {
            mOverlaps[i*mPerms.size() + j] = mPerms[i].overlap(mPerms[j]);
        }
    }
}

int PermCache::getOverlap(PermId left, PermId right)
{
    return mOverlaps[left*mPerms.size() + right];
}

