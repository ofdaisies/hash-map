#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stack>
#include <functional>
#include <set>
#include <queue>
#include <string>
#include <map>
#include <iomanip>
#include <sstream>
#include <cassert>
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "buf.hpp"
using namespace fefu;
TEST_CASE( "max_load_factor()" ) {
    int k = 3;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    std::pair<int, int> d = {2, 9};
    auto v = a.insert(c);
    v = a.insert(d);
    a.max_load_factor(0.5);
    REQUIRE(a.bucket_count() == 4);
    REQUIRE((a.max_load_factor() - 0.5) < 0.0000001);
}
TEST_CASE( "max_load_factor()1" ) {
    int k = 3;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    std::pair<int, int> d = {2, 9};
    auto v = a.insert(c);
    v = a.insert(d);
    a.max_load_factor(0.8);
    REQUIRE(a.bucket_count() == 3);
    REQUIRE((a.max_load_factor() - 0.8) < 0.0000001);
}
TEST_CASE( "max_load_factor" ) {
    int k = 5;
    hash_map<int, int> a(k);
    REQUIRE(abs(a.max_load_factor() - 0.75) < 0.0000001);
}
TEST_CASE( "bucket_count" ) {
    int k = 5;
    hash_map<int, int> a(k);
    REQUIRE(a.bucket_count() == 5);
}
TEST_CASE( "rehash" ) {
    int k = 5;
    hash_map<int, int> a(k);
    int b = 10;
    a.rehash(b);
    REQUIRE(a.bucket_count() == 10);
}
TEST_CASE( "reserve" ) {
    int k = 10;
    hash_map<int, int> a(k);
    int b = 8;
    a.reserve(b);
    REQUIRE(a.bucket_count() == 11);
}

