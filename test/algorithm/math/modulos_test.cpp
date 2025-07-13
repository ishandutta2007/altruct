#include "altruct/algorithm/math/modulos.h"
#include "altruct/algorithm/math/ranges.h"
#include "altruct/algorithm/collections/collections.h"

#include "gtest/gtest.h"

#include <unordered_map>
#include <vector>

using namespace std;
using namespace altruct::math;

void crt_test_impl(int a1, int n1, int a2, int n2) {
    int a, n; chinese_remainder(&a, &n, a1, n1, a2, n2);
    EXPECT_EQ(lcm(n1, n2), n);
    EXPECT_EQ(a1, a % n1);
    EXPECT_EQ(a2, a % n2);
    EXPECT_GE(a, 0);
    EXPECT_LT(a, n);
}

TEST(modulos_test, chinese_remainder) {
    crt_test_impl(0, 10, 5, 13);
    crt_test_impl(5, 10, 3, 13);
    crt_test_impl(5, 10, 3, 14);
    crt_test_impl(4, 10, 6, 14);
    crt_test_impl(6, 14, 6, 14);
    crt_test_impl(102, 65535, 12345, 48888);
}

TEST(modulos_test, garner) {
    typedef moduloX<int> modx;
    typedef std::vector<modx> modv;

    modv a = { { 1000, 1009 }, { 1000, 1013 }, { 1000, 1019 } };
    modv a3 = altruct::collections::transform(a, [](const modx& e){ return powT(e, 3); });

    moduloX<int> r0{ 0, 1 };
    for (int i = 0; i < a3.size(); i++) {
        chinese_remainder<int>(&r0.v, &r0.M(), a3[i].v, a3[i].M());
    }
    EXPECT_EQ(1000000000, r0.v);
    EXPECT_EQ(1009 * 1013 * 1019, r0.M());

    modv x3 = garner(a3);
    moduloX<int> r1{ 0, 1 };
    for (int i = 0; i < x3.size(); i++) {
        r1.v += r1.M() * x3[i].v;
        r1.M() *= x3[i].M();
    }
    EXPECT_EQ(1000000000, r1.v);
    EXPECT_EQ(1009 * 1013 * 1019, r1.M());
}

TEST(modulos_test, jacobi) {
    vector<vector<int>> v(1);
    for (int m = 1; m <= 50; m++) {
        v.push_back(vector<int>());
        for (int n = 0; n <= 20; n++) {
            v[m].push_back(jacobi(n, m));
        }
    }
    EXPECT_EQ((vector<int>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}), v[1]);
    EXPECT_EQ((vector<int>{0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1}), v[3]);
    EXPECT_EQ((vector<int>{0, 1, -1, -1, 1, 0, 1, -1, -1, 1, 0, 1, -1, -1, 1, 0, 1, -1, -1, 1, 0}), v[5]);
    EXPECT_EQ((vector<int>{0, 1, 1, -1, 1, -1, -1, 0, 1, 1, -1, 1, -1, -1, 0, 1, 1, -1, 1, -1, -1}), v[7]);
    EXPECT_EQ((vector<int>{0, 1, -1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, -1, 1, 0, 1, -1, 0, 1, 0}), v[45]);
}

TEST(modulos_test, sqrt_cipolla) {
    EXPECT_EQ(0, sqrt_cipolla(0, 17));
    EXPECT_EQ(1, sqrt_cipolla(1, 17));
    EXPECT_EQ(6, sqrt_cipolla(2, 17));
    EXPECT_EQ(15, sqrt_cipolla(4, 17));
    EXPECT_EQ(12, sqrt_cipolla(8, 17));
    EXPECT_EQ(14, sqrt_cipolla(9, 17));
    EXPECT_EQ(8, sqrt_cipolla(13, 17));
    EXPECT_EQ(7, sqrt_cipolla(15, 17));
    EXPECT_EQ(13, sqrt_cipolla(16, 17));
    typedef modulo<int, 17> mod;
    EXPECT_EQ(mod(0), sqT(sqrt_cipolla(mod(0))));
    EXPECT_EQ(mod(1), sqT(sqrt_cipolla(mod(1))));
    EXPECT_EQ(mod(2), sqT(sqrt_cipolla(mod(2))));
    EXPECT_EQ(mod(4), sqT(sqrt_cipolla(mod(4))));
    EXPECT_EQ(mod(8), sqT(sqrt_cipolla(mod(8))));
    EXPECT_EQ(mod(9), sqT(sqrt_cipolla(mod(9))));
    EXPECT_EQ(mod(13), sqT(sqrt_cipolla(mod(13))));
    EXPECT_EQ(mod(15), sqT(sqrt_cipolla(mod(15))));
    EXPECT_EQ(mod(16), sqT(sqrt_cipolla(mod(16))));
}

