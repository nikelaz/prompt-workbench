#include "app.h"
#include "dba.h"
#include <string>

int main(void)
{
    dba::DBAState dba_state;
    dba::init(dba_state, "data.db"); 
   
    app::AppState app_state;
    app::init(app_state);
    app::render_loop(app_state, dba_state);

    dba::deinit(dba_state);
    app::deinit();
    return 0;
}
