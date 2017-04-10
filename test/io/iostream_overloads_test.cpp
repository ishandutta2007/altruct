#include "io/iostream_overloads.h"

#include "gtest/gtest.h"

#include <string>
#include <sstream>
#include <functional>

using namespace std;
using namespace altruct::io;

namespace {
template<typename T>
std::string to_str(const T& val) {
    std::stringstream ss;
    ss << val;
    return ss.str();
}
}

TEST(iostream_overloads_test, pair) {
    EXPECT_EQ("{42, 3}", to_str(std::pair<int, int>(42, 3)));
}

TEST(iostream_overloads_test, vector) {
    EXPECT_EQ("{}", to_str(std::vector<int>{}));
    EXPECT_EQ("{42, 3, 15}", to_str(std::vector<int>{42, 3, 15}));
}

TEST(iostream_overloads_test, set) {
    EXPECT_EQ("{}", to_str(std::set<int>{}));
    EXPECT_EQ("{3, 15, 42}", to_str(std::set<int>{42, 3, 15}));
}

TEST(iostream_overloads_test, map) {
    EXPECT_EQ("{}", to_str(std::map<int, int>{}));
    EXPECT_EQ("{{3, 7}, {15, 8}, {42, -1}}", to_str(std::map<int, int>{{ 42, -1 }, { 3, 7 }, { 15, 8 }}));
}

TEST(iostream_overloads_test, nested_map_of_vector_to_set) {
    EXPECT_EQ("{}", to_str(std::map<std::vector<int>, std::set<int, std::greater<int>>>{}));
    auto nested = std::map<std::vector<int>, std::set<int, std::greater<int>>>();
    nested.insert({ { 42, 1, 12 }, { -1, 5, 3 } });
    nested.insert({ { 3 }, { 7, 11 } });
    nested.insert({ { 15, -2 }, { 8, 10, 9 } });
    EXPECT_EQ("{{{3}, {11, 7}}, {{15, -2}, {10, 9, 8}}, {{42, 1, 12}, {5, 3, -1}}}", to_str(nested));
}

TEST(iostream_overloads_test, fraction) {
    EXPECT_EQ("5/1", to_str(altruct::math::fraction<int>(5)));
    EXPECT_EQ("2/3", to_str(altruct::math::fraction<int>(2, 3)));
    EXPECT_EQ("3/2", to_str(altruct::math::fraction<int>(6, 4)));
}

TEST(iostream_overloads_test, modulo) {
    typedef altruct::math::modulo<int, 1007, altruct::math::modulo_storage::CONSTANT> mod;
    typedef altruct::math::moduloX<int> modx;
    EXPECT_EQ("123", to_str(mod(123)));
    EXPECT_EQ("123", to_str(modx(123, 1007)));
    EXPECT_EQ("{42, 3, 15}", to_str(std::vector<mod>{42, 3, 15}));
}

TEST(iostream_overloads_test, polynom) {
    typedef altruct::math::modulo<int, 1007, altruct::math::modulo_storage::CONSTANT> mod;
    EXPECT_EQ("{0}", to_str(altruct::math::polynom<int>{}));
    EXPECT_EQ("{42, 3, 15}", to_str(altruct::math::polynom<int>{42, 3, 15}));
    EXPECT_EQ("{42, 3, 15}", to_str(altruct::math::polynom<mod>{42, 3, 15}));
}
