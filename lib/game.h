#ifndef GAME_H
    #define GAME_H
    #include "stdint.h"
    #include "stdint.h"
    #include "lcd.h"
    #include "cstdio"
    #include "stdlib.h"
    #include "delay.h"

    void game_init(void);

    void game_update(uint8_t x, uint8_t y);
#endif