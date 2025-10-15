#include "app.h"
#include "dba.h"
#include <iostream>

int main(void)
{
    dba::init("data.db"); 

    std::unique_ptr<TestSuite> test_suite = dba::get_test_suite(1);
    std::cout << "Title: " << test_suite->title << std::endl;
    std::cout << "Description: " << test_suite->description << std::endl;
    std::cout << "System Prompt: " << test_suite->system_prompt << std::endl;
    std::cout << "Model: " << test_suite->model << std::endl;

    std::vector<TestSuite> test_suites = dba::get_all_test_suites();

    for (TestSuite ts : test_suites) {
        std::cout << "Title: " << test_suite->title << std::endl;
    }

    app::AppState options;
    app::init(options);
    app::renderLoop();
    app::shutdown();
    return 0;
}
