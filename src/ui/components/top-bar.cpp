#include "ui.h"

void ui::components::top_bar(routing::Router& router) {
    if (ui::components::button("Back"))
    {
        routing::back(router);
    }
}
