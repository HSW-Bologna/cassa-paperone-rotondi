#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED


#include "devices/cashless.h"


typedef struct {
    struct {
        mdb_cashless_t cashless;
    } run;
} mut_model_t;

typedef const mut_model_t model_t;


void model_init(mut_model_t *model);


#endif
