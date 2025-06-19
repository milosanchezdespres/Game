#pragma once

#define DEFINE_SINGLETON_ACCESSOR(Type, FuncName) \
    inline Type& FuncName() { return Type::instance(); }

namespace px
{
    template <typename T>
    struct Singleton
    {
        public:
            static T& instance()
            {
                static T instance;
                return instance;
            }

        protected:
            Singleton() = default;
            ~Singleton() = default;

            Singleton(const Singleton&) = delete;
            Singleton& operator=(const Singleton&) = delete;
    };
}