#ifndef NADI_STATE_HPP
#define NADI_STATE_HPP
#include "nadi/nadi.h"
namespace nadicpp{
    struct state{
        nadi_receive_callback receive_;
        void* receive_ctx_;
        nadi_node_handle node_id_;
    };
}
#endif