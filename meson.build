# In this directory, execute 'meson build/' once
# After that: 'ninja test -C build/'
# meson build/ --reconfigure --buildtype=debug
# meson build/ --reconfigure --buildtype=debugoptimized
# meson build/ --reconfigure --buildtype=release

project('brickwork', 'cpp',
        default_options: ['cpp_std=c++2a', 'warning_level=3'],
        version: '0.3.0',
        license: 'GPL3')

brickwork_include = include_directories('include')

subdir('src')
