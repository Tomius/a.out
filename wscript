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
    cfg.env.append_value('CXXFLAGS', cfg.filter_flags([
        '-stdlib=libc++', '-fcolor-diagnostics', '-Wall', '-Wextra',
        '-Wno-unused-parameter', '-Wno-dangling-else', '-pedantic', '-Wdocumentation']))
    cfg.env.append_value('LINKFLAGS', cfg.filter_flags(['-stdlib=libc++']))

    if cfg.options.debug:
        cfg.define('DEBUG', True)
    else:
        cfg.define('NDEBUG', True)

    cfg.check_boost()
    cfg.check_cfg(package='glfw3', args='--cflags --libs')
    cfg.check_cxx(header_name='glbinding/Binding.h', lib='glbinding', uselib_store='GLBINDING')

    os = cfg.env.DEST_OS
    cfg.define(os.upper(), 1)
    if (os in ['cygwin', 'msys', 'win32', 'win64', 'wine']):
        cfg.define('WINDOWS', 1)
    if os in ['cygwin', 'wine', 'linux', 'gnu', 'freebsd', 'netbsd', 'openbsd',
              'sunos', 'hpux', 'irix', 'sgi' 'aix']:
        cfg.define('POSIX', 1)

    cfg.write_config_header('src/config.h')

def build(bld):
    src = [
        'src/main.cpp',
        'src/video/window.cpp',
    ]

    x = VersionTask(env=bld.env)
    x.set_outputs(bld.path.find_or_declare('src/version.h'))
    bld.add_to_group(x)

    x = bld(features = 'cxx cxxprogram',
            source   = src,
            includes = 'src',
            uselib   = 'BOOST GLFW3 GLBINDING',
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
