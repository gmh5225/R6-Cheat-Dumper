#ifndef _OVERLAY_INPUT_HPP_
#define _OVERLAY_INPUT_HPP_

#include <iostream>

namespace overlay::input {
    extern void enable();
    extern void disable();

    extern bool key_down(uint32_t key);
    extern bool key_pressed(uint32_t key);
}

#endif