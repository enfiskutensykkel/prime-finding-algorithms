
## Dijkstra's algorithm
This is the original algorithm (adapted to C code) from the original paper [2]. The algorithm finds the first `N` primes.

```C
const int N = 100;
const int M = 10;  // square root of 100

int x, square, i, k, lim, prim, P[N], V[M];

P[1] = 2;  // set the initial prime
printf("%d\n", 2);

x = 1;
lim = 1;
square = 4;  // set the initial multiple of the first prime

for (i = 2; i < N; ++i) {
    do {
        x = x + 2;
        if (square <= x) {
            V[lim] = square;
            ++lim;
            square = P[lim] * P[lim];
        }

        k = 2;
        prim = true;

        while (prim && k < lim) {
            if (V[k] < x) {
                V[k] += P[k];
            }
            prim = x != V[k];
            ++k;
        }
    } while (!prim); // uses repeat - until prim

    P[i] = x;
    printf("%d\n", x);
}
```
Note that it assumes array indices starts on 1.


[1]: b001's YouTube video: "[Dijkstra's Hidden Prime Finding Algorithm](https://www.youtube.com/watch?v=fwxjMKBMR7s)". February 2024. Watched: 2025-01-02.
[2]: Andrew S. Tanenbaum, "[A General-Purpose Macro Processor as a Poor Man's Compiler-Compiler](https://ieeexplore.ieee.org/document/170A2350)". In _IEEE Transactions on Software Engineering_, vol. SE-2, no. 2, pp. 121-125, June 1976. DOI: [10.1109/TSE.1976.233539](https://dx.doi.org/https://www.heinrichhartmann.com/archive/Dijkstra%27s-Prime-Number-Algorithm.html)
[3]: Heinrich Hartmann, "[Dijkstra's Prime Number Algoritm](https://www.heinrichhartmann.com/archive/Dijkstra%27s-Prime-Number-Algorithm.html)". April 2016. URL: [https://www.heinrichhartmann.com/archive/Dijkstra%27s-Prime-Number-Algorithm.html]". Retrieved: 2025-01-04.
