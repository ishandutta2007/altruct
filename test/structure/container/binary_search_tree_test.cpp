#include "structure/container/binary_search_tree.h"

#include "algorithm/collections/collections.h"
#include "io/iostream_overloads.h"
#include "structure_test_util.h"

#include <functional>

#include "gtest/gtest.h"

using namespace std;
using namespace altruct::container;
using namespace altruct::collections;
using namespace altruct::test_util;

namespace {
    template<typename K, typename T = K, int DUP = bst_duplicate_handling::IGNORE, typename CMP = std::less<K>, typename ALLOC = allocator<bst_node<T>>>
    class binary_search_tree_dbg : public binary_search_tree<K, T, DUP, CMP, ALLOC> {
    public:
        binary_search_tree_dbg(const CMP& cmp = CMP(), const ALLOC& alloc = ALLOC()) :
            binary_search_tree(cmp, alloc) {
        }

        template<typename It>
        binary_search_tree_dbg(It begin, It end, const CMP& cmp = CMP(), const ALLOC& alloc = ALLOC()) : 
            binary_search_tree(begin, end, cmp, alloc) {
        }

        binary_search_tree_dbg(std::initializer_list<T> list) :
            binary_search_tree(list) {}

        binary_search_tree_dbg(binary_search_tree_dbg&& rhs) :
            binary_search_tree(std::move(rhs)) {
        }

        binary_search_tree_dbg(const binary_search_tree_dbg& rhs) :
            binary_search_tree(rhs) {
        }

        binary_search_tree_dbg& operator=(binary_search_tree_dbg&& rhs) {
            binary_search_tree::operator=(std::move(rhs));
            return *this;
        }

        binary_search_tree_dbg& operator=(const binary_search_tree_dbg& rhs) {
            binary_search_tree::operator=(rhs);
            return *this;
        }

        void debug_check(const_node_ptr ptr = nullptr) const {
            if (ptr == nullptr) {
                ptr = root();
                ASSERT_TRUE(nil->parent == nil) << "ERROR: nil not connected back to itself";
                ASSERT_TRUE(nil->left == nil->right) << "ERROR: nil left & right roots out of sync";
            }
            if (ptr == nil) {
                return;
            }
            if (!ptr->left->is_nil()) {
                ASSERT_FALSE(cmp(_key(ptr->val), _key(ptr->left->val))) << "ERROR: parent < left";
                ASSERT_FALSE(ptr->left->parent != ptr) << "ERROR: left not connected back to parent";
                debug_check(ptr->left);
            }
            if (!ptr->right->is_nil()) {
                ASSERT_FALSE(cmp(_key(ptr->right->val), _key(ptr->val))) << "ERROR: right < parent";
                ASSERT_FALSE(ptr->right->parent != ptr) << "ERROR: right not connected back to parent";
                debug_check(ptr->right);
            }
        }

        static void make_link(node_ptr par, node_ptr ch, bool go_left) {
            binary_search_tree::make_link(par, ch, go_left);
        }
    };

    // Note: relies on iterator functioning properly
    template<typename K, typename T, int DUP, typename CMP, typename ALLOC, typename COLLECTION>
    void verify_structure(const binary_search_tree_dbg<K, T, DUP, CMP, ALLOC>& t, const COLLECTION& c) {
        t.debug_check();
        vector<T> va;
        for (auto it = t.cbegin(); it != t.cend(); ++it) {
            for (int i = 0; i < it.count(); i++) {
                va.push_back(*it);
            }
        }
        EXPECT_EQ(vector<T>(c.begin(), c.end()), va);
        EXPECT_EQ(c.size(), t.size());
        EXPECT_EQ(c.empty(), t.empty());
    }

    template<typename T>
    bst_node<T> new_node(const T& val, bst_node<T>* nil) {
        bst_node<T> t(val);
        t.parent = nil;
        t.left = nil;
        t.right = nil;
        t.balance = 0;
        t.size = 0;
        return t;
    }
}

TEST(binary_search_tree_test, bst_node) {
    bst_node<int> nodes[4] {10, 20, 30, 40};
    (nodes + 0)->parent = (nodes + 0);
    EXPECT_TRUE((nodes + 0)->is_nil());
    (nodes + 1)->parent = (nodes + 0);
    EXPECT_FALSE((nodes + 1)->is_nil());

    (nodes + 1)->size = 25;
    (nodes + 1)->left = (nodes + 2);
    (nodes + 2)->size = 6;
    (nodes + 1)->right = (nodes + 3);
    (nodes + 3)->size = 8;
    EXPECT_EQ(11, (nodes + 1)->count());
}

