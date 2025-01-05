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
most likely wrong or lacked some details. By digging through an old
[Reddit thread](https://www.reddit.com/r/learnprogramming/comments/1apk2dk/help_needed_with_recreating_dijkstras_prime/),
I came to the conclusion that the YouTuber had most likely "tricked" 
himself by using Python and using a [heap implementation written 
in C](https://docs.python.org/3/library/heapq.html) that was so fast enough 
to hide away the overhead of the incorrect implementation. Additionally,
it appears that the sieve implementation was non-optimized, thus using more
space than the minimal possible, i.e., it didn't use a bitmap.

I corroborated this theory by looking at yet another blogger's 
implementation[^3] which was also based on the explanation from 
the YouTube video. Seeing how this implementation was similar to
mine, I drew the conclusion that the algorithm explained in the video
is not quite correct, even though it also produces prime numbers.


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


## Implemented algorithms
* `sieve`: The sieve of Eratosthenes algorithm for finding prime numbers.
  Nothing really special, but I use a bitmap to conserve some space.
* `trial`: Using trial by division for finding primes. 
  Previously found primes are stored in an array and then checked 
  against for new candidates.
* `trial_list`: A variant of the trial by division using a list
  to store primes.
* `b001`: The algorithm (as I understood it) from the YouTube video[^1].
  Uses a min-heap to store multiples/factors so that the lowest multiple
  is always first.
* `b001_array`: A variant of the algorithm[^3] by Keith Vetter.
  Instead of a heap, it uses a sorted array to store multiples/factors.
* `dijkstra`: An implementation of the algorithm from the Tannenbaum
  paper.
* `dijkstra_optimized`: The algorithm from the Tannenbaum paper but with
  less fancy array/realloc handling. Slightly faster.


## Building and running
Run `make` as follows to build executables in the project directory:
```
$ make release
```

The release targets makes sure that `-O3` is set and sets the `NDEBUG` define
for all the programs. The implementations use this to skip freeing memory 
on completion, since we don't want to benchmark clean up code.

All C source files in `src` are expected to be stand-alone executables, i.e.,
C code with `main()`. This just made it easier to implement multiple algorithms.

All programs expect a number `N` as argument, for finding prime numbers up to that number. 
For example, `$ ./sieve 100` prints all prime numbers up to 100.

The benchmark script uses `/usr/bin/time` for measuring run-time (user time) and memory usage:
```
$ ./benchmark
Benchmark N=10000000, detected algorithms: sieve dijkstra trial_list trial optimized_dijkstra b001 b001_array
Running sieve...OK; 0.04 seconds (2748 kB)
Running dijkstra...OK; 0.23 seconds (6588 kB)
Running trial_list...OK; 0.69 seconds (21980 kB)
Running trial...OK; 0.67 seconds (6748 kB)
Running optimized_dijkstra...OK; 0.23 seconds (6824 kB)
Running b001...OK; 1.04 seconds (11720 kB)
Running b001_array...OK; 5.69 seconds (1596 kB)
```

When piped to file, the benchmark script will produce a CSV formatted output.

## Conclusion
I ran on my Lenovo ThinkPad E14 Gen4 laptop, with an AMD Ryzen 7 5825U CPU
and 40 GiB of RAM. Using Fedora Linux 41 with the 6.12.6 version of the Linux
kernel and GCC version 14.2.1.

As mentioned above, the algorithm as presented in the original YouTube video
is most likely not entirely correct. The sieve of Eratosthenes is the fastest
algorithm, and by using a bitmap, it's memory usage can be quite condensed
down. `-O3` seems to have a really huge effect, particularly on the slowest 
algorithms, which probably means that I'm doing something silly somewhere.


[^1]: b001, "[Dijkstra's Hidden Prime Finding Algorithm](https://www.youtube.com/watch?v=fwxjMKBMR7s)". February 2024. Watched: 2025-01-02.
[^2]: Heinrich Hartmann, "[Dijkstra's Prime Number Algoritm](https://www.heinrichhartmann.com/archive/Dijkstra%27s-Prime-Number-Algorithm.html)". April 2016. Retrieved: 2025-01-04.
[^3]: Keith Vetter, "[Dijkstra's Prime Algorithm](https://wiki.tcl-lang.org/page/Dijkstra%27s+Prime+Algorithm). February 2024. Retrieved: 2025-01-02.
[^4]: Andrew S. Tanenbaum, "[A General-Purpose Macro Processor as a Poor Man's Compiler-Compiler](https://ieeexplore.ieee.org/document/170A2350)". In _IEEE Transactions on Software Engineering_, vol. SE-2, no. 2, pp. 121-125, June 1976. DOI: [10.1109/TSE.1976.233539](https://dx.doi.org/https://www.heinrichhartmann.com/archive/Dijkstra%27s-Prime-Number-Algorithm.html)
