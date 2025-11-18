#include "Flexable.h"

Flexable::Flexable() {
    reg("xAlignment", xAlignment);
    reg("yAlignment", yAlignment);
    reg("xOverflow", xOverflow);
    reg("yOverflow", yOverflow);
    reg("xFill", xFill);
    reg("yFill", yFill);
}
