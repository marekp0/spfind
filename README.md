# spfind
`spfind` is a superpermutation finder. Inspired by
[this numberphile video](https://www.youtube.com/watch?v=wJGE4aEWc28).

## Building
Just run `make`. Requires C++14.

## Sample Usage
```
$ ./spfind 4
Done!
States expanded: 35
Sequence length: 33
Sequence: [1 2 3 4 1 2 3 1 4 2 3 1 2 4 3 1 2 1 3 4 2 1 3 2 4 1 3 2 1 4 3 2 1 ]
```

## Status
Currently very slow and memory inefficient. Only values up to 5 work at the
moment. Current plans:
 * Use SMA\* instead of A\* for memory reasons
 * Find some way to improve the heuristic

