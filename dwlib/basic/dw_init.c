#include "dw_init.h"
#include "dw_port.h"

#if defined(PKG_DWLIB_ENABLE_SM)
#include "dw_space_manager.h"
#endif

/* dwlib≥ı ºªØ */
bool dw_init(const char *name, uint32_t baudrate)
{
    if(dw_port_serial_resource_init() == false)
    {
        dw_print("serial resource init failed");
        return false;
    }
    
    if(dw_port_serial_init(name, baudrate) == false)
    {
        dw_print("serial init failed");
        return false;
    }
    
#if defined(PKG_DWLIB_ENABLE_SM)
    if(dw_plugin_sm_init() == false)
    {
        dw_print("space manager init failed");
        return false;
    }
#endif
    
    return true;
}
