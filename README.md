Altruct is a collection of algorithms and data structures, hence the name: **Al**go + S**truct**.
It is written in C++ and predominately uses templates. This is both a curse and a blessing.
Templates allow powerful and fast abstractions at a cost of higher compilation times and harder debugging.


## Motivation
* Reusability. It is tedious to implement the same things over and over again.
 When you read a problem on a programming contest you may immediately have an idea of how to solve it.
 But before that, you need to implement several tricky algorithms and/or structures. Instead of focusing on
 a bigger picture and how to combine those building blocks, you need to spend time writing blocks themselves.
* Reliability. Related to the above point, it is a big relief to know that your building blocks are correct.
 If your solution is not working correctly, chances are much higher that the bug is in the top-level code
 and/or logical reasoning than an edge case in the implementation of the building blocks used.
* Apart from the practical reasons above, I also wanted to deepen my knowledge in computer science and mathematics.
* I also wanted to deepen my knowledge in software engineering by designing a library.
 There are many challenges involved in properly organizing the code, properly testing it,
 making sure it is robust and portable, and finally making it open source.


## Unit test coverage and samples
Altruct has extensive unit tests coverage powered by `googletest`.
Those unit tests are also its best usage examples.

### Experimental
Nearly every bit of the code has a unit test that covers it in multiple scenarios.
The code that doesn't yet have a proper unit test is included in the `experinemntal` directory.
Much like the rest of the library, the code in `experimental` has been successfully used
in multiple programming contest problems and chances of having a bug are not big.
However, until the proper unit tests get added, it will remain experimental.


## Supported plaftorms
* Altruct has been compiled on Windows, Linux and Mac OSX.
* Altruct has been compiled with MSVS, GCC and Clang.
* Altruct comes with a MSVS solution and a BUCK file.
* Code is written in endian independent way.

I occasionally check that Altruct still compiles on all of the above,
however I am almost exclusively using MSVS on Windows so there is a
posibility of something being temporarily broken for other compilers.
In the future I may set up some continuous integration tests running.


## Example

Here is a naive implementation for calculating the n-th term of a linear recurrence of degree L (in modulo M arithmetic) that works in O(L * n).

```
    int a_k3 = 1, a_k2 = 4, a_k1 = 7; // a[k-3], a[k-2], a[k-1]
    for (int k = 3; k <= n; k++) {
        int a_k = (2 * a_k1 - 3 * a_k2 + 5 * a_k3) % 1009;
        a_k3 = a_k2, a_k2 = a_k1, a_k1 = a_k;
    }
    cout << a_k1 << endl;
```

Here is how to do this in O(L^2 log n) by composing the mathematical structures Altruct provides.
The algorithm calculates the fast exponentiation of polynom `x` to the `n-th` power in O(log n) steps,
where each operation is performed modulo characteristic polynomial in O(L^2). There is a similar
algorithm that uses matrix eponentiation but that would cost O(L^3) instead of O(L^2) per step.

```
    typedef modulo<int, 1009> mod;
    typedef polynom<mod> poly;
    typedef moduloX<poly> polymod;
    // the initial values
    poly init = { 1, 4, 7 };
    // the characteristic polynomial of our recurrence
    // 0 == -a[n] + 2 a[n - 1] - 3 a[n - 2] + 5 a[n - 3]
    poly p = { 5, -3, +2, -1 };
    poly x = { 0, 1 }; // 0 + 1*x
    polymod xn = powT(polymod(x, p), n); // x^n % p(x)
    mod r = 0;
    for (int i = 0; i < p.size(); i++) {
        r += init[i] * xn.v[i];
    }
    cout << r.v << endl;
```

But of course, the aforementioned algorithm is already implemented as `linear_recurrence`,
so for that you can just use the Altruct function and not have to implement it by yourself.

```
    typedef modulo<int, 1009> mod;
    cout << linear_recurrence<mod, mod>({ 2, -3, +5 }, { 1, 4, 7 }, n).v << endl;
```

Observe how in the above example, the actual type used is `modulo<polynom<modulo<int>>>`.
All of the math structures Altruct provides can be composed in such a way where it makes sense.

The same implementation of Fast Fourier Transform and related convolutions can be used both
on complex numbers and on modulo numbers. Or on any other field provided that you know how to
calculate the n-th root of unity in it.

See below for a comprehensive listing of the library content.


## Library structure

There are several levels in the code hierarchy:
* source - This is the main body of the library.
* experimental - Same as `source`, but without proper unit test coverage yet.
* sample - A limited set of examples as `test` already shows the usage.
* test - Unit tests for the rest of the library.
* test_util - Utility code that comes handy in testing, but is not the main body of the library.

