from building import * 

# get current dir path
cwd = GetCurrentDir()

# init src and inc vars
src = []
inc = []

# add dwlib common header 
inc = inc + [cwd + '/dwlib']

# add dwlib basic code
inc = inc + [cwd + '/dwlib/basic']
src = src + ['./dwlib/basic/dw_init.c']

# add dwlib port code
inc = inc + [cwd + '/dwlib/port']
src = src + ['./dwlib/port/dw_port.c']

# add group to IDE project
group = DefineGroup('dwlib', src, depend = ['PKG_USING_DWLIB'], CPPPATH = inc)

Return('group')
