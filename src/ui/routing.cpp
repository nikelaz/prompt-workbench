#include "routing.h"
#include "GLFW/glfw3.h"

static const int PARENT_ROUTE[] = {
    -1,                                       // PROMPT_EDITOR (0)
    (int)routing::PROMPT_EDITOR,              // TEST_SUITES (1)
    (int)routing::TEST_SUITES,                // TEST_SUITES_DETAILS (2)
    (int)routing::TEST_SUITES,                // CREATE_TEST_SUITE (3)
    (int)routing::TEST_SUITES_DETAILS,        // EDIT_TEST_SUITE (4)
    (int)routing::USER_PROMPTS_LIST,          // USER_PROMPT_DETAILS (5)
    (int)routing::TEST_SUITES_DETAILS,        // USER_PROMPTS_LIST (6)
    (int)routing::USER_PROMPTS_LIST,          // CREATE_USER_PROMPTS (7)
    (int)routing::USER_PROMPT_DETAILS,        // EDIT_USER_PROMPT (8)
    (int)routing::TEST_SUITES_DETAILS,        // RESULT_RUNS_LIST (9)
    (int)routing::RESULT_RUNS_LIST,           // RESULT_RUN_DETAILS (10)
    (int)routing::RESULT_RUN_DETAILS,         // ANSWER_DETAILS (11)
    (int)routing::RESULT_RUNS_LIST,           // COMPARE_RESULT_RUNS (12)
    (int)routing::COMPARE_RESULT_RUNS,        // SYSTEM_PROMPT_COMPARISON (13)
    -1,                                       // API_CREDENTIALS (14)
    -1,                                       // PAGE_2 (15)
};

routing::Router routing::init(Route default_route)
{
    return Router { default_route };
}

void routing::push(Router& router, Route route)
{
    if (router.current_route == route) return;
    router.current_route = route;
}

void routing::back(Router& router)
{
    int parent = PARENT_ROUTE[(int)router.current_route];
    if (parent == -1) return;
    router.current_route = (Route)parent;

    // This is necessary to force a redraw/wake up render loop
    // otherwise there can be perceivable latency
    glfwPostEmptyEvent();
}

bool routing::has_prev(Router& router)
{
    return PARENT_ROUTE[(int)router.current_route] != -1;
}