Further subdivision is about headers files and source files. All of the above use one or both of:
* include - A list of header files containing templates and declarations.
* src - A list of C++ source files. (I may have to name this directory more consistently.)

Yet further subdivision is about algorithms and data structures, and their respective subdomains.

Finally there are build files, source control files and other repository metadata.


## Implemented algorithms and structures

Here is a list of most of the things currently present in the library.
I have many more to add, but the code that is not migrated to Altruct yet
needs to be brought to a higher quality bar that is required here.

* Algorithm:
  * Graph:
    * Base
      * Graph structure
      * Iterative DFS
    * Shortest paths & spanning trees
      * Dijkstra
      * Floyd-Warshall
      * Prim's minimum spanning tree
    * Flow & matching
      * Bipartite matching
      * Dinic blocking flow (max flow)	 
      * Push-Relabel flow (max flow)
    * Decompositions & connectivity
      * Chain decomposition
        * Biconnectivity
        * Cut edges & vertices
      * Heavy-light Decomposition of a tree
      * Lowest Common Ancestor
      * Tarjan strongly connected components
      * Transitive closure & reduction
      * Topological sort
    * NP-hard
      * Chromatic polynomial
  * Hash:
    * Polynomial hash (useful for string related problems)
    * std::tuple hash
  * Math:
    * Base:
      * IdentityT, ZeroT,
      * absT, minT, maxT,
      * powT, sqT, sqrtT, cbT, cbrtT
      * gcd, gcd_ex, gcd_max, lcm
      * div_floor, div_ceil, div_round, multiple
    * Bits:
      * ilog2, lzc, tzc, bit_cnt1, bit_reverse,
      * or_down, xor_down, gray_to_bin, bin_to_gray
      * hi_bit, lo_bit, is_pow2, next_pow2
    * Combinatorial primes:
      * factorial/binomial/multinomial prime exponent
      * factorial/binomial modulo prime power
    * Combinatorics:
      * next_partition, next_combination, nth_permutation
    * Counting
      * Stirling number (first and second kind)
      * Partitions number
    * Convolutions:
      * Slow and/or/xor/max/cyclic convolution (O(n^2) implementation)
      * Fast and/or/xor/max convolution (FFT-like O(n log n) implementation)
      * Fast ordinary/cyclic convolution (FFT O(n log n) implementation)
      * Fast Walsh-Hadamard transform
      * Fast Arithmetic transform
      * Fast Fourier transform
    * Continued fractions:
      * Convergents, semi-convergents
      * Best rational approximations to sqrt(d)
      * Closest lattice point to a line
    * Fractions:
      * Farey sequence
    * Diophantine equations
      * Generalized Pell's equation `x^2 - D y^2 = N`
    * Modulos:
      * Chinese Remainder Theorem
      * Garner algorithm
      * Jacobi symbol
      * Cipolla algorithm (modular square root)
      * Hensel lift (square root modulo prime power)
      * Primitive root, primitive root of unity
      * k-th root, k-th roots of unity
      * Factorial/binomial modulo prime power
    * Polynoms:
      * Monotonic search
      * Zeros
      * Discrete integral
    * Primes:
      * Precompute for a range (1 to n, or segmented):
        * Primes, Prime-Pi, Euler-Phi (Totient), Moebius-Mu, Divisor-Sigma, Prime-Factor
        * Moebius transform, factorization
      * Compute from a given factorization:
        * Divisors, Euler-Phi, Carmichael-Lambda
        * SquaresR
      * Pollard-Rho integer factorization
      * Miller-Rabin primality test
      * Integer digits for a base
    * Prime counting:
      * PrimePi in O(n^(5/7))
      * PrimeSum in O(n^(5/7))
      * PrimePowerSum in O(n^(5/7))
    * Ranges:
      * Arithmetic progression
      * Powers, Factorials, Inverse
      * Negation, Alternate sign
      * Accumulate, Differences
    * Recurrences:
      * Linear recurrence
      * Fibonacci, Lucas-L, Lucas-U, Lucas-V
      * Bernoulli numbers
      * Berlekamp-Massey algorithm (finds the characteristic polynomial of a linear recurrence)
    * Reduce:
      * Sum, Product
      * Min, Max, Mex (Grundy minimal excludant)
    * Sums:
      * Sum of arbitrary function f
      * Sum of powers
      * Sum of `floor((a * k + b) / q)` in O(log n)
      * Sum of `f(k, floor(n / k))` in O(sqrt n)
    * Divisor sums:
      * Dirichlet convolution, division, inverse:
        * arbitrary arithmetic functions in O(n log n)
        * multiplicative functions in O(n log log n)
        * completely multiplicative functions in O(n)
      * Moebius transform:
        * arbitrary arithmetic functions in O(n log n)
        * multiplicative functions in O(n log log n)
      * Sum of multiplicative functions in O(n^(2/3)):
        * Divisor-Sigma in O(n log log n)
        * Mertens function (sum of Moebius-Mu)
        * Totient summatory function (sum of Euler-Phi)
        * Arbitrary function M such that `T(n) = Sum[M(floor(n/k))]`
    * Sequences
      * Delta, Dirichlet, Zero, One, Identity, Square, Cube
      * Triangular, Tetrahedral, Pyramidal, Octahedral, Dodecahedral, Icosahedral
      * Divisor-Sigma family partial sums
    * Triples
      * Pythagorean (90 degree) triples
      * Eisenstein (60 & 120 degree) triples
  * Parser:
    * Shunting-yard (infix expression parser & evaluator)
  * Random:
    * Mersene Twister
    * XorShift
  * Search:
    * Binary search
    * Knuth-Morris-Pratt algorithm (string search)

