#ifndef NADICPP_ADDRESS_HPP
#define NADICPP_ADDRESS_HPP
#include <nadi/nadi.h>
namespace nadicpp{
    struct address{
        nadi_node_handle node;
        unsigned channel;
    };
}

#endif