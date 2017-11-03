#ifndef __DW_H_
#define __DW_H_

/* dwlib common header file */
#include "dw_common.h"

/* dwlib all api */

/* dwlib basic function api */
#include "dw_init.h"
#include "dw_system.h"

/* dwlib plugins function api */
#if defined(PKG_DWLIB_ENABLE_SM)
#include "dw_space_manager.h"
#endif

#endif
