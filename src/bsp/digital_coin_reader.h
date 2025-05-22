#ifndef BSP_DIGITAL_COIN_READER_H_INCLUDED
#define BSP_DIGITAL_COIN_READER_H_INCLUDED


typedef enum {
    BSP_DIGITAL_COIN_READER_LINE_1 = 0,
#define BSP_DIGITAL_COIN_READER_LINES 1
} bsp_digital_coin_reader_line_t;


void bsp_digital_coin_reader_init(void);


#endif
