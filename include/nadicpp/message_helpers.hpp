#ifndef MESSAGEHELPERS_HPP
#define MESSAGEHELPERS_HPP
#include <nadi/nadi.h>
#include <nlohmann/json.hpp>

namespace nadicpp::helpers{
    inline nadi_message* heap_allocate_json_message(nadi_node_handle node, unsigned channel, const nlohmann::json& data, nadi_free_callback free,void* usr = nullptr){
        auto msg = new nadi_message;
        const char meta[] = R"({"format":"json"})";
        auto sd = data.dump();

        char* m = new char[sizeof(meta)+1];
        strcpy(m,meta);
        msg->meta = m;
        msg->meta_hash = 0;
        msg->data_length = sd.size();
        char * dp = new char[msg->data_length+1];
        strcpy(dp,sd.c_str());
        msg->data = dp;
        msg->node = node;
        msg->channel = channel;
        msg->free = free;
        msg->user = usr;
        return msg;
    }
    inline nadi_message* heap_allocate_abstract_nodes_list(const nlohmann::json& list, nadi_free_callback free, const std::string_view id){
        auto data = nlohmann::json::object();
        data["type"] = "context.abstract_nodes.list";
        data["instances"] = list;
        data["id"] = id;
        return heap_allocate_json_message(0,0xF000,data,free);
    }
    inline nadi_message* heap_allocate_connect_confirm(nadi_free_callback free, const std::string_view id){
        auto data = nlohmann::json::object();
        data["type"] = "context.connect.confirm";
        data["status"] = "success";
        data["id"] = id;
        return heap_allocate_json_message(0,0xF000,data,free);
    }

    inline nadi_message* heap_allocate_message_from_JSON(const nlohmann::json& obj, uint64_t node, unsigned channel){
        if (!obj.contains("meta") || !obj["meta"].is_object() || !obj.contains("data")){
            return nullptr;
        }

        const nlohmann::json& data = obj["data"];
        if (data.is_string()){
            
        }
        else if(data.is_binary()){

        }
        else {
            return nullptr;
        }
    }
}

#endif