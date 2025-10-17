#include "routing.h"
#include "GLFW/glfw3.h"

routing::Router routing::init(Route default_route)
{
    Router router {
        default_route,
    };

    router.history.push_back(default_route);

    return router;
}

void routing::push(Router& router, Route route)
{
    if (router.current_route == route) return;

    router.current_route = route;
    router.history.push_back(route);
}

void routing::back(Router& router)
{
    if (router.history.size() == 1) return;

    router.history.pop_back();
    router.current_route = router.history.back();

    // This is necessary to force a redraw/wake up render loop
    // otherwise there can be perceivable letency 
    glfwPostEmptyEvent();
}
