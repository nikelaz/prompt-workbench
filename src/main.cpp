#include "app.h"

int main(void)
{
    app::AppState options;
    app::init(options);
    app::renderLoop();
    app::shutdown();
    return 0;
}
