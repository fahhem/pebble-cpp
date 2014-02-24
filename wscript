
#
# This file is the default set of rules to compile a Pebble project.
#
# Feel free to customize this to your needs.
#

top = '.'
out = 'build'

def options(ctx):
    ctx.load('pebble_sdk')
    ctx.load('g++')

def configure(ctx):
    ctx.load('pebble_sdk')

    CROSS_COMPILE_PREFIX = 'arm-none-eabi-'
    ctx.env.CXX = CROSS_COMPILE_PREFIX+'g++'
    ctx.load('g++')

    ctx.env.CXXFLAGS = list(ctx.env.CFLAGS)
    ctx.env.CXXFLAGS.remove('-std=c99')
    sdk_folder = ctx.root.find_dir(ctx.env['PEBBLE_SDK'])
    ctx.env.CXXFLAGS.extend(['-std=c++11', '-fPIE', '-fno-unwind-tables', '-fno-exceptions'])

def build(ctx):
    ctx.load('pebble_sdk')
    ctx.pbl_program(source=ctx.path.ant_glob(['src/**/*.cpp']),
                    target='pebble-app.elf')

    ctx.pbl_bundle(elf='pebble-app.elf',
                   js=ctx.path.ant_glob('src/js/**/*.js'))

