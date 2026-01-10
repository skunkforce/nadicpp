#ifndef NADICPP_POOL_HPP
#define NADICPP_POOL_HPP
#include <deque>
#include <vector>
#include <ranges>
#include <algorithm>
#include <nadi/nadi.h>

namespace nadicpp{
    template<typename T>
    class pool{
        std::deque<T> buffer_;
        std::vector<T*> free_stack_;
        public:
        pool(std::size_t prealloc = 0):buffer_(prealloc),free_stack_(prealloc){
            std::ranges::transform(buffer_, std::back_inserter(free_stack_),
                       [](T& obj) -> T* { return &obj; });
        }
        T* allocate(){
            if(free_stack_.empty()){
                buffer_.push_back(T{});
                free_stack_.push_back(&buffer_.back());
            }
            T* r = free_stack_.back();
            free_stack_.pop_back();
            return r;
        }
        void free(T* p){
            free_stack_.push_back(p);
        }
    };

    //this allocator will allocate messages without setting the type string pointer
    //it is intended to be used in contexts where the type is fixed.
    template<nadi_free_callback cb, std::size_t Buff_Size>
    class message_allocator{
        struct char_buf_t{
            char c[Buff_Size];
        };

        nadicpp::pool<nadi_message> msg_pool_;
        nadicpp::pool<char_buf_t> msg_data_pool_;
        public:
        message_allocator(std::size_t prealloc = 0):msg_pool_(prealloc),msg_data_pool_(prealloc){}
        message allocate(){
            nadi_message* m = msg_pool_.allocate();
            m->data_length = 0;
            m->data = msg_data_pool_.allocate();
            m->user = this;
            m->free = cb;
            return message(m);
        }
        void free(nadi_message* m){
            msg_data_pool_.free(static_cast<char_buf_t*>(m->data));
            msg_pool_.free(m);
        }
    };

}
#endif