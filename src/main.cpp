#include "app.h"
#include "dba.h"
#include <string>

int main(void)
{
    dba::init("data.db"); 
    dba::create_test_suite(
        "Test Suite #1",
        "Some Description",
        "Talk like a pirate",
        "Microsoft/Phi-4"
    );

    dba::create_test_suite(
        "Another Test Suite",
        "Lorem ipsum dolor site amet",
        "Talk like a pirate",
        "OpenAI/GPT-5"
    );

    app::AppState options;
    app::init(options);
    app::renderLoop();
    app::shutdown();
    return 0;
}