* Chrono
  * RDTSC clock
  * since helper

* Concurrency
  * lock helper
  * parallel_execute

* I/O:
  * Fast I/O
  * Reader: File / Stream / String / Buffered / Simple
  * Writer: File / Stream / String / Buffered / Simple

* Structure:
  * Container:
    * Aho-Corasick trie
    * Binary heap
    * Binary search tree (no balancing)
    * Treap - a variant of [randomized BST](https://en.wikipedia.org/wiki/Treap)
    * Rope - efficient [array tree](https://en.wikipedia.org/wiki/Rope_(data_structure))
    * Palindrome tree
    * Suffix array
    * Bit vector (std::bitset + std::vector<bool> + more)
    * Segment tree (range query, single element update)
    * Lazy segment tree (range query, range update with lazy propagation)
    * Low-High map, Sqrt map (efficient storage for a certain sparse data)
    * Range Minimum Query structure (O(n) build, O(1) query)
  * Graph:
    * `disjoint_set` - A [disjoint-set a.k.a. union-find](https://en.wikipedia.org/wiki/Disjoint-set_data_structure) structure
  * Math:
    * `complex` - A [Complex number](https://en.wikipedia.org/wiki/Complex_number) over arbitrary ring
    * `double_int` - Work-in-progress, division not implemented
    * `fraction` - A [fraction](https://en.wikipedia.org/wiki/Fraction_(mathematics)) over arbitrary ring
    * `galois_field_2` - [GF(2)](https://en.wikipedia.org/wiki/GF(2)) - A finite field with two elements: 0 and 1
    * `matrix` - A [matrix](https://en.wikipedia.org/wiki/Matrix_(mathematics)) over arbitrary ring
    * `modulo` - [Modular arithmetics](https://en.wikipedia.org/wiki/Modular_arithmetic) over arbitrary ring
    * `moebius_tr` - [Moebius Transformation](https://en.wikipedia.org/wiki/M%C3%B6bius_transformation)
    * `nimber` - Grundy [Nimber](https://en.wikipedia.org/wiki/Nimber) arithmetics
    * `permuation` - A [permutation](https://en.wikipedia.org/wiki/Permutation) in cycle/transposition/array notation
    * `polynom` - A [polynomial](https://en.wikipedia.org/wiki/Polynomial) with coefficients over arbitrary ring
    * `quadratic` - A number of the form `a + b * sqrt(D)`. See [Quadratic field](https://en.wikipedia.org/wiki/Quadratic_field) and [Quadratic integer](https://en.wikipedia.org/wiki/Quadratic_integer)
    * `series` - A [formal power series](https://en.wikipedia.org/wiki/Formal_power_series) over arbitrary ring. See [Generating function](https://en.wikipedia.org/wiki/Generating_function)
    * `vector2d`, `vector3d`, `vectorNd` - Essentialy a point in 2D/3D geometry. See [Euclidean vector](https://en.wikipedia.org/wiki/Euclidean_vector)
    * `fenwick_tree` - A [Fenwick tree](https://en.wikipedia.org/wiki/Fenwick_tree) structure a.k.a. "logaritamska struktura" in Croatia
    * `prime_holder` - A utility container that keeps a range of primes and related functions
    * `root_wrapper` - A utility class that wraps the root powers used in FFT
    * `with_infinity` - Extends with a point at infinity. See [Riemann sphere](https://en.wikipedia.org/wiki/Riemann_sphere)
