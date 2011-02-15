# platform specific customizations
# 2011-02-13, Davide Bacchet (davide.bacchet@gmail.com)
# $LastChangedDate$
# $LastChangedBy$


import sys

def customizeAppleLinker(env):
    env.Append(LINKFLAGS=['-arch','i386'])
    # in osx rpath refers to install_name when used in shared libs, 
    # and to runtime search path when used in executables
    env.Append(SHLINKFLAGS=['$_SHLIBRPATH'])
    env['SHLIBRPATHPREFIX'] = '-install_name '
    env['SHLIBRPATHSUFFIX'] = ''
    env['_SHLIBRPATH'] = '${_concat(SHLIBRPATHPREFIX, RPATH, SHLIBRPATHSUFFIX, __env__)}'
    env.Append(LINKFLAGS=['$__RPATH'])
    env['RPATHPREFIX'] = '-Wl,-rpath,'
    env['RPATHSUFFIX'] = ''
    env['_RPATH'] = '${_concat(RPATHPREFIX, RPATH, RPATHSUFFIX, __env__)}'


def customizeEnvironment(env):
    """customize the build evironment"""
    if sys.platform=='darwin':
        customizeAppleLinker(env)

