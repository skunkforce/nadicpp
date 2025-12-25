#ifndef NADICPP_CALLBACK_HPP
#define NADICPP_CALLBACK_HPP
#include "nadi/nadi.h"
#include "message.hpp"
namespace nadicpp{
class callback{
    nadi_receive_callback fp_;
    void* ctx_;
    public:
    callback(nadi_receive_callback fp,void* ctx):fp_(fp),ctx_(ctx){}
    void operator()(message m){
        fp_(m.release(),ctx_);
    }
};
}
#endif