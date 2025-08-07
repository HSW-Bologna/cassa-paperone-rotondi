#include <string.h>
#include <assert.h>
#include "model.h"


void model_init(mut_model_t *model) {
    assert(model);
    *model = (mut_model_t){0};
}
