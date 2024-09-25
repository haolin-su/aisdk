/******************************************************************************
  Copyright (C), 2024, Shenzhen G&T Industrial Development Co., Ltd

  File:      processor.h

  Author:  leo.li         
  Version: 1.0        
  Date:  2024.09
  
  Description:   processor APIs 

** History:
**Author (core ID)                Date          Number     Description of Changes
**-----------------------------------------------------------------------------
** leo.li                      09-Sep-2024     LIBtt54238   Init 
** -----------------------------------------------------------------------------
******************************************************************************/

#ifndef FIBO_PROCESSOR_H_
#define FIBO_PROCESSOR_H_

#include <string>
#include <vector>

namespace aisdk {

class Processor {
public:
    Processor() = default;
    virtual ~Processor() = default;
    virtual bool OnProcess();
};



} // namespace aisdk

#endif  // FIBO_PROCESSOR_H_