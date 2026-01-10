#ifndef NADICPP_ADDRESS_HPP
#define NADICPP_ADDRESS_HPP
#include <nadi/nadi.h>
#include <compare>
namespace nadicpp{
    struct address{
        nadi_node_handle node;
        unsigned channel;
        auto operator<=>(const address& other) const = default;
    };
}

#endif