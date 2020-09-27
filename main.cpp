#include <iostream>
#include "NKA.h"

void task1();
void task2();
void task3();
void task2aSem();

int main() {
    /*task1();
    task2();
    task3();
    task2aSem();*/
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

void task3() {
    NKA nka(0, {'a', 'b'}, 6, {1});

    nka.addTransition(0, "a", 1);
    nka.addTransition(1, EPS, 2);
    nka.addTransition(2, "b", 3);
    nka.addTransition(2, "a", 4);
    nka.addTransition(3, EPS, 1);
    nka.addTransition(2, "a", 4);
    nka.addTransition(4, "ab", 4);
    nka.addTransition(4, "a", 5);
    nka.addTransition(5, "ab", 5);
    nka.addTransition(5, EPS, 1);

    nka.createTexFileThisNKA("HW3/task3NKA.tex", 5);
    nka.makeDKA();
    nka.createTexFileThisNKA("HW3/task3DKA.tex", 5);
}

void task2aSem() {
    NKA nka(0, {'a', 'b'}, 3, {2});

    nka.addTransition(0, "a", 1);
    nka.addTransition(1, "ab", 1);
    nka.addTransition(1, "ba", 1);
    nka.addTransition(1, "b", 2);
    nka.addTransition(2, "a", 2);
    nka.addTransition(2, "ba", 2);
    nka.addTransition(2, EPS, 0);

    nka.createTexFileThisNKA("Sem/task2aNKA.tex", 5);
    nka.makeDKA();
    nka.createTexFileThisNKA("Sem/task2aDKA.tex", 5);
    nka.makeFullDKAFromDKA();
    nka.createTexFileThisNKA("Sem/task2aFullDKA.tex", 5);
    nka.makeMinFullDKAFromFullDKA();
    nka.createTexFileThisNKA("Sem/task2aMinFullDKA.tex", 5);
}
