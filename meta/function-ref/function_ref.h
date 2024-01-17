#pragma once

#include <utility>
#include <type_traits>
#include <memory>
#include <functional>


template <typename Signature>
class FunctionRef;

template <class F, class ... Args>
class FunctionRef<F(Args ...)> {
public:
    template <class Func>
    FunctionRef(Func&& func) {
        ptr_ = reinterpret_cast<void*>(&func);
        pryg_ = [](void* f, Args&& ... args) -> F {
            if constexpr (std::is_void_v<F>) {
            (*reinterpret_cast<typename std::add_pointer<Func>::type>(f))(std::forward<Args>(args)...);
        } else {
            return (*reinterpret_cast<typename std::add_pointer<Func>::type>(f))(std::forward<Args>(args)...);
        }
        };
    }
    template <class Func>
    FunctionRef(Func* func) {
        ptr_ = reinterpret_cast<void*>(func);
        pryg_ = [](void* f, Args&& ... args) -> F {
            if constexpr (std::is_void_v<F>) {
            (*reinterpret_cast<typename std::add_pointer<Func>::type>(f))(std::forward<Args>(args)...);
        } else {
            return (*reinterpret_cast<typename std::add_pointer<Func>::type>(f))(std::forward<Args>(args)...);
        }
        };
    }
    auto operator()(Args&& ... args){
        if constexpr (std::is_void_v<F>) {
            pryg_(ptr_, std::forward<Args>(args)...);
        } else {
            return pryg_(ptr_, std::forward<Args>(args)...);
        }
    }
private:
    F (*pryg_)(void*, Args&&...) = nullptr;
    void* ptr_ = nullptr;
};