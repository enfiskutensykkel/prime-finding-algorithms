# Comparison of prime number finding algorithms
I recently came across a YouTube video[^1] explaining a prime number finding 
algorithm by Edsger W. Dijkstra. This algorithm is a trade-off between the 
[trial by division](https://en.wikipedia.org/wiki/Trial_division) approach and 
the [Sieve of Eratosthenes](https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes), 
with regard to memory consumption and run-time. However, after implementing the 
algorithm as it was explained in the video in C, I did not get the 
expected results: Trial by division ran much faster, and by using a simple bitmap,
the sieve had less of a memory footprint. 

Puzzled by this, I tried looking up the original Dijkstra algorithm to see if 
I had misunderstood something and came across a blog post[^2] detailing the
algorithm and refactoring it for a modern programming language.
Reading this, I realized that the implementation explained in the video was
most likely wrong or lacked some details. By digging through some old Reddit
threads, I came to the conclusion that the YouTuber had most likely "tricked" 
himself by using Python and using a [heap implementation written 
in C](https://docs.python.org/3/library/heapq.html) that was so fast enough 
to hide away the overhead of the incorrect implementation. Additionally,
it appears that the sieve implementation was non-optimized, thus using more
space than the minimal possible, i.e., it didn't use a bitmap.

I corroborated this by looking at another blogger's implementation[^3] 
also based on the explanation from the YouTube video, which was similar to
my initial implementation.

## Dijkstra's algorithm
This is the original algorithm (adapted to C code) from the 
original paper[^4]. The algorithm finds `N` number of primes numbers:
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


## Building and running
Run `make` as follows to build executables in the project directory:
```
$ make RELEASE=1
```

The `RELEASE=1` variable to make enables `-O3` and sets the `NDEBUG` define
for all the programs. The implementations use this to skip freeing memory 
on completion, since we don't want to benchmark clean up code.

All programs expect a number `N` as argument, for finding prime numbers up to that number. 
For example, `$ ./sieve 100` prints all prime numbers up to 100.

The benchmark script uses `/usr/bin/time` for measuring run-time (user time) and memory usage:
```
$ ./benchmark
Running programs with N=100000000...
algorithm=b001, iterations=100000000, runtime=13.12secs, memory_usage=91336kB
algorithm=trial, iterations=100000000, runtime=11.67secs, memory_usage=46248kB
algorithm=sieve, iterations=100000000, runtime=0.51secs, memory_usage=13668kB
algorithm=dijkstra, iterations=100000000, runtime=4.00secs, memory_usage=46376kB
Done
```


[^1]: b001, "[Dijkstra's Hidden Prime Finding Algorithm](https://www.youtube.com/watch?v=fwxjMKBMR7s)". February 2024. Watched: 2025-01-02.
[^2]: Heinrich Hartmann, "[Dijkstra's Prime Number Algoritm](https://www.heinrichhartmann.com/archive/Dijkstra%27s-Prime-Number-Algorithm.html)". April 2016. Retrieved: 2025-01-04.
[^3]: Keith Vetter, "[Dijkstra's Prime Algorithm](https://wiki.tcl-lang.org/page/Dijkstra%27s+Prime+Algorithm). February 2024. Retrieved: 2025-01-02.
[^4]: Andrew S. Tanenbaum, "[A General-Purpose Macro Processor as a Poor Man's Compiler-Compiler](https://ieeexplore.ieee.org/document/170A2350)". In _IEEE Transactions on Software Engineering_, vol. SE-2, no. 2, pp. 121-125, June 1976. DOI: [10.1109/TSE.1976.233539](https://dx.doi.org/https://www.heinrichhartmann.com/archive/Dijkstra%27s-Prime-Number-Algorithm.html)
