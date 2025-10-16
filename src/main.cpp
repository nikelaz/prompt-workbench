#include "app.h"
#include "dba.h"
#include <iostream>
#include <string>

using std::optional;

int main(void)
{
    dba::init("data.db"); 

    /*
    optional<int64_t> test_suite_id = dba::create_test_suite(
        "test title",
        "test description",
        "talk like a pirate",
        "test model"
    );
    */

    optional<TestSuite> test_suite = dba::get_test_suite(1);

    if (test_suite)
    {
        optional<int64_t> user_prompt_id = dba::create_user_prompt(
            "Hello!",
            test_suite->id
        );

        if (user_prompt_id)
        {
            std::cout << "User Prompt ID: " << std::to_string(user_prompt_id.value()) << std::endl;
        }
    }

    /*
    std::optional<TestSuite> test_suite = dba::get_test_suite(1);
    std::cout << "Title: " << test_suite->title << std::endl;
    std::cout << "Description: " << test_suite->description << std::endl;
    std::cout << "System Prompt: " << test_suite->system_prompt << std::endl;
    std::cout << "Model: " << test_suite->model << std::endl;

    std::vector<TestSuite> test_suites = dba::get_all_test_suites();

    for (TestSuite ts : test_suites) {
        std::cout << "Title: " << test_suite->title << std::endl;
    }
    */

    app::AppState options;
    app::init(options);
    app::renderLoop();
    app::shutdown();
    return 0;
}
