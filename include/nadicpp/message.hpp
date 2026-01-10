#ifndef NADICPP_MESSAGE_HPP
#define NADICPP_MESSAGE_HPP
#include "nadi/nadi.h"
#include "address.hpp"
#include "nlohmann/json.hpp"
#include <optional>
#include <string_view>
namespace nadicpp{
class message{
    nadi_message * msg = nullptr;
    public:
    explicit message(nadi_message* m = nullptr) noexcept
        : msg(m) {}

    ~message() {
        if(msg){
            msg->free(msg);       
        }      
    }

    message(const message&)            = delete;
    message& operator=(const message&) = delete;

    message(message&& other) noexcept
        : msg(other.msg)
    {
        other.msg = nullptr;      // leave the source in a valid empty state
    }

    message& operator=(message&& other) noexcept
    {
        if (this != &other) {
            if(msg){
                delete msg;           // clean up current resource
            }
            msg = other.msg;
            other.msg = nullptr;
        }
        return *this;
    }

    nadi_message * get(){
        return msg;
    }

    nadi_message * release(){
        auto r = msg;
        msg = nullptr;
        return r;
    }

    void set_address(address a){
        if(msg){
            msg->node = a.node;
            msg->channel = a.channel;
        }
    }
    nlohmann::json meta(){
        return nlohmann::json::parse(msg->meta);
    }

    bool is_json_format(){
        auto m = meta();
        return m["format"] == "json";
    }
    
    std::optional<nlohmann::json> to_json(){
        if(is_json_format()){
            auto pdata = (const char*)msg->data;
            auto out = nlohmann::json::object();
            out["meta"] = nlohmann::json::parse(msg->meta);
            auto sv = std::string_view(pdata,pdata+msg->data_length);
            out["data"] = nlohmann::json::parse(sv);
            return out;
        }
        return {};
    }

};
}
#endif