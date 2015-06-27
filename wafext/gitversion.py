#! /usr/bin/env python
# -*- mode: python; encoding: utf-8 -*-

# VERY SIMPLE python header to determine git revison for waf build system and
# also providing a fallback if git was not found, or the user doesn't use the
# git repo (downloaded tarball, for example)

# Copyright © 2008-2011 Kővágó Zoltán <DirtY.iCE.hu@gmail.com>
# This piece of python code is free software. It comes without any warranty, to
# the extent permitted by applicable law. You can redistribute it and/or modify
# it under the terms of the Do What The Fuck You Want To Public License, Version
# 2, as published by Sam Hocevar. See http://sam.zoy.org/wtfpl/COPYING for more
# details.

# Usage:
# 1. Copy this file (`gitversion.py') to the root directory of project, where
#    the root wscript lives
# 2. Edit `wscript':
#    First remove `VERSION=...' from the wscript
#    Then add `from gitversion import VERSION' (outside functions). If you need
#    the git branch too, use `from gitversion import VERSION, BRANCH'

import os

HAVE_GIT=os.system("git rev-parse --git-dir >/dev/null 2>/dev/null") == 0
if (HAVE_GIT):
    VERSION = os.popen("git describe --dirty --always --tags --candidates=1 "
                       "2>/dev/null").readline().strip('\n')
    BRANCH = os.popen("git branch | grep -e '^\*' | sed 's/^\* //'"). \
        readline().strip('\n')
else:
    f = open("RELEASE", "r")
    VERSION = f.readline().strip()
    BRANCH = f.readline().strip()
