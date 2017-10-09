from building import *

# get current dir path
cwd = GetCurrentDir()

# init src and inc vars
src = []
inc = []

# add dwin common include
inc = inc + [cwd + '/dwlib']

# add dwin basic code
src = src + ['./dwlib/basic/src/dw_init.c']
src = src + ['./dwlib/basic/src/dw_space.c']
inc = inc + [cwd + '/dwlib/basic/inc']

#add dwin plugins code 

#add system plugins code
if GetDepend('RT_USING_DW_SYSTEM'):
	src = src + ['./dwlib/plugins/system/dw_system.c']
	inc = inc + [cwd + '/dwlib/plugins/system']

#add cmd plugins code
if GetDepend('RT_USING_DW_CMD'):
	src = src + ['./dwlib/plugins/cmd/dw_cmd.c']
	inc = inc + [cwd + '/dwlib/plugins/cmd']

#add login plugins code
if GetDepend('RT_USING_DW_LOGIN'):
	src = src + ['./dwlib/plugins/login/dw_login.c']
	inc = inc + [cwd + '/dwlib/plugins/login']

#add vicon plugins code
if GetDepend('RT_USING_DW_VICON'):
	src = src + ['./dwlib/plugins/vicon/dw_vicon.c']
	inc = inc + [cwd + '/dwlib/plugins/vicon']

# add group to IDE project
group = DefineGroup('Dwinlib', src, depend = ['RT_USING_DWIN'], CPPPATH = inc)

Return('group')
