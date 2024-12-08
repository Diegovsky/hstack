project('hstack', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'c_std=gnu11'])


libunwind = dependency('libunwind')
hstack = library('hstack', sources: ['hstack.c', 'hstack.h'], dependencies: libunwind)

exe = executable('main', 'main.c', link_with: hstack, install : true)