TEST_CASE( "insert1" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    auto v = a.insert(c);
    REQUIRE(a[1] == 1);
}
TEST_CASE( "insert2" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    auto v = a.insert({1, 1});
    REQUIRE(a[1] == 1);
}
TEST_CASE( "load factor" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    auto v = a.insert(c);
    REQUIRE(abs(a.load_factor() - 0.2) < 0.000001);
}
TEST_CASE( "bucket" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    auto v = a.insert(c);
    auto d = a.bucket(1);
    REQUIRE(d == 1);
}
TEST_CASE( "[]&" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    auto v = a.insert(c);
    int y = 1;
    auto x = a[y];
    REQUIRE(x == 1);
}
TEST_CASE( "[]&&" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    auto v = a.insert(c);
    auto x = a[1];
    REQUIRE(x == 1);
}
TEST_CASE( "find1" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    auto v = a.insert(c);
    int key_ = 1;
    auto v1 = a.find(key_);
    REQUIRE(v1 == v.first);
}
TEST_CASE( "find1 false" ) {
    int k = 5;
    hash_map<int, int> a(k);
    int key_ = 1;
    auto v1 = a.find(key_);
    REQUIRE(v1 == a.end());
}
TEST_CASE( "count and contain" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    auto v = a.insert(c);
    int key_ = 1;
    int key1 = 2;
    int v1 = a.count(key_);
    bool v2 = a.contains(key1);
    REQUIRE(v1);
    REQUIRE(!v2);
}
TEST_CASE( "erase1" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    auto v = a.insert(c);
    int m = a.erase(c.first);
    REQUIRE(m == 1);
}
TEST_CASE( "erase1 0" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    auto v = a.insert(c);
    int b = 2;
    int m = a.erase(b);
    REQUIRE(m == 0);
}
TEST_CASE( "erase2" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    auto v = a.insert(c);
    auto m = a.erase(a.begin());
    REQUIRE(m == a.end());
}
TEST_CASE( "erase3" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    auto v = a.insert(c);
    auto m = a.erase(a.cbegin());
    k = 1;
    REQUIRE(m == a.end());
}
TEST_CASE( "at" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    auto v = a.insert(c);
    k = 1;
    a.at(k) = 2;
    REQUIRE(a.at(k) == 2);
}
TEST_CASE( "const at" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    auto v = a.insert(c);
    k = 1;
    const int n = a.at(k);
    REQUIRE(n == 1);
}
TEST_CASE( "emplace" ) {
    int k = 5;
    hash_map<int, int> a(k);
    auto v = a.emplace(1, 1);
    REQUIRE(a[1] == 1);
}
TEST_CASE( "try emplace" ) {
    int k = 5;
    hash_map<int, int> a(k);
    auto v = a.emplace(1, 1);
    v = a.try_emplace(1, 1);
    k = 2;
    auto v1 = a.try_emplace(k, 5);
    REQUIRE(!v.second);
    REQUIRE(a.at(k) == 5);
}
TEST_CASE( "max size" ) {
    int k = 5;
    hash_map<int, int> a(k);
    REQUIRE(a.max_size() == std::numeric_limits<std::size_t>::max());
}
TEST_CASE( "empty" ) {
    int k = 5;
    hash_map<int, int> a(k);
    REQUIRE(a.empty());
}
TEST_CASE( "constructor 1" ) {
    int k = 5;
    hash_map<int, int> a;
    std::pair<int, int> c = {1, 1};
    a.insert(c);
    REQUIRE(a.bucket_count() == 2);
}
TEST_CASE( "insert l" ) {
    int k = 5;
    hash_map<int, int> a;
    std::initializer_list<std::pair<const int, int>> l{{1, 2}, {3, 4}};
    a.insert(l);
    REQUIRE(a.at(1) == 2);
    REQUIRE(a.at(3) == 4);
}
TEST_CASE( "insert input iterator" ) {
    int k = 5;
    hash_map<int, int> a;
    std::initializer_list<std::pair<const int, int>> l{{1, 2}, {3, 4}};
    a.insert(l.begin(), l.end());
    REQUIRE(a.at(1) == 2);
    REQUIRE(a.at(3) == 4);
}
TEST_CASE( "constructor2" ) {
    std::initializer_list<std::pair<const int, int>> l{{1, 2}, {3, 4}};
    hash_map<int, int> a(l);
    REQUIRE(a.at(1) == 2);
    REQUIRE(a.at(3) == 4);
}
TEST_CASE( "constructor3" ) {
    std::initializer_list<std::pair<const int, int>> l{{1, 2}, {3, 4}};
    hash_map<int, int> a(l.begin(), l.end());
    REQUIRE(a.at(1) == 2);
    REQUIRE(a.at(3) == 4);
    REQUIRE(a.size() == 2);
}
TEST_CASE( "== true" ) {
    int k = 5;
    hash_map<int, int> a(k);
    hash_map<int, int> b(k);
    std::pair<int, int> c = {1, 1};
    a.insert(c);
    b.insert(c);
    REQUIRE(a == b);
}
TEST_CASE( "== false1" ) {
    int k = 5;
    hash_map<int, int> a(k);
    hash_map<int, int> b(k);
    std::pair<int, int> c = {1, 1};
    std::pair<int, int> d = {1, 2};
    a.insert(c);
    b.insert(d);
    REQUIRE(!(a == b));
}
TEST_CASE( "== false2" ) {
    int k = 5;
    int k1 = 3;
    hash_map<int, int> a(k);
    hash_map<int, int> b(k1);
    std::pair<int, int> d = {1, 2};
    a.insert(d);
    REQUIRE(!(a == b));
}
TEST_CASE( "constructor4" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    std::pair<int, int> d = {1, 2};
    a.insert(c);
    a.insert(d);
    hash_map<int, int> b(a);
    REQUIRE(a == b);
}
TEST_CASE( "constructor5" ) {
    int k = 5;
    hash_map<int, int> a(hash_map<int, int> (5));
    REQUIRE(a.bucket_count() == 5);
    REQUIRE(a.size() == 0);
}
TEST_CASE( "=" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    std::pair<int, int> d = {1, 2};
    a.insert(c);
    a.insert(d);
    hash_map<int, int> b;
    b = a;
    REQUIRE(a == b);
}
TEST_CASE( "=l" ) {
    int k = 5;
    hash_map<int, int> a;
    std::initializer_list<std::pair<const int, int>> l{{1, 2}, {3, 4}};
    hash_map<int, int> b(l);
    a.insert(l);
    REQUIRE(a == b);
}
TEST_CASE( "clear" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    a.insert(c);
    a.clear();
    hash_map<int, int> b(k);
    REQUIRE(a == b);
}
TEST_CASE( "insert or assign&&" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    a.insert(c);
    a.insert_or_assign(1, 2);
    REQUIRE(a.at(c.first) == 2);
}
TEST_CASE( "insert or assign" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    a.insert(c);
    k = 1;
    a.insert_or_assign(k, 2);
    REQUIRE(a.at(c.first) == 2);
}
TEST_CASE( "insert of erased" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    a.insert(c);
    a.erase(c.first);
    a.insert(c);
    REQUIRE(a.at(c.first) == 1);
}
TEST_CASE( "const find" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    auto i(a.find(c.first));
    REQUIRE(i == a.cend());
}
TEST_CASE( "merge" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    std::pair<int, int> d = {2, 2};
    a.insert(c);
    a.insert(d);
    hash_map<int, int> b(k);
    b.merge(a);
    REQUIRE(b.at(c.first) == 1);
    REQUIRE(b.at(d.first) == 2);
}
TEST_CASE( "at throw" ) {
    int k = 5;
    hash_map<int, int> a(k);
    std::pair<int, int> c = {1, 1};
    REQUIRE_THROWS_AS(a.at(c.first), std::out_of_range);
}