TEST(binary_search_tree_test, bst_inorder) {
    typedef binary_search_tree_dbg<int> bst;
    bst_node<int> nil(-1); nil.parent = &nil;
    vector<bst_node<int>> nodes;
    for (int i = 0; i < 12; i++) {
        nodes.push_back(new_node(i, &nil));
    }
    bst::make_link(&nil, &nodes[5], true);
    bst::make_link(&nodes[5], &nodes[1], true);
    bst::make_link(&nodes[1], &nodes[0], true);
    bst::make_link(&nodes[1], &nodes[4], false);
    bst::make_link(&nodes[4], &nodes[3], true);
    bst::make_link(&nodes[3], &nodes[2], true);
    bst::make_link(&nodes[5], &nodes[9], false);
    bst::make_link(&nodes[9], &nodes[6], true);
    bst::make_link(&nodes[9], &nodes[11], false);
    bst::make_link(&nodes[6], &nodes[7], false);
    bst::make_link(&nodes[7], &nodes[8], false);
    bst::make_link(&nodes[11], &nodes[10], true);
    bst_node<int>* tmp = &nil;
    for (int i = -1; i < 12; i++) {
        EXPECT_EQ(i, tmp->val);
        tmp = bst_iterator_util<int>::inorder_next(tmp);
    }
    for (int i = 11; i >= -1; i--) {
        tmp = bst_iterator_util<int>::inorder_prev(tmp);
        EXPECT_EQ(i, tmp->val);
    }
    bst_iterator<int> it(tmp);
    for (int i = -1; i < 12; i++) {
        EXPECT_EQ(i, *it);
        ++it;
    }
    for (int i = 11; i >= -1; i--) {
        --it;
        EXPECT_EQ(i, *it);
    }
    for (int i = -1; i < 12; i++) {
        EXPECT_EQ(i, *it);
        it++;
    }
    for (int i = 11; i >= -1; i--) {
        it--;
        EXPECT_EQ(i, *it);
    }
    bst_const_iterator<int> cit(tmp);
    for (int i = -1; i < 12; i++) {
        EXPECT_EQ(i, *cit);
        ++cit;
    }
    for (int i = 11; i >= -1; i--) {
        --cit;
        EXPECT_EQ(i, *cit);
    }
    for (int i = -1; i < 12; i++) {
        EXPECT_EQ(i, *cit);
        cit++;
    }
    for (int i = 11; i >= -1; i--) {
        cit--;
        EXPECT_EQ(i, *cit);
    }
}

TEST(binary_search_tree_test, bst_iterator) {
    typedef pair<const int, string> entry;
    entry e{ 42, "abc" };
    bst_node<entry> t(e), r({}), s({});
    t.size = 25;
    t.left = &r;
    r.size = 6;
    t.right = &s;
    s.size = 8;

    bst_iterator<entry> it(&t);
    EXPECT_EQ(e, *it);
    EXPECT_EQ(e.first, it->first);
    EXPECT_EQ(e.second, it->second);
    EXPECT_TRUE(it == &t);
    EXPECT_FALSE(it == &s);
    EXPECT_EQ(11, it.count());

    bst_const_iterator<entry> cit(&t);
    EXPECT_EQ(e, *cit);
    EXPECT_EQ(e.first, cit->first);
    EXPECT_EQ(e.second, cit->second);
    EXPECT_TRUE(cit == &t);
    EXPECT_FALSE(cit == &s);
    EXPECT_TRUE(cit == it);
    EXPECT_EQ(11, cit.count());
}

TEST(binary_search_tree_test, bst_key) {
    typedef pair<const int, string> entry;
    EXPECT_EQ("abc", (bst_key<string, string>::of("abc")));
    EXPECT_EQ(42, (bst_key<int, entry>::of({ 42, "def" })));
}

