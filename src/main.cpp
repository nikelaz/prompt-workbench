#include "app.h"
#include "dba.h"
#include <string>

int main(void)
{
    dba::init("data.db"); 
    app::AppState options;
    app::init(options);
    app::renderLoop();
    app::shutdown();
    return 0;
}
