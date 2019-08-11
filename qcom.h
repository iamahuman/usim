#ifndef USIM_QCOM_H
#define USIM_QCOM_H

#include "usim.h"

#if LISPM_SYSTEM <= 7800L
#include "qcom78.h"
#elif LISPM_SYSTEM >= 9900L
#include "qcom99.h"
#endif

#endif
