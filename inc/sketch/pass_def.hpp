#ifndef __GPSE_SKETCH_PASS_DEF_H__
#define __GPSE_SKETCH_PASS_DEF_H__

#include "lang/treepass.hpp"

namespace gpse
{
  namespace sketch
  {
    lang::TreePass getPrinterPass();
    lang::TreePass getTypecheckPass();
    lang::TreePass getOptimizePass();
    lang::TreePass getRunPass();
  }
}

#endif // __GPSE_SKETCH_PASS_DEF_H__
