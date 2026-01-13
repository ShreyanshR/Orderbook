#pragma once

#include <limits>
#include "GlobalVariables.h"

struct Constants
{
    static const Price InvalidPrice = std::numeric_limits<Price>::quiet_NaN();
};