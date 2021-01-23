#pragma once

#include "base.h"

#include <vector>
#include <string>

namespace altruct {
namespace math {

/**
 * Prime sieve of Eratosthenes up to `n`
 *
 * Performs Eratosthenes prime sieving and stores the result in the provided
 * arrays. Array `p` contains all the primes up to `n`, whereas array `q`
 * contains flags whether the number at the given index is prime or not. If
 * null pointer is passed for one of those arrays, the corresponding output
 * won't be stored. At least one of `p` or `q` needs to be non-null.
 *
 * If only `p` is specified, `p` needs to be of size at least `n`.
 * If both `p` and `q` are specified, `p` needs to be of size at least `pi(n)`.
 * `q` if specified, needs to be of size at least `n`.
 *
 * Complexity: O(n log log n)
 *
 * @param p - array to store primes up to `n`, or null if not required
 * @param q - array to store prime flags up to `n`, or null if not required
 * @param n - performs sieving for numbers up to `n` (exclusive)
 * @return - number of primes up to `n`
 */
int primes(int *p, char *q, int n);

/**
 * Prime Pi (Number of primes) up to `n`
 *
 * Stores the number of primes up to `i` for each integer `i` less than `n`.
 *
 * Complexity: O(n)
 *
 * @param pi - array to store the number of primes
 * @param n - calculates number of primes up to `n` (exclusive)
 * @param p - array of prime numbers up to `n`
 * @param m - number of prime numbers up to `n`
 */
void prime_pi(int *pi, int n, const int *p, int m);

/**
 * Euler's Phi (Number of coprimes; Totient) up to `n`
 *
 * Stores the number of numbers coprime to `i` up to `i` for each integer `i`
 * less than `n`.
 *
 * Complexity: O(n log log n)
 *
 * @param phi - array to store the number of coprimes
 * @param n - calculate number of coprimes up to `n` (exclusive)
 * @param p - array of prime numbers up to `n`
 * @param m - number of prime numbers up to `n`
 */
void euler_phi(int *phi, int n, const int *p, int m);

/**
 * Moebius Mu (+/-1 if squarefree, 0 otherwise) up to `n`
 *
 * Moebius Mu for each integer `i` is defined as:
 *    0 if `i` is not a square-free positive integer
 *   +1 if `i` is a square-free positive integer with an even number of prime factors
 *   -1 if `i` is a square-free positive integer with an odd number of prime factors
 *
 * Complexity: O(n log log n)
 *
 * @param mu - array to store the result
 * @param n - calculate moebius mu up to `n` (exclusive)
 */
void moebius_mu(int *mu, int n, const int* p = 0, int m = 0);

/**
 * Prime Nu up to `n`
 *
 * Calculates the number of distinct primes in `i` for each integer `i` less than `n`.
 *
 * Complexity: O(n log log n)
 *
 * @param nu - array to store the result
 * @param n - calculate prime nu up to `n` (exclusive)
 */
void prime_nu(int *nu, int n, const int* p, int m);

/**
 * Segmented PrimeQ in range `[b, e)`
 *
 * Stores whether `i` is prime for each integer `i` in range `[b, e)`.
 * Prime numbers up to `sqrt(e)` should be provided. I.e. `p[m-1] >= sqrt(e-1)`.
 *
 * Complexity: O((e - b) log log e)
 *
 * @param q - array to store the result
 * @param b, e - range `[b, e)`
 * @param p - array of prime numbers up to `sqrt(e)`
 * @param m - number of prime numbers up to `sqrt(e)`
 */
void segmented_q(char* q, int64_t b, int64_t e, const int *p, int m);

/**
 * Segmented Euler's Phi (Number of coprimes; Totient) in range `[b, e)`
 *
 * Stores the number of numbers coprime to `i` up to `i` for each integer `i`
 * in range `[b, e)`.
 * Prime numbers up to `sqrt(e)` should be provided. I.e. `p[m-1] >= sqrt(e-1)`.
 *
 * Complexity: O((e - b) log log e)
 *
 * @param phi - array to store the number of coprimes
 * @param tmp - temporary array
 * @param b, e - calculate number of coprimes in range `[b, e)`
 * @param p - array of prime numbers up to `sqrt(e)`
 * @param m - number of prime numbers up to `sqrt(e)`
 */
void segmented_phi(int64_t *phi, int64_t *tmp, int64_t b, int64_t e, const int *p, int m);

/**
 * Segmented Moebius Mu (+/-1 if squarefree, 0 otherwise) in range `[b, e)`
 *
 * Moebius Mu for each integer `i` is defined as:
 *    0 if `i` is not a square-free positive integer
 *   +1 if `i` is a square-free positive integer with an even number of prime factors
 *   -1 if `i` is a square-free positive integer with an odd number of prime factors
 * Prime numbers up to `sqrt(e)` should be provided. I.e. `p[m-1] >= sqrt(e-1)`.
 *
 * Complexity: O((e - b) log log e)
 *
 * @param mu - array to store the result
 * @param b, e - calculate number of coprimes in range `[b, e)`
 * @param p - array of prime numbers up to `sqrt(e)`
 * @param m - number of prime numbers up to `sqrt(e)`
 */
void segmented_mu(int64_t *mu, int64_t b, int64_t e, const int *p, int m);

/**
 * Divisor Sigma 0 (Number of divisors) up to `n`
 *
 * Complexity: O(n log n)
 *
 * @param ds0 - array to store the result
 * @param n - calculate number of divisors up to `n` (exclusive)
 */
void divisor_sigma0(int *ds0, int n);

/**
 * Divisor Sigma 1 (Sum of divisors) up to `n`
 *
 * Complexity: O(n log n)
 *
 * @param ds1 - array to store the result
 * @param n - calculate sum of divisors up to `n` (exclusive)
 */
void divisor_sigma1(int64_t *ds1, int n);

/**
  * Smallest prime factor for composite integers up to `n`, 0 for primes.
  *
  * Computes the smallest prime factor for composite integers up to n.
  * For primes, 0 is stored instead. This allows factoring 64-bit integers
  * as the smallest prime factor can only be greater than sqrt(n) for primes.
  * 
  * Output array `spf` needs to be of size `n`.
  *
  * Complexity: O(n log log n)
  *
  * @param spf - array of size `n` to store the result
  * @param n - factors numbers up to `n` (exclusive)
  */
void smallest_factor(int* spf, size_t n);

/**
 * Biggest prime factor for integers up to `n`
 *
 * Computes the biggest prime factor for integers up to n.
 *
 * Complexity: O(n log log n)
 *
 * @param bpf - array of size `n` to store the result
 * @param n - factors numbers up to `n` (exclusive)
 * @param p - array of prime numbers up to `n`
 * @param m - number of prime numbers up to `n`
 */
void factor(int *bpf, int n, const int *p, int m);

/**
 * Prime factorization of integer `n`
 *
 * Stores the prime factors and their exponents to the map `mf`. This
 * function requires a prime factors array for integers up to `n` inclusive
 * to be provided. That array can be precalculated with the `factor` function.
 *
 * Complexity: O(log n / log log n)
 *
 * @param mf - map to store prime factors and the corresponding exponents
 * @param n - integer to factor
 * @param pf - array of prime factors for integers up to `n` inclusive
 */
template<typename M>
void factor_integer_to_map(M &mf, int n, const int *pf) {
    while (n > 1) {
        int p = pf[n], e = 0;
        while (n % p == 0) {
            n /= p, e++;
        }
        mf[p] += e;
    }
}

/**
 * Prime factorization of integer `n`
 *
 * Stores the prime factors and their exponents to the vector `vf`. This
 * function requires a prime factors array for integers up to `n` inclusive
 * to be provided. That array can be precalculated with the `factor` function.
 *
 * Complexity: O(log n / log log n)
 *
 * @param vf - vector to store prime factors and the corresponding exponents
 * @param n - integer to factor
 * @param pf - array of prime factors for integers up to `n` inclusive
 */
void factor_integer(std::vector<std::pair<int, int>> &vf, int n, const int *pf);

/**
 * Prime factorization of the product of integers `vn`
 *
 * Stores the prime factors and their exponents to the vector `vf`. This
 * function requires a prime factors array for integers up to `n` inclusive
 * to be provided, where `n` is the largest element in `vn`. That array can
 * be precalculated with the `factor` function.
 *
 * Complexity: O(k log n / log log n)
 *
 * @param vf - vector to store prime factors and the corresponding exponents
 * @param vn - integers whose product to factor
 * @param pf - array of prime factors for integers up to `n` inclusive
 */
void factor_integer(std::vector<std::pair<int, int>> &vf, std::vector<int> vn, const int *pf);

/**
 * Calculates divisors from a factorization.
 *
 * Stores the divisors to the vector `vd`. Only divisors up to `maxd` are stored.
 *
 * @param vd - vector to store divisors
 * @param vf - prime factorization of the original number
 * @param maxd - maximum divisor to store, 0 for all divisors
 * @param d - reserved
 * @param i - reserved
 */
template<typename D, typename P>
void divisors(std::vector<D> &vd, const std::vector<std::pair<P, int>> &vf, D maxd = 0, D d = 1, int i = 0) {
    if (i >= (int)vf.size()) {
        vd.push_back(d);
        return;
    }
    const auto &f = vf[i];
    for (int e = 0; e <= f.second; e++) {
        divisors(vd, vf, maxd, d, i + 1);
        if (maxd > 0 && d > maxd / f.first) break;
        d *= f.first;
    }
}

/**
 * Extracts prime factors from a factorization.
 *
 * Extracts the first element `p` of each pair `(p, e)`.
 */
template<typename P>
std::vector<P> prime_factors(const std::vector<std::pair<P, int>> &vf) {
    std::vector<P> vp;
    vp.reserve(vf.size());
    for (const auto& f : vf) {
        vp.push_back(f.first);
    }
    return vp;
}

/**
* Extracts prime exponents from a factorization.
*
* Extracts the second element `e` of each pair `(p, e)`.
*/
template<typename P>
std::vector<int> prime_exponents(const std::vector<std::pair<P, int>> &vf) {
    std::vector<int> ve;
    ve.reserve(vf.size());
    for (const auto& f : vf) {
        ve.push_back(f.second);
    }
    return ve;
}

/**
 * Calculates number of divisors from a factorization.
 */
template<typename P, typename I = P>
I divisor_sigma0(const std::vector<std::pair<P, int>> &vf) {
    I r = 1;
    for (const auto& f : vf) {
        r *= f.second + 1;
    }
    return r;
}

/**
 * Calculates Euler Phi from a factorization.
 */
template<typename P, typename I = P>
I euler_phi(const std::vector<std::pair<P, int>> &vf) {
    I r = 1;
    for (const auto& f : vf) {
        r *= powT<I>(f.first, f.second - 1) * (f.first - 1);
    }
    return r;
}

/**
 * Calculates Carmichael Lambda from a factorization.
 */
template<typename P, typename I = P>
I carmichael_lambda(const std::vector<std::pair<P, int>> &vf) {
    I r = 1;
    for (const auto& f : vf) {
        int e = (f.first == 2 && f.second > 2) ? f.second - 1 : f.second;
        r = lcm<I>(r, powT<I>(f.first, e - 1) * (f.first - 1));
    }
    return r;
}

/**
 * Gives the vector of digits of `n` in base `b`.
 *
 * @param n - number to extract the digits of
 * @param b - base
 * @param len - zeros are added if necessary to fill the `len` digits
 */
template<typename I, typename B>
std::vector<B> integer_digits(const I& n, const B& b, int len = 0) {
    std::vector<B> vd;
    for (I t = n; t > 0; t /= b) {
        vd.push_back(castOf<B, I>(t % b));
    }
    while ((int)vd.size() < len) {
        vd.push_back(0);
    }
    return vd;
}

/**
 * Gives the integer from its digits in base `b`.
 *
 * @param vd - digits
 * @param b - base
 */
template<typename I, typename B>
I from_digits(const std::vector<B>& vd, const B& b) {
    I n = 0;
    for (auto it = vd.rbegin(); it != vd.rend(); ++it) {
        n *= b;
        n += *it;
    }
    return n;
}

/**
 * Converts the vector of digits to a lowercase string.
 * Only bases up to 36 are supported.
 */
template<typename B>
std::string integer_string(const std::vector<B>& digits) {
    static const char* c = "0123456789abcdefghijklmnopqrstuvwxyz";
    std::string s;
    for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
        s += c[*it];
    }
    return s;
}

/**
 * Gives the string representation of `n` in base `b`.
 *
 * @param n - number to extract the digits of
 * @param b - base up to 36
 * @param len - zeros are added if necessary to fill the `len` digits
 */
template<typename I, typename B>
std::string integer_string(const I& n, const B& b, int len = 0) {
    return integer_string(integer_digits(n, b, len));
}

} // math
} // altruct
