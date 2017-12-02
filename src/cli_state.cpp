#include "os/os.h"
#include "cli.h"
#include "indicator.h"

using namespace os;

bool cmd_state(const String &args)
{
    indicator::state_t ring, red, green,  blue;
    indicator::get(ring, red, green, blue);

    attr(BLACK, BG_CYAN);
    print(F("Ring status"));
    attr(RESET);
    println();

    print(F("RING: "));    
    attr(LIGHT_CYAN);
    print(indicator::print(ring));
    attr(RESET);    
    println();

    print(F("CORE: "));    
    attr(LIGHT_RED);
    print(indicator::print(red));
    attr(RESET);
    print(" ");
    attr(LIGHT_GREEN);
    print(indicator::print(green));
    attr(RESET);
    print(" ");
    attr(LIGHT_BLUE);
    print(indicator::print(blue));
    attr(RESET);
    println();

    return true;
}
