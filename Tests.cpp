//
// Created by aleks311001 on 26.09.2020.
//

#include <gtest/gtest.h>
#include "NKA.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(testIsomorphysm, test1) {
    NKA first(0, {'a', 'b'}, 4, {3});
    NKA second(0, {'a', 'b'}, 4, {2});

    first.addTransition(0, "a", 1);
    first.addTransition(1, "ba", 1);
    first.addTransition(1, "b", 2);
    first.addTransition(0, "b", 2);
    first.addTransition(2, "b", 3);
    first.addTransition(2, EPS, 3);
    first.addTransition(3, "b", 2);

    second.addTransition(0, "a", 1);
    second.addTransition(1, "b", 3);
    second.addTransition(3, "a", 1);
    second.addTransition(1, "b", 2);
    second.addTransition(0, "b", 2);
    second.addTransition(2, "b", 2);

    ASSERT_TRUE(second == first);
}

TEST(testIsomorphysm, test2) {
    NKA first(0, {'a', 'b'}, 2, {1});
    NKA second(0, {'a', 'b'}, 4, {3, 1});

    first.addTransition(0, "a", 1);
    first.addTransition(1, "bb", 1);

    second.addTransition(0, "a", 3);
    second.addTransition(3, "bb", 1);
    second.addTransition(1, "b", 2);
    second.addTransition(2, "b", 3);

    ASSERT_TRUE(first == second);
}

TEST(testIsomorphysm, test3) {
    NKA first(0, {'a', 'b'}, 3, {2});

    first.addSymbol('c');

    first.addTransition(0, "cbc", 1);
    first.addTransition(0, "a", 2);
    first.addTransition(1, "ab", 1);
    first.addTransition(1, EPS, 0);
    first.addTransition(1, "c", 2);
    first.addTransition(2, "bb", 2);
    first.addTransition(2, "bc", 0);


    NKA second(3, {'a', 'c'}, {1, 3, 4, 5, 6, 8}, {8, 14});

    second.addSymbol('b');
    second.addConfiguration(14);

    second.addConfiguration(10);
    second.addConfiguration(11);
    second.addAcceptingConfiguration(11);

    second.addTransition(3, "c", 5);
    second.addTransition(3, EPS, 6);
    second.addTransition(5, "bc", 1);
    second.addTransition(1, "a", 4);
    second.addTransition(4, "b", 1);
    second.addTransition(1, "c", 14);
    second.addTransition(6, "a", 8);
    second.addTransition(14, "bc", 3);
    second.addTransition(8, "bb", 14);
    second.addTransition(8, EPS, 14);
    second.addTransition(6, "a", 14);
    second.addTransition(10, "b", 11);
    second.addTransition(11, "bb", 14);
    second.addTransition(14, "bb", 14);

    ASSERT_FALSE(second == first);

    second.addTransition(14, "b", 10);
    second.addTransition(1, EPS, 3);

    ASSERT_TRUE(second == first);

    first.addTransition(1, "b", 2);
    ASSERT_FALSE(second == first);

    second.addTransition(1, "b", 14);
    ASSERT_TRUE(second == first);

    second.makeDKA();
    ASSERT_TRUE(second == first);

    ASSERT_THROW(first.addTransition(4, "a", 2), std::invalid_argument);
    ASSERT_THROW(first.addTransition(1, "formal", 2), std::invalid_argument);
    ASSERT_THROW(first.addConfiguration(2), std::invalid_argument);

    first.makeMinFullDKAFromFullDKA();
    second.makeFullDKA();
    ASSERT_TRUE(second == first);

    for (int i = 0; i < 100; ++i) {
        long long add = first.addNewConfiguration(0);
        first.addAcceptingConfiguration(add);
        first.addTransition(0, "b", add);
    }
    ASSERT_THROW(first.makeDKA(), std::range_error);
}

TEST(testReplaceMultiSymbolsEdges, test1) {
    NKA nka(0, {'a', 'b', 'c'}, {0, 1, 2}, {2});

    nka.addTransition(0, "abc", 1);
    nka.addTransition(0, "abc", 2);
    nka.addTransition(0, "abc", 0);
    nka.addTransition(1, "ab", 2);

    nka.replaceMultiSymbolsEdges();

    NKA answer(0, {'a', 'b', 'c'}, 10, {2});

    answer.addTransition(0, "a", 3);
    answer.addTransition(3, "b", 4);
    answer.addTransition(4, "c", 1);
    answer.addTransition(0, "a", 5);
    answer.addTransition(5, "b", 6);
    answer.addTransition(6, "c", 2);
    answer.addTransition(0, "a", 7);
    answer.addTransition(7, "b", 8);
    answer.addTransition(8, "c", 0);
    answer.addTransition(1, "a", 9);
    answer.addTransition(9, "b", 2);

    ASSERT_TRUE(nka == answer);
}

