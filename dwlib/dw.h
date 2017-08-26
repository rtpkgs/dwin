#ifndef __DW_H_
#define __DW_H_

#ifdef __cplusplus
extern "C" {
#endif
	
#include "dw_init.h"

#if defined(RT_USING_DW_SYSTEM)
    #include "dw_system.h"
#endif
	
#if defined(RT_USING_DW_LOGIN)
    #include "dw_login.h"
#endif
	
#if defined(RT_USING_DW_VICON)
    #include "dw_vicon.h"
#endif

#ifdef __cplusplus
}
#endif

#endif
