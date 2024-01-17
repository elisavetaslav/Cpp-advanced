#pragma once

#include <vector>

template <class Functor>
class ReverseBinaryFunctor {
public:
    ReverseBinaryFunctor(Functor func) : func_(func) {
    }

    template <class T>
    bool operator()(T one, T two) {
        return !func_(one, two);
    }

private:
    Functor func_;
};

template <class Functor>
class ReverseUnaryFunctor {
public:
    ReverseUnaryFunctor(Functor func) {
        func_ = func;
    };

    template <class T>
    bool operator()(T one) {
        return !func_(one);
    }

private:
    Functor func_;
};

template <class Functor>
ReverseUnaryFunctor<Functor> MakeReverseUnaryFunctor(Functor functor) {
    return ReverseUnaryFunctor<Functor>(functor);
}

template <class Functor>
ReverseBinaryFunctor<Functor> MakeReverseBinaryFunctor(Functor functor) {
    return ReverseBinaryFunctor<Functor>(functor);
}