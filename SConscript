from building import * 

# get current dir path
cwd = GetCurrentDir()

# init src and inc vars
src = []
inc = []

# add dwin common include
inc = inc + [cwd + "/dwin"]

# add dwin basic code
inc = inc + [cwd + "/dwin/basic"]
src = src + Glob('./dwin/basic/*.c')

# add dwin plugins code 
if GetDepend('PKG_DWIN_ENABLE_PLUGIN_BUTTON'):
	inc = inc + [cwd + "/dwin/plugins/button"]
	src = src + Glob('./dwin/plugins/button/*.c')

if GetDepend('PKG_DWIN_ENABLE_PLUGIN_ICON'):
	inc = inc + [cwd + "/dwin/plugins/icon"]
	src = src + Glob('./dwin/plugins/icon/*.c')

if GetDepend('PKG_DWIN_ENABLE_PLUGIN_INPUTBOX'):
	inc = inc + [cwd + "/dwin/plugins/inputbox"]
	src = src + Glob('./dwin/plugins/inputbox/*.c')

if GetDepend('PKG_DWIN_ENABLE_PLUGIN_TEXTBOX'):
	inc = inc + [cwd + "/dwin/plugins/textbox"]
	src = src + Glob('./dwin/plugins/textbox/*.c')

# add group to IDE project
group = DefineGroup('dwin', src, depend = ['PKG_USING_DWIN'], CPPPATH = inc)

Return('group')
