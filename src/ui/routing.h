#pragma once

#include <vector>

namespace routing {
    enum Route {
        TEST_SUITES,
        TEST_SUITES_DETAILS,
        CREATE_TEST_SUITE,
        EDIT_TEST_SUITE,
        USER_PROMPT_DETAILS,
        USER_PROMPTS_LIST,
        CREATE_USER_PROMPTS,
        EDIT_USER_PROMPT,
        RESULT_RUNS_LIST,
        RESULT_RUN_DETAILS,
        ANSWER_DETAILS,
        COMPARE_RESULT_RUNS,
        SYSTEM_PROMPT_COMPARISON,
        API_CREDENTIALS,
        PAGE_2
    };

    struct Router {
        Route current_route;
        std::vector<Route> history;
    };

    Router init(Route default_route);
    void push(Router& router, Route route);
    void back(Router& router);
    bool has_prev(Router& router);
}
