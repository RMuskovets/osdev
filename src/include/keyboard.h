#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <system.h>

#define RSHIFT 54
#define LSHIFT 42

void keyboard_handler(register_t * r);

void keyboard_init();

#endif
