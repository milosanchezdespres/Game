#pragma once

#include <cstddef>
#include <iostream>

namespace px
{
//------------------------------------------------------------
    using std::size_t;
    using std::cout, std::endl;

    using Entity = size_t;

//------------------------------------------------------------
    struct IComponent
    {
        protected:
            size_t ID_;

            void set_id(size_t new_id) { ID_ = new_id; }

        public:
            const size_t& ID;

            IComponent() : ID(ID_) {}
            virtual ~IComponent() = 0;

            virtual void init(size_t new_id) { set_id(new_id); }
    };

    inline IComponent::~IComponent() = default;

//------------------------------------------------------------    
    struct _ECS
    {
        //...

        _ECS()
        {
            //...
        }
    };

    /* ! \ do not access this directly / ! \*/
    inline _ECS* ecs;
}
    
//------------------------------------------------------------ 
//access ECS just by writting ECS, do not instantiate
using px::_ECS;
using px::ecs;

#define ECS \
    ([]() -> _ECS* { \
        if(!ecs) ecs = new _ECS(); \
        return ecs; \
    })()

//------------------------------------------------------------ 