TEST(modulos_test, sqrt_hensel_lift_p2) {
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(0, 0));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(0, 1));
    EXPECT_EQ(make_pair(1, 1), sqrt_hensel_lift_p2(1, 1));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(0, 2));
    EXPECT_EQ(make_pair(1, 1), sqrt_hensel_lift_p2(1, 2));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(2, 2));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(3, 2));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(0, 3));
    EXPECT_EQ(make_pair(1, 3), sqrt_hensel_lift_p2(1, 3));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(2, 3));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(3, 3));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(4, 3));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(5, 3));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(6, 3));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(7, 3));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(0, 4));
    EXPECT_EQ(make_pair(1, 7), sqrt_hensel_lift_p2(1, 4));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(2, 4));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(3, 4));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(4, 4));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(5, 4));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(6, 4));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(7, 4));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(8, 4));
    EXPECT_EQ(make_pair(5, 3), sqrt_hensel_lift_p2(9, 4));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(10, 4));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(11, 4));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(12, 4));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(13, 4));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(14, 4));
    EXPECT_EQ(make_pair(0, 0), sqrt_hensel_lift_p2(15, 4));
    EXPECT_EQ(make_pair(273, 1775), sqrt_hensel_lift_p2(801, 12));
}

TEST(modulos_test, sqrt_hensel_lift) {
    EXPECT_EQ(0, sqrt_hensel_lift(0, 17, 5));
    EXPECT_EQ(1, sqrt_hensel_lift(1, 17, 5));
    EXPECT_EQ(461199, sqrt_hensel_lift(2, 17, 5));
    EXPECT_EQ(1419855, sqrt_hensel_lift(4, 17, 5));
    EXPECT_EQ(922398, sqrt_hensel_lift(8, 17, 5));
    EXPECT_EQ(1419854, sqrt_hensel_lift(9, 17, 5));
    EXPECT_EQ(499740, sqrt_hensel_lift(13, 17, 5));
    EXPECT_EQ(1318629, sqrt_hensel_lift(15, 17, 5));
    EXPECT_EQ(1419853, sqrt_hensel_lift(16, 17, 5));
    EXPECT_EQ(883131, sqrt_hensel_lift(12346, 17, 5));
}

TEST(modulos_test, sqrt_mod) {
    prime_holder prim(100 + 1);
    for (int m = 1; m < prim.size(); m++) {
        unordered_map<int, vector<int>> mr;
        for (int x = 0; x < m; x++) {
            int y = (x * x) % m;
            mr[y].push_back(x);
        }
        for (int y = 1; y < m; y++) {
            if (gcd(m, y) != 1) continue;
            const auto& vr_expected = mr[y];
            auto vr_actual = sqrt_mod(y, prim.factor_integer(m));
            sort(vr_actual.begin(), vr_actual.end());
            EXPECT_EQ(vr_expected, vr_actual) << "ERROR: sqrt_mod(" << y << ", " << m << ")";
        }
    }
}

