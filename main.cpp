#include <iostream>
#include "NKA.h"

void testMakeExplicitWays();
void testChangeEpsTransitions();

int main() {
    testMakeExplicitWays();
    testChangeEpsTransitions();

    return 0;
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

    nka.createTexFileThisNKA("TestMakeExplicitWaysNKA.tex", 5);
    nka.makeExplicitWays();
    nka.createTexFileThisNKA("TestMakeExplicitWaysDKA.tex", 5);
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

    nka.createTexFileThisNKA("TestChangeEpsTransitionsNKA.tex", 5);
    nka.changeEpsTransitions();
    nka.createTexFileThisNKA("TestChangeEpsTransitionsDKA.tex", 5);
}

void testFullDKA() {
    NKA nka()
}
