#ifndef USIM_DEFMIC_H
#define USIM_DEFMIC_H

#include "usim.h"

#if LISPM_SYSTEM <= 7800L
#include "defmic78.h"
#elif LISPM_SYSTEM >= 9900L
#include "defmic99.h"
#endif

#endif
