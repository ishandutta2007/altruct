#pragma once

#include <cstdint>
#include "random.h"

namespace altruct {
namespace random {

/**
 * A 64 bit variant of the xorshift* pseudo-random number generator family.
 */
class xorshift_64star {
public:
    /**
     * UniformRandomBitGenerator requirement
     */
    using result_type = uint64_t;
    static constexpr uint64_t min() noexcept { return std::numeric_limits<uint64_t>::min(); }
    static constexpr uint64_t max() noexcept { return std::numeric_limits<uint64_t>::max(); }
    uint64_t operator()() { return next(); }

    /**
     * Constructs a new instance of this class.
     * The state must be initialized by calling {@code seed} before consuming
     * the values produced by {@code next}.
     */
    xorshift_64star() { seed(uint64_t(1)); }

    /**
     * Constructs a new instance of this class seeded with the provided state.
     * The values produced by {@code next} are ready to be consumed.
     */
    explicit xorshift_64star(uint64_t state) { seed(state); }

    /**
     * Seeds this instance with the provided 64bit state.
     */
    void seed(uint64_t state) { x_ = state; }

    /**
     * Gets the next random number.
     */
    uint64_t next() {
        // The constants used in this implementation are as suggested by the author.
        x_ ^= x_ >> 12; // a
        x_ ^= x_ << 25; // b
        x_ ^= x_ >> 27; // c
        return x_ * 2685821657736338717ULL;
    }

    /**
     * Gets the next random number in [min, max] range, both inclusive.
     */
    uint64_t next(uint64_t min, uint64_t max) {
        return integer_to_range<uint64_t>(next(), min, max);
    }

    /*
     * Gets the next random number in range [min, max] inclusive, with stronger
     * uniformity guarantees at expense of decreased performance.
     * In most cases {@code next(min, max)} will suffice.
     */
    uint64_t next_uniform(uint64_t min, uint64_t max) {
        return uniform_next<uint64_t>([&]() { return next(); }, min, max);
    }

    /**
     * Gets the next random number as a double in [0, 1] range, both inclusive.
     */
    double next_0_1() {
        return integer_to_double_0_1<uint64_t>(next());
    }

private:
    /* The state. Must be seeded with a nonzero value. */
    uint64_t x_;
};

/**
 * This is an adaptation of the public domain xorshift1024star generator
 * from http://xorshift.di.unimi.it/xorshift1024star.c
 */
class xorshift_1024star {
public:
    /**
     * UniformRandomBitGenerator requirement
     */
    using result_type = uint64_t;
    static constexpr uint64_t min() noexcept { return std::numeric_limits<uint64_t>::min(); }
    static constexpr uint64_t max() noexcept { return std::numeric_limits<uint64_t>::max(); }
    uint64_t operator()() { return next(); }

    /**
     * Constructs a new instance of this class.
     * The state must be initialized by calling {@code seed} before consuming
     * the values produced by {@code next}.
     */
    xorshift_1024star() { seed(uint64_t(1)); }

    /**
     * Constructs a new instance of this class seeded with the provided state.
     * The values produced by {@code next} are ready to be consumed.
     */
    explicit xorshift_1024star(uint64_t state) { seed(state); }

    /**
     * Constructs a new instance of this class seeded with the provided state.
     * The provided state should consist of <b>exactly</b> 16 uint64_t values.
     * The values produced by {@code next} are ready to be consumed.
     */
    explicit xorshift_1024star(uint64_t* state) { seed(state); }

    /**
     * Seeds this instance with the provided 64bit state.
     * The provided 64bit state will be used as a seed for xorshift_64star
     * generator which will then be used to generate the 16 uint64_t values
     * for the new state of this generator.
     */
    void seed(uint64_t state64) {
        xorshift_64star xs64(state64);
        for (int i = 0; i < 16; i++) {
            s_[i] = xs64.next();
        }
        p_ = 0;
    }

    /**
     * Seeds this instance with the provided state.
     * The provided state should consist of <b>exactly</b> 16 uint64_t values.
     */
    void seed(uint64_t* state) {
        for (int i = 0; i < 16; i++) {
            s_[i] = state[i];
        }
        p_ = 0;
    }

    /**
     * Gets the next random number.
     */
    uint64_t next() {
        // The constants used in this implementation are as suggested by the author.
        uint64_t s0 = s_[p_];
        p_ = (p_ + 1) & 15;
        uint64_t s1 = s_[p_];
        s1 ^= s1 << 31; // a
        s1 ^= s1 >> 11; // b
        s0 ^= s0 >> 30; // c
        s_[p_] = s0 ^ s1;
        return s_[p_] * 1181783497276652981ULL;
    }

    /**
     * Gets the next random number in [min, max] range, both inclusive.
     */
    uint64_t next(uint64_t min, uint64_t max) {
        return integer_to_range<uint64_t>(next(), min, max);
    }

    /*
     * Gets the next random number in range [min, max] inclusive, with stronger
     * uniformity guarantees at expense of decreased performance.
     * In most cases {@code next(min, max)} will suffice.
     */
    uint64_t next_uniform(uint64_t min, uint64_t max) {
        return uniform_next<uint64_t>([&]() { return next(); }, min, max);
    }

    /**
     * Gets the next random number as a double in [0, 1] range, both inclusive.
     */
    double next_0_1() {
        return integer_to_double_0_1<uint64_t>(next());
    }

private:
    /**
     * The state must be seeded so that it is not everywhere zero. If you have
     * a 64-bit seed, we suggest to seed a xorshift64* generator and use its
     * output to fill the state s.
     */
    uint64_t s_[16];
    int p_;
};

} // random
} // altruct
