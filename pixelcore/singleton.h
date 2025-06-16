#pragma once

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