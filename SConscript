from building import * 

# get current dir path
cwd = GetCurrentDir()

# init src and inc vars
src = []
inc = []

# add dwin common include
inc = inc + [cwd + "/dwin"]

# add dwin basic code
src = src + Glob('./dwin/basic/transport/*.c')

# add group to IDE project
group = DefineGroup('dwin', src, depend = ['PKG_USING_DWIN'], CPPPATH = inc)

Return('group')
