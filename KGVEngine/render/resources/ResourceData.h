#ifndef RESOURCE_DATA_H_
#define RESOURCE_DATA_H_

#include "pch.h"

struct ResourceData {
    const void *Data;
    U32 memPitch;
    U32 memSlicePitch;
};

#endif // RESOURCE_DATA_H_