TEST(modulos_test, multiplicative_order) {
    prime_holder prim(100);
    for (int m = 2; m < prim.size(); m++) {
        int phi = prim.phi(m);
        auto phi_factors = prime_factors(prim.factor_integer(phi));
        for (int a = 1; a < m; a++) {
            if (gcd(a, m) != 1) continue;
            int o = multiplicative_order(a, m, phi, phi_factors);
            EXPECT_EQ(modulo_power(a, o, m), 1) << m << " " << a;
            for (int k = 1; k < o; k++) {
                EXPECT_NE(modulo_power(a, k, m), 1) << m << " " << a << " " << k;
            }
        }
    }
}

TEST(modulos_test, primitive_root) {
    prime_holder prim(100);
    EXPECT_EQ(1, primitive_root(2, 1, vector<int>{ }));
    EXPECT_EQ(2, primitive_root(3, 2, vector<int>{ 2 }));
    EXPECT_EQ(3, primitive_root(4, 2, vector<int>{ 2 }));
    EXPECT_EQ(2, primitive_root(5, 4, vector<int>{ 2 }));
    EXPECT_EQ(5, primitive_root(6, 2, vector<int>{ 2 }));
    EXPECT_EQ(3, primitive_root(7, 6, vector<int>{ 2, 3 }));
    EXPECT_EQ(0, primitive_root(8, 4, vector<int>{ 2 }));
    EXPECT_EQ(2, primitive_root(9, 6, vector<int>{ 2, 3 }));
    EXPECT_EQ(3, primitive_root(10, 4, vector<int>{ 2 }));
    EXPECT_EQ(2, primitive_root(11, 10, vector<int>{ 2, 5 }));
    EXPECT_EQ(5, primitive_root(18, 6, vector<int>{ 2, 3 }));
    EXPECT_EQ(0, primitive_root(120120, 23040, vector<int>{ 2, 3, 5 }));
    vector<int> vg;
    for (int m = 2; m <= 20; m++) {
        vg.push_back(primitive_root(m, prim));
    }
    EXPECT_EQ((vector<int>{1, 2, 3, 2, 5, 3, 0, 2, 3, 2, 0, 2, 3, 0, 0, 3, 5, 2, 0}), vg);
}

TEST(modulos_test, primitive_root_of_unity) {
    prime_holder prim(100);
    EXPECT_EQ(1, primitive_root_of_unity(2, 1, vector<int>{ }));
    EXPECT_EQ(2, primitive_root_of_unity(3, 2, vector<int>{ 2 }));
    EXPECT_EQ(3, primitive_root_of_unity(4, 2, vector<int>{ 2 }));
    EXPECT_EQ(2, primitive_root_of_unity(5, 4, vector<int>{ 2 }));
    EXPECT_EQ(5, primitive_root_of_unity(6, 2, vector<int>{ 2 }));
    EXPECT_EQ(3, primitive_root_of_unity(7, 6, vector<int>{ 2, 3 }));
    EXPECT_EQ(3, primitive_root_of_unity(8, 2, vector<int>{ 2 }));
    EXPECT_EQ(2, primitive_root_of_unity(9, 6, vector<int>{ 2, 3 }));
    EXPECT_EQ(3, primitive_root_of_unity(10, 4, vector<int>{ 2 }));
    EXPECT_EQ(2, primitive_root_of_unity(11, 10, vector<int>{ 2, 5 }));
    EXPECT_EQ(5, primitive_root_of_unity(18, 6, vector<int>{ 2, 3 }));
    EXPECT_EQ(17, primitive_root_of_unity(120120, 60, vector<int>{ 2, 3, 5 }));
    vector<int> vg;
    for (int m = 2; m <= 20; m++) {
        vg.push_back(primitive_root_of_unity(m, prim));
    }
    EXPECT_EQ((vector<int>{1, 2, 3, 2, 5, 3, 3, 2, 3, 2, 5, 2, 3, 2, 3, 3, 5, 2, 3}), vg);
}

