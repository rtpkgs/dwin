from building import * 

# get current dir path
cwd = GetCurrentDir()

# init src and inc vars
inc = []
src = []

# add dwlib common header 
inc = inc + [cwd + '/dwlib']

# add dwlib basic code
inc = inc + [cwd + '/dwlib/basic']
src = src + ['./dwlib/basic/dw_init.c']
src = src + ['./dwlib/basic/dw_system.c']

# add dwlib port code
inc = inc + [cwd + '/dwlib/port']
src = src + ['./dwlib/port/dw_port.c']

# add dwlib plugins space manager
if GetDepend(['PKG_DWLIB_ENABLE_SM']):
    inc = inc + [cwd + '/dwlib/plugins/spacemanager']
    src = src + ['./dwlib/plugins/spacemanager/dw_space_manager.c']

# add dwlib plugins button
if GetDepend(['PKG_DWLIB_ENABLE_BUTTON']):
    inc = inc + [cwd + '/dwlib/plugins/button']
    src = src + ['./dwlib/plugins/button/dw_button.c']

# add group to IDE project
group = DefineGroup('dwlib', src, depend = ['PKG_USING_DWLIB'], CPPPATH = inc)

Return('group')
