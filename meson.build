project(
    'Kei Editor',
    'cpp',
    default_options: ['cpp_std=c++23', 'warning_level=3'],
    version: '0.0.1',
    license: 'GPL-3.0-or-later',
    license_files: ['COPYING'],
)

deps = [
    dependency('jsoncpp',  fallback: ['jsoncpp', 'jsoncpp_dep']),
    dependency('sdl3',     fallback: ['SDL',     'sdl3_dep']),
    dependency('sdl3-ttf', fallback: ['SDL_ttf', 'sdl3_ttf_dep']),
    dependency('lyra',     fallback: ['Lyra',    'lyra_dep']),
]

args = [
    '-DAPP_VERSION="@0@"'.format(meson.project_version()),
    '-DAPP_NAME="@0@"'.format(meson.project_name()),
    '-DAPP_ID="org.kei.editor"',
]

subdir('src')
executable(
    'keditor',
    source_files,
    include_directories: include_directories('include'),
    install: true,
    cpp_args: args,
    dependencies: deps,
)