TEST(testChangeEpsTransitions, test1) {
    NKA nka(0, {'a', 'b', 'c'}, {0, 1, 2, 3}, {0});

    nka.addTransition(0, EPS, 1);
    nka.addTransition(1, "a", 1);
    nka.addTransition(1, EPS, 2);
    nka.addTransition(2, "b", 2);
    nka.addTransition(2, EPS, 3);
    nka.addTransition(3, "c", 3);
    nka.addTransition(3, EPS, 0);

    nka.changeEpsTransitions();

    NKA answer(0, {'a', 'b', 'c'}, 4, {0, 1, 2, 3});

    answer.addTransition(0, "a", 1);
    answer.addTransition(0, "b", 2);
    answer.addTransition(0, "c", 3);
    answer.addTransition(1, "a", 1);
    answer.addTransition(1, "b", 2);
    answer.addTransition(1, "c", 3);
    answer.addTransition(2, "a", 1);
    answer.addTransition(2, "b", 2);
    answer.addTransition(2, "c", 3);
    answer.addTransition(3, "a", 1);
    answer.addTransition(3, "b", 2);
    answer.addTransition(3, "c", 3);

    ASSERT_TRUE(answer == nka);
}

TEST(testMakeExplicitWays, test1) {
    NKA nka(0, {'a', 'b'}, {0, 1, 2}, {2});

    nka.addTransition(0, "a", 1);
    nka.addTransition(0, "a", 2);
    nka.addTransition(1, "a", 0);
    nka.addTransition(1, "a", 1);
    nka.addTransition(1, "b", 2);
    nka.addTransition(2, "a", 1);
    nka.addTransition(2, "b", 2);
    nka.addTransition(2, "b", 0);

    nka.makeExplicitWays();

    NKA answer(0, {'a', 'b'}, 7, {3, 4, 5, 6});

    answer.addTransition(0, "a", 5);
    answer.addTransition(1, "a", 2);
    answer.addTransition(1, "b", 3);
    answer.addTransition(2, "a", 6);
    answer.addTransition(2, "b", 3);
    answer.addTransition(3, "a", 1);
    answer.addTransition(3, "b", 4);
    answer.addTransition(4, "a", 5);
    answer.addTransition(4, "b", 4);
    answer.addTransition(5, "a", 2);
    answer.addTransition(5, "b", 4);
    answer.addTransition(6, "a", 6);
    answer.addTransition(6, "b", 4);

    ASSERT_TRUE(answer == nka);
}

TEST(testFullDKAandAntiDKA, test1) {
    NKA nka(0, {'a', 'b', 'c'}, 3, {0});

    nka.addTransition(0, "c", 0);
    nka.addTransition(0, EPS, 1);
    nka.addTransition(1, "a", 1);
    nka.addTransition(1, "b", 2);
    nka.addTransition(1, EPS, 0);
    nka.addTransition(2, "c", 1);

    nka.makeDKA();

    NKA answer(0, {'a', 'b', 'c'}, 3, {0, 1});

    answer.addTransition(0, "a", 1);
    answer.addTransition(0, "b", 2);
    answer.addTransition(0, "c", 0);
    answer.addTransition(2, "c", 1);
    answer.addTransition(1, "a", 1);
    answer.addTransition(1, "b", 2);
    answer.addTransition(1, "c", 0);

    ASSERT_TRUE(answer == nka);

    nka.makeFullDKAFromDKA();

    answer.addConfiguration(3);
    answer.addTransition(2, "a", 3);
    answer.addTransition(2, "b", 3);
    answer.addTransition(3, "a", 3);
    answer.addTransition(3, "b", 3);
    answer.addTransition(3, "c", 3);
    ASSERT_TRUE(answer == nka);

    nka.makeAntiDKAFromFullDKA();
    answer.addAcceptingConfiguration(2);
    answer.addAcceptingConfiguration(3);
    answer.delAcceptingConfiguration(0);
    answer.delAcceptingConfiguration(1);

    ASSERT_TRUE(answer == nka);
}

TEST(testMinFullDKA, test1) {
    NKA nka(0, {'a', 'b'}, 3, {2});

    nka.addTransition(0, "a", 1);
    nka.addTransition(1, "ab", 1);
    nka.addTransition(1, "ba", 1);
    nka.addTransition(1, "b", 2);
    nka.addTransition(2, "a", 2);
    nka.addTransition(2, "ba", 2);
    nka.addTransition(2, EPS, 0);

    nka.makeMinFullDKA();

    NKA answer(0, {'a', 'b'}, 8, {3, 4, 6});

    answer.addTransition(0, "a", 2);
    answer.addTransition(0, "b", 1);
    answer.addTransition(1, "a", 1);
    answer.addTransition(1, "b", 1);
    answer.addTransition(2, "a", 5);
    answer.addTransition(2, "b", 3);
    answer.addTransition(3, "a", 4);
    answer.addTransition(3, "b", 7);
    answer.addTransition(4, "a", 6);
    answer.addTransition(4, "b", 3);
    answer.addTransition(5, "a", 1);
    answer.addTransition(5, "b", 2);
    answer.addTransition(6, "a", 6);
    answer.addTransition(6, "b", 4);
    answer.addTransition(7, "a", 3);
    answer.addTransition(7, "b", 1);
    ASSERT_TRUE(answer == nka);
}
