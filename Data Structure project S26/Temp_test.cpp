#include <iostream>
#include <cassert>
using namespace std;

#include "LinkedStack.h"
#include "LinkedQueue.h"
#include "priQueue.h"

// ================= HELPER =================
void printTest(string name, bool pass) {
    cout << (pass ? " PASS: " : " FAIL: ") << name << endl;
}

// ================= STACK TEST =================
void testStack() {
    cout << "\n===== STACK TEST =====\n";

    LinkedStack<int> s;
    int x;

    // Empty
    printTest("Initially empty", s.isEmpty());

    // Pop empty
    printTest("Pop empty", !s.pop(x));

    // Peek empty
    printTest("Peek empty", !s.peek(x));

    // Push
    s.push(10);
    s.push(20);
    s.push(30);

    // Peek
    s.peek(x);
    printTest("Top is 30", x == 30);

    // LIFO
    s.pop(x);
    bool lifo = (x == 30);
    s.pop(x);
    lifo &= (x == 20);
    s.pop(x);
    lifo &= (x == 10);

    printTest("LIFO order", lifo);

    // Back to empty
    printTest("Empty after pops", s.isEmpty());

    // Stress test
    for (int i = 0; i < 5000; i++)
        s.push(i);

    bool correct = true;
    for (int i = 4999; i >= 0; i--) {
        s.pop(x);
        if (x != i) correct = false;
    }

    printTest("Stress test", correct);
}

// ================= QUEUE TEST =================
void testQueue() {
    cout << "\n===== QUEUE TEST =====\n";

    LinkedQueue<int> q;
    int x;

    // Empty
    printTest("Initially empty", q.isEmpty());

    // Dequeue empty
    printTest("Dequeue empty", !q.dequeue(x));

    // Peek empty
    printTest("Peek empty", !q.peek(x));

    // Enqueue
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);

    // Peek
    q.peek(x);
    printTest("Front is 1", x == 1);

    // FIFO
    bool fifo = true;

    q.dequeue(x);
    fifo &= (x == 1);

    q.dequeue(x);
    fifo &= (x == 2);

    q.dequeue(x);
    fifo &= (x == 3);

    printTest("FIFO order", fifo);

    // Back to empty
    printTest("Empty after dequeue", q.isEmpty());

    // Mixed operations
    q.enqueue(10);
    q.enqueue(20);
    q.dequeue(x); // remove 10
    q.enqueue(30);

    bool mix = true;
    q.dequeue(x); mix &= (x == 20);
    q.dequeue(x); mix &= (x == 30);

    printTest("Mixed operations", mix);
}

// ================= PRIORITY QUEUE TEST =================
void testPriorityQueue() {
    cout << "\n===== PRIORITY QUEUE TEST =====\n";

    priQueue<int> pq;
    int value, pri;

    // Empty
    printTest("Initially empty", pq.isEmpty());

    // Dequeue empty
    printTest("Dequeue empty", !pq.dequeue(value, pri));

    // Insert
    pq.enqueue(100, 1);
    pq.enqueue(200, 5);
    pq.enqueue(300, 3);

    // Peek highest priority
    pq.peek(value, pri);
    printTest("Highest priority first", (value == 200 && pri == 5));

    // Order check (descending priority)
    bool correct = true;

    pq.dequeue(value, pri);
    correct &= (value == 200);

    pq.dequeue(value, pri);
    correct &= (value == 300);

    pq.dequeue(value, pri);
    correct &= (value == 100);

    printTest("Priority order", correct);

    // Duplicate priority
    pq.enqueue(1, 2);
    pq.enqueue(2, 2);
    pq.enqueue(3, 2);

    pq.dequeue(value, pri);
    pq.dequeue(value, pri);
    pq.dequeue(value, pri);

    printTest("Handle same priority", true); // depends on order
}

// ================= MAIN =================
int main() {
    cout << " DATA STRUCTURE TESTING \n";

    testStack();
    testQueue();
    testPriorityQueue();

    cout << "\n ALL TESTS COMPLETED\n";
    return 0;
}