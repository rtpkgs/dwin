#ifndef __DWIN_H_
#define __DWIN_H_

#include "dwin_init.h" 

#ifdef PKG_DWIN_ENABLE_PLUGIN_BUTTON
#include "dwin_plugin_button.h" 
#endif

#ifdef PKG_DWIN_ENABLE_PLUGIN_INPUTBOX
#include "dwin_plugin_inputbox.h" 
#endif

#ifdef PKG_DWIN_ENABLE_PLUGIN_ICON
#include "dwin_plugin_icon.h" 
#endif

#ifdef PKG_DWIN_ENABLE_PLUGIN_TEXTBOX
#include "dwin_plugin_textbox.h" 
#endif

#endif 