TEST(binary_search_tree_test, constructor) {
    // default
    set<int> s0;
    binary_search_tree_dbg<int> t0;
    verify_structure(t0, s0);

    // range
    set<int> s1; for (int i = 0; i < 100; i++) s1.insert(rand() % 10);
    binary_search_tree_dbg<int> t1(s1.begin(), s1.end());
    verify_structure(t1, s1);

    // range + comparator
    set<int, std::greater<int>> s2; for (int i = 0; i < 110; i++) s2.insert(rand() % 1000000000);
    binary_search_tree_dbg<int, int, bst_duplicate_handling::IGNORE, std::greater<int>> t2(s2.begin(), s2.end(), std::greater<int>());
    verify_structure(t2, s2);

    // initializer list
    binary_search_tree_dbg<int> ti{ 42, 3, 15 };
    verify_structure(ti, (set<int>{42, 3, 15}));

    // move constructor
    binary_search_tree_dbg<int> t3(std::move(binary_search_tree_dbg<int>(s1.begin(), s1.end())));
    verify_structure(t3, s1);
    
    // copy constructor
    binary_search_tree_dbg<int> t4(t3);
    verify_structure(t3, s1);
    verify_structure(t4, s1);

    // move assignment
    t4 = std::move(binary_search_tree_dbg<int>(s1.begin(), s1.end()));
    verify_structure(t4, s1);
    
    // copy assignment
    t4 = t3;
    verify_structure(t4, s1);
    verify_structure(t3, s1);
    
    // clear
    t1.clear();
    verify_structure(t1, s0);
    // use after clear
    t1.insert(12), t1.insert(8), t1.insert(4);
    verify_structure(t1, (set<int>{12, 8, 4}));
}

TEST(binary_search_tree_test, swap) {
    set<int> s1; for (int i = 0; i < 100; i++) s1.insert(rand() % 1000000000);
    set<int> s2; for (int i = 0; i < 110; i++) s2.insert(rand() % 1000000000);
    binary_search_tree_dbg<int> t1(s1.begin(), s1.end());
    binary_search_tree_dbg<int> t2(s2.begin(), s2.end());
    verify_structure(t1, s1);
    verify_structure(t2, s2);
    t1.swap(t2);
    verify_structure(t2, s1);
    verify_structure(t1, s2);
    std::swap(t2, t1);
    verify_structure(t1, s1);
    verify_structure(t2, s2);
}

TEST(binary_search_tree_test, duplicate_handling) {
    set<int> s1; for (int i = 0; i < 110; i++) s1.insert(rand() % 1000000000);
    binary_search_tree_dbg<int, int, bst_duplicate_handling::IGNORE> t1(s1.begin(), s1.end());
    verify_structure(t1, s1);

    multiset<int> s2; for (int i = 0; i < 110; i++) s2.insert(rand() % 1000000000);
    binary_search_tree_dbg<int, int, bst_duplicate_handling::COUNT> t2(s2.begin(), s2.end());
    verify_structure(t2, s2);
    
    typedef pair<const int, string> entry;
    multimap<int, string> s3; for (int i = 0; i < 110; i++) s3.insert({ rand() % 10, to_string(i) });
    binary_search_tree_dbg<int, entry, bst_duplicate_handling::STORE> t3(s3.begin(), s3.end());
    verify_structure(t3, s3);
}

TEST(binary_search_tree_test, iterators) {
    set<int> s1; for (int i = 0; i < 110; i++) s1.insert(rand() % 1000000000);
    binary_search_tree_dbg<int> t1(s1.begin(), s1.end());
    EXPECT_EQ((vector<int>(s1.begin(), s1.end())), (vector<int>(t1.begin(), t1.end())));
    EXPECT_EQ((vector<int>(s1.cbegin(), s1.cend())), (vector<int>(t1.cbegin(), t1.cend())));
    EXPECT_EQ((vector<int>(s1.rbegin(), s1.rend())), (vector<int>(t1.rbegin(), t1.rend())));
    EXPECT_EQ((vector<int>(s1.crbegin(), s1.crend())), (vector<int>(t1.crbegin(), t1.crend())));
}

TEST(binary_search_tree_test, relational_operators) {
    binary_search_tree_dbg<int> t{ 3, 8, 15, 16 };
    ASSERT_COMPARISON_OPERATORS(+1, t, (binary_search_tree_dbg<int>{ }));                  // empty
    ASSERT_COMPARISON_OPERATORS(0, t, (binary_search_tree_dbg<int>{ 3, 8, 15, 16 }));      // equal
    ASSERT_COMPARISON_OPERATORS(+1, t, (binary_search_tree_dbg<int>{ 3, 8, 15 }));         // shorter
    ASSERT_COMPARISON_OPERATORS(-1, t, (binary_search_tree_dbg<int>{ 3, 8, 15, 16, 17 })); // longer
    ASSERT_COMPARISON_OPERATORS(-1, t, (binary_search_tree_dbg<int>{ 3, 9, 15 }));         // shorter but larger
    ASSERT_COMPARISON_OPERATORS(+1, t, (binary_search_tree_dbg<int>{ 3, 7, 15, 16, 17 })); // longer but smaller
}

TEST(binary_search_tree_test, query) {
    set<int> s;
    s.swap(s);
}
