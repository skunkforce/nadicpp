#include "address.hpp"
#include "callback.hpp"
#include "message.hpp"
#include "state.hpp"

// Use this macro at the bottom of your main.cpp
#define NADI_WRAP_C_LIBRARY_INTERFACE(WRAPPED_CLASS)                       \
extern "C" {                                                               \
    DLL_EXPORT nadi_status nadi_init(                                      \
        nadi_node_handle* node,                                            \
        nadi_receive_callback cb,                                          \
        void* cb_ctx,                                                      \
        char* init)                                                        \
    {                                                                      \
        try{                                                               \
            *node = new WRAPPED_CLASS(nadicpp::callback(cb, cb_ctx),       \
                                    std::string_view(init));               \
            return NADI_OK;                                                \
        }                                                                  \
        catch(...){                                                        \
            return NADI_UNKNOWN_ERROR;                                     \
        }                                                                  \
    }                                                                      \
                                                                           \
    DLL_EXPORT nadi_status nadi_deinit(nadi_node_handle node)              \
    {                                                                      \
        delete static_cast<WRAPPED_CLASS*>(node);                          \
        return NADI_OK;                                                    \
    }                                                                      \
                                                                           \
    DLL_EXPORT nadi_status nadi_send(                                      \
        nadi_message* message,                                             \
        nadi_node_handle node,                                             \
        unsigned int target_channel)                                       \
    {                                                                      \
        return static_cast<WRAPPED_CLASS*>(node)->send(                    \
            nadicpp::message(message), target_channel);                    \
    }                                                                      \
                                                                           \
    DLL_EXPORT nadi_status nadi_handle_events(nadi_node_handle node)       \
    {                                                                      \
        return static_cast<WRAPPED_CLASS*>(node)->handle_events();         \
    }                                                                      \
                                                                           \
    DLL_EXPORT nadi_status nadi_descriptor(char* str, size_t* length)      \
    {                                                                      \
        std::string js = WRAPPED_CLASS::descriptor();                      \
        *length = js.size();                                               \
        std::copy(js.begin(), js.end(), str);                              \
        return NADI_OK;                                                    \
    }                                                                      \
}