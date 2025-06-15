#include "pixelcore/colors.h"
#include "pixelcore/ECS.h"

using namespace px;

struct Transform { float x, y; };

int main()
{
    Entity test = ECS.create_entity();

    ECS.add_component<Transform>(test);
    ECS.component<Transform>(test)->x  = 60;
    ECS.component<Transform>(test)->y  = 40;

    for(Entity entity : ECS.each<Transform>())
    {
        Transform* transform = ECS.component<Transform>(entity);

        cout << transform->x << endl;
        cout << transform->y << endl;
    }

    return 0;
}
