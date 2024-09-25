/******************************************************************************
  Copyright (C), 2024, Shenzhen G&T Industrial Development Co., Ltd

  File:      license.h

  Author:  leo.li         
  Version: 1.0        
  Date:  2024.09
  
  Description:   license APIs 

** History:
**Author (core ID)                Date          Number     Description of Changes
**-----------------------------------------------------------------------------
** leo.li                      09-Sep-2024     LIBtt54238   Init 
** -----------------------------------------------------------------------------
******************************************************************************/

#ifndef FIBO_LICENSE_H_
#define FIBO_LICENSE_H_

#include <string>

namespace aisdk {

class License {
public:
    static bool validate(const std::string& license_key)
    {
        return true;
    }
};

} // namespace aisdk

#endif  // FIBO_LICENSE_H_