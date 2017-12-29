from building import * 

# get current dir path
cwd = GetCurrentDir()

# init src and inc vars
src = []
inc = []

# add clib list common include
inc = inc + [cwd + '/clist']

# add clib list basic code
src = src + ['./clist/list.c']
src = src + ['./clist/list_node.c']
src = src + ['./clist/list_iterator.c']

# add group to IDE project
group = DefineGroup('clist', src, depend = ['PKG_USING_CLIST'], CPPPATH = inc)

Return('group')
