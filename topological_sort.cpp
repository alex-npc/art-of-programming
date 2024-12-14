/*
* Implementation of topological sort found on page 265 of
* The Art of Computer Programming, Vol. 1: Fundamental Algorithms, 3rd Edition
* by Donald Knuth
*/

#include <array>
#include <iostream>
#include <span>
#include <vector>

// A relation is defined by a pair of two integers.
// It is read first preceeds second, as in the
// pair's first member should occur before the
// second member in the output list
using Relation = std::pair<int, int>;

struct node
{
    int count = -1;
    std::vector<int> successors {};
};

std::vector<int> topological_sort(std::span<const Relation> relationships)
{
    const int max = [&]()
    {
        int l = 0;
        for (const auto &[lhs, rhs] : relationships)
            l = std::max(l, std::max(lhs, rhs));
        return l;
    }();


    std::vector<node> sorter(max + 1);
    std::vector<int> output;
    output.reserve(relationships.size());

    // Track # of times an item has a preceeding relationship
    // Track items this node succeeds
    for (const auto &[first, second] : relationships)
    {
        // Initialize counts, ensures we ignore negative output
        sorter[first].count = std::max(sorter[first].count, 0);
        sorter[second].count = std::max(sorter[second].count, 0);

        sorter[second].count++;
        sorter[first].successors.emplace_back(second);
    }

    const auto emplace_no_preceeding = [&](size_t ix)
    {
        if (0 == sorter[ix].count) { output.emplace_back(ix); }
    };

    // Enqueue each item which is not preceeded by another element
    for (size_t ix = 0; ix < sorter.size(); ix++)
    {
        emplace_no_preceeding(ix);
    }

    // For each enqueued item, iterate over successors and decrement
    // their preceeding counter
    for (size_t ix = 0; ix < output.size(); ix++)
    {
        const auto preceeding_index = output[ix];
        for (const auto &successor : sorter[preceeding_index].successors)
        {
            sorter[successor].count--;
            emplace_no_preceeding(successor);
        }
    }

    return output;
}

int main()
{
    constexpr std::array RELATIONS
    {
        Relation { 9, 2 },
        Relation { 3, 7 },
        Relation { 7, 5 },
        Relation { 5, 8 },
        Relation { 8, 6 },
        Relation { 4, 6 },
        Relation { 1, 3 },
        Relation { 7, 4 },
        Relation { 9, 5 },
        Relation { 2, 8 }
    };

    std::vector<int> res = topological_sort(RELATIONS);
    for (const auto &item : res)
        std::cout << item << '\n';
}