TEST(modulos_test, kth_roots_of_unity) {
    prime_holder prim(100);
    EXPECT_EQ((set<int>{ 1, 4, 13, 16 }), kth_roots_of_unity(17, 4, 16, primitive_root_of_unity(17, 16, vector<int>{2})));
    EXPECT_EQ((set<int>{1, 17}), kth_roots_of_unity(18, 4, 6, primitive_root_of_unity(18, 6, vector<int>{2, 3})));
    EXPECT_EQ((set<int>{1, 7, 13}), kth_roots_of_unity(18, 3, 6, primitive_root_of_unity(18, 6, vector<int>{2, 3})));
    EXPECT_EQ((set<int>{1, 4, 13, 16}), kth_roots_of_unity(17, 4, prim));
    EXPECT_EQ((set<int>{1, 7, 13}), kth_roots_of_unity(18, 3, prim));
    EXPECT_EQ((set<int>{1, 17}), kth_roots_of_unity(18, 4, prim));
    EXPECT_EQ((set<int>{1}), kth_roots_of_unity(18, 5, prim));
    EXPECT_EQ((set<int>{1, 5, 7, 11, 13, 17}), kth_roots_of_unity(18, 6, prim));
}

TEST(modulos_test, discrete_log_brute_force) {
    for (int m = 2; m < 100; m++) {
        for (int a = 1; a < m; a++) {
            int a_x = 1;
            for (int x = 0; x < m; x++) {
                int xx = discrete_log_brute_force(a, a_x, m);
                EXPECT_EQ(modulo_power(a, xx, m), a_x) << m << " " << a << " " << x;
                a_x = modulo_mul(a_x, a, m);
            }
        }
    }
}

TEST(modulos_test, discrete_log_baby_giant) {
    for (int m = 2; m < 100; m++) {
        for (int a = 1; a < m; a++) {
            if (gcd(a, m) != 1) continue;
            int n = 1;
            for (int a_n = a; a_n != 1; a_n = modulo_mul(a_n, a, m)) {
                n++;
            }
            int a_x = 1;
            for (int x = 0; x < n; x++) {
                int xx = discrete_log_baby_giant(a, a_x, m, n);
                EXPECT_EQ(modulo_power(a, xx, m), a_x) << m << " " << a << " " << x;
                a_x = modulo_mul(a_x, a, m);
            }
        }
    }
}

TEST(modulos_test, discrete_log_shanks) {
    prime_holder prim(300);
    for (int p : prim.p()) {
        for (int a = 1; a < p; a++) {
            if (a % p == 0) continue;
            int a_x = 1;
            for (int x = 0; x == 0 || a_x != 1; x++) {
                int xx = discrete_log_shanks(a, a_x, p);
                EXPECT_EQ(modulo_power(a, xx, p), a_x) << p << " " << a << " " << x;
                a_x = modulo_mul(a_x, a, p);
            }
        }
    }
}

TEST(modulos_test, discrete_log_pp) {
    int N = 300;
    prime_holder prim(N);
    for (int p : prim.p()) {
        int ps = p;
        for (int s = 1; ps < N; ps *= p, s++) {
            for (int a = 1; a < ps; a++) {
                if (a % p == 0) continue;
                int a_x = 1;
                for (int x = 0; x == 0 || a_x != 1; x++) {
                    int xx = discrete_log_pp(a, a_x, p, s);
                    EXPECT_EQ(modulo_power(a, xx, ps), a_x) << p << " " << s << " " << a << " " << x;
                    a_x = modulo_mul(a_x, a, ps);
                }
            }
        }
    }
}

TEST(modulos_test, discrete_log) {
    int N = 300;
    prime_holder prim(N);
    for (int m = 2; m < N; m++) {
        int phi = prim.phi(m);
        auto phi_factors = prime_factors(prim.factor_integer(phi));
        for (int a = 1; a < m; a++) {
            if (gcd(a, m) != 1) continue;
            int a_x = 1;
            for (int x = 0; x == 0 || a_x != 1; x++) {
                uint64_t xx = discrete_log(a, a_x, m, phi, phi_factors);
                EXPECT_EQ(modulo_power(a, xx, m), a_x) << m << " " << a << " " << x;
                a_x = modulo_mul(a_x, a, m);
            }
        }
    }
}
