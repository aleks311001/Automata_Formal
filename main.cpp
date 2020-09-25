#include <iostream>
#include "NKA.h"

void testReplaceMultiSymbolsEdges();
void testChangeEpsTransitions();
void testMakeExplicitWays();
void testFullDKA();
void task1();
void task2();

int main() {
    //testReplaceMultiSymbolsEdges();
    //testMakeExplicitWays();
    //testChangeEpsTransitions();
    task2();

    return 0;
}

void testReplaceMultiSymbolsEdges() {
    NKA nka(0, {'a', 'b', 'c'}, {0, 1, 2}, {2});

    nka.addTransition(0, "abc", 1);
    nka.addTransition(0, "abc", 2);
    nka.addTransition(0, "abc", 0);
    nka.addTransition(1, "aabb", 2);

    nka.createTexFileThisNKA("tests/testReplaceMultiSymbolsEdgesNKA.tex", 5);
    nka.replaceMultiSymbolsEdges();
    nka.createTexFileThisNKA("tests/testReplaceMultiSymbolsEdgesNKARes.tex", 5);
}

void testChangeEpsTransitions() {
    NKA nka(0, {'a', 'b', 'c'}, {0, 1, 2, 3}, {0});

    nka.addTransition(0, EPS, 1);
    nka.addTransition(1, "a", 1);
    nka.addTransition(1, EPS, 2);
    nka.addTransition(2, "b", 2);
    nka.addTransition(2, EPS, 3);
    nka.addTransition(3, "c", 3);
    nka.addTransition(3, EPS, 0);

    nka.createTexFileThisNKA("tests/TestChangeEpsTransitionsNKA.tex", 5);
    nka.changeEpsTransitions();
    nka.createTexFileThisNKA("tests/TestChangeEpsTransitionsDKA.tex", 5);
}

void testMakeExplicitWays() {
    NKA nka(0, {'a', 'b'}, {0, 1, 2}, {2});

    nka.addTransition(0, "a", 1);
    nka.addTransition(0, "a", 2);
    nka.addTransition(1, "a", 0);
    nka.addTransition(1, "a", 1);
    nka.addTransition(1, "b", 2);
    nka.addTransition(2, "a", 1);
    nka.addTransition(2, "b", 2);
    nka.addTransition(2, "b", 0);

    nka.createTexFileThisNKA("tests/TestMakeExplicitWaysNKA.tex", 5);
    nka.makeExplicitWays();
    nka.createTexFileThisNKA("tests/TestMakeExplicitWaysDKA.tex", 5);
}

void testFullDKA() {
    NKA nka(0, {'a', 'b', 'c'}, 3, {0});

    nka.addTransition(0, "c", 0);
    nka.addTransition(0, EPS, 1);
    nka.addTransition(1, "a", 1);
    nka.addTransition(1, "b", 2);
    nka.addTransition(1, EPS, 0);
    nka.addTransition(2, "c", 1);

    nka.createTexFileThisNKA("tests/TestFullDKA_NKA.tex", 5);
    nka.changeEpsTransitions();
    nka.createTexFileThisNKA("tests/TestFullDKA_NKAwithoutEPS.tex", 5);
    nka.makeExplicitWays();
    nka.createTexFileThisNKA("tests/TestFullDKA_DKA.tex", 5);
    nka.makeFullDKAFromDKA();
    nka.createTexFileThisNKA("tests/TestFullDKA_FullDKA.tex", 5);
    nka.makeAntiDKAFromFullDKA();
    nka.createTexFileThisNKA("tests/TestFullDKA_AntiDKA.tex", 5);
}

void task1() {
    NKA nka(0, {'a', 'b'}, 2, {0, 1});

    nka.addTransition(0, "ab", 0);
    nka.addTransition(0, "ba", 0);
    nka.addTransition(0, "a", 1);
    nka.addTransition(0, "b", 1);

    nka.createTexFileThisNKA("HW3/task1NKA.tex", 5);
    nka.makeDKA();
    nka.createTexFileThisNKA("HW3/task1DKA.tex", 5);
    nka.makeFullDKAFromDKA();
    nka.createTexFileThisNKA("HW3/task1FullDKA.tex", 5);
    nka.makeAntiDKAFromFullDKA();
    nka.createTexFileThisNKA("HW3/task1AntiDKA.tex", 5);
}

void task2() {
    NKA nka(0, {'a', 'b'}, 5, {1, 2, 4});

    nka.addTransition(0, EPS, 1);
    nka.addTransition(0, EPS, 2);
    nka.addTransition(1, "ab", 1);
    nka.addTransition(1, "a", 4);
    nka.addTransition(4, "a", 4);
    nka.addTransition(2, "a", 3);
    nka.addTransition(2, "b", 3);
    nka.addTransition(3, "a", 2);
    nka.addTransition(3, "b", 2);

    nka.createTexFileThisNKA("HW3/task2NKA.tex", 5);
    nka.makeDKA();
    nka.createTexFileThisNKA("HW3/task2DKA.tex", 5);
    nka.makeFullDKAFromDKA();
    nka.createTexFileThisNKA("HW3/task2FullDKA.tex", 5);
    nka.makeAntiDKAFromFullDKA();
    nka.createTexFileThisNKA("HW3/task2AntiDKA.tex", 5);
}
