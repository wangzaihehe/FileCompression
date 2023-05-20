#ifndef PQUEUE_H_
#define PQUEUE_H_

#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>
#include <iostream>
#include <type_traits>

template <typename T, typename C = std::less<T>>
class PQueue {
public:
    // Constructor
    PQueue() {}
    // Return number of items in priority queue
    size_t Size();
    // Return top of priority queue
    T& Top();
    // Remove top of priority queue
    void Pop();
    // Insert item and sort priority queue
    void Push(const T &item);

private:
    std::vector<T> items;
    size_t cur_size = 0;
    C cmp;


    // Helper methods for indices
    size_t Root() {
        return 0;
    }
    size_t Parent(size_t n) {
        return (n - 1) / 2;
    }
    size_t LeftChild(size_t n) {
        return 2 * n + 1;
    }
    size_t RightChild(size_t n) {
        return 2 * n + 2;
    }

    // Helper methods for node testing
    bool HasParent(size_t n) {
        return n != Root();
    }
    bool IsNode(size_t n) {
        return n < cur_size;
    }

    // Helper methods for restructuring
    void PercolateUp(size_t n);
    void PercolateDown(size_t n);

    // Node comparison
    bool CompareNodes(size_t i, size_t j);
};

// To be completed below
template <typename T, typename C>
size_t PQueue<T, C>::Size(){
    return cur_size;
}

template <typename T, typename C>
void PQueue<T, C>::PercolateUp(size_t n) {
    while (HasParent(n) && CompareNodes( n , Parent(n)))  {
        std::swap(items[Parent(n)], items[n]);
        n = Parent(n);
    }
}

template <typename T, typename C>
void PQueue<T, C>::PercolateDown(size_t n){
    // While node has at least one child (if one, necessarily on the left)
    while (IsNode(LeftChild(n))) {
        // Consider left child by default
        size_t child = LeftChild(n);
        // If right child exists and smaller than left child, then consider right child
        if (IsNode(RightChild(n)) && CompareNodes(RightChild(n),LeftChild(n)))
            child = RightChild(n);
        // Exchange smallest child with node to restore heap-order if necessary
        if (CompareNodes(child,n))
            std::swap(items[child], items[n]);
        else
            break;

        // Do it again, one level down
        n = child;
    }
}

template <typename T, typename C>
T& PQueue<T, C>::Top(void){
    if (!Size())
        throw std::underflow_error(
                "Empty priority queue!");
    return items[Root()];
}

template <typename T, typename C>
void PQueue<T, C>::Push(const T &item) {
    if (cur_size + 1 == items.size())
        throw std::overflow_error("Priority queue full!");
    // Insert at the end
    if (cur_size == 0){
        items.push_back(std::move(item));
        // Percolate up
        cur_size++;
    } else {
        items.push_back(std::move(item));
        // Percolate up
        cur_size++;
        PercolateUp(cur_size-1);
    }


}

template <typename T, typename C>
void PQueue<T, C>::Pop(){
    if (!Size())
        throw std::underflow_error("Empty priority queue!");
    // Move last item back to root and reduce heap's size


    items[Root()] = std::move(items[cur_size-1]);
    cur_size--;
    items.pop_back();
    PercolateDown(Root());


}

template <typename T, typename C>
bool PQueue<T, C>::CompareNodes(size_t i, size_t j){
    if (cmp(items[i],items[j])){
        return true;
    }
    return false;
}
#endif  // PQUEUE_H_
