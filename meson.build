project ('ddb_spy_plugin', 'c',
         version: '0.1.0',
         license: 'Zlib',
         default_options:[
            'c_std=gnu11',
            'warning_level=2',
            'strip=true',
            'b_pie=true'
         ],
         meson_version: '>=0.52')

message ('Build configuration - @0@'.format(get_option('buildtype')))

dsp_ver = meson.project_version().split('.')
dsp_src = files('''
            ddb_tracer.c
        '''.split())
dsp_incs = include_directories('''
            .
        '''.split())
dsp_deps = []
dsp_cflags = '''
    -Wno-unused
'''.split()

cc = meson.get_compiler('c')

foreach flag : dsp_cflags
    if cc.has_argument(flag)
        add_project_arguments (flag, language: 'c')
    endif
endforeach

if not cc.has_header('deadbeef/deadbeef.h',
            include_directories: dsp_incs)
    error ('Don\'t found DeadBeeF headers')
endif

dsp_lib = shared_library('ddb_spy', dsp_src,
            include_directories: dsp_incs,
            dependencies: dsp_deps,
            install: true)
