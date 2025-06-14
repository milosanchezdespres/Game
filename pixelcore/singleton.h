#pragma once

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
};