#ifndef NADICPP_POOL_HPP
#define NADICPP_POOL_HPP
#include <deque>
#include <vector>
#include <ranges>
#include <algorithm>
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

}
#endif