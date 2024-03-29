# -*- mode: python; encoding: utf-8 -*-

import sys
sys.path.insert(0, 'wafext')

APPNAME='a.out'
from gitversion import VERSION, BRANCH

# these variables are mandatory ('/' are converted automatically)
top = '.'
out = 'build'

def options(opt):
    opt.load('compiler_cxx boost')
    opt.add_option('--debug', action='store_true', default=False)

def configure(cfg):
    cfg.load('compiler_cxx boost clang_compilation_database')

    cfg.check_cxx(cxxflags='-std=c++14')
    cfg.env.append_value('CXXFLAGS', ['-std=c++14'])
    cfg.env.append_value('LINKFLAGS', ['-std=c++14'])
    cfg.env.append_value('CXXFLAGS', cfg.filter_flags([
        '-fcolor-diagnostics', '-Wall', '-Wextra', '-Wno-unused-parameter',
        '-Wno-dangling-else', '-pedantic', '-Wdocumentation']))

    cfg.define('GLM_FORCE_RADIANS', True)

    if cfg.options.debug:
        cfg.define('DEBUG', True)
    else:
        cfg.define('NDEBUG', True)

    cfg.check_boost(lib='iostreams')
    cfg.check_cfg(package='glfw3', args='--cflags --libs')
    cfg.check_cxx(header_name='glm/vec4.hpp')

    cfg.recurse('aliens')

    os = cfg.env.DEST_OS
    cfg.define(os.upper(), 1)
    if (os in ['cygwin', 'msys', 'win32', 'win64', 'wine']):
        cfg.define('WINDOWS', 1)
    if os in ['cygwin', 'wine', 'linux', 'gnu', 'freebsd', 'netbsd', 'openbsd',
              'sunos', 'hpux', 'irix', 'sgi' 'aix']:
        cfg.define('POSIX', 1)

    cfg.write_config_header('src/config.h')

def build(bld):
    bld.recurse('aliens')

    src = [
        'src/main.cpp',
        'src/gl/shader.cpp',
        'src/gl/texture.cpp',
        'src/game_objects/scene.cpp',
        'src/gfx/simple_bitmap_font.cpp',
        'src/logger/cmdline.cpp',
        'src/logger/logger.cpp',
        'src/logger/putter.cpp',
        'src/misc/exception.cpp',
        'src/misc/optiongrouper.cpp',
        'src/video/camera.cpp',
        'src/video/window.cpp',
        'src/physics/bounding_box.cpp',
        'src/physics/bounding_circle.cpp',
        'src/physics/collision.cpp',
        'src/physics/rigidbody.cpp',
        'src/physics/manifold.cpp',
        'src/physics/physics_scene.cpp',
        'src/physics/r4k.cpp',
    ]

    x = VersionTask(env=bld.env)
    x.set_outputs(bld.path.find_or_declare('src/version.h'))
    bld.add_to_group(x)

    x = bld(features = 'cxx cxxprogram',
            source   = src,
            includes = 'src',
            uselib   = 'BOOST GLFW3',
            use      = 'glbinding',
            target   = APPNAME)

from waflib.Task import Task
class VersionTask(Task):
    color = 'PINK'

    def run(self):
        str = """
/* Auto generated stuff, do not edit */
#ifndef INCLUDE_VERSION_H
#define INCLUDE_VERSION_H
#pragma once

#define VERSION "%s"
#define BRANCH "%s"
%s

#endif
""" % (VERSION, BRANCH, '' if BRANCH == '' else '#define HAS_BRANCH 1')

        self.outputs[0].write(str)

    def sig_vars(self):
        Task.sig_vars(self)
        upd = self.m.update
        upd(VERSION.encode())
        upd(BRANCH.encode())

        return self.m.digest()

from waflib.Configure import conf

@conf
def filter_flags(cfg, flags):
    ret = []

    for flag in flags:
        try:
            cfg.check_cxx(cxxflags=flag)
            ret.append(flag)
        except:
            pass

    return ret
