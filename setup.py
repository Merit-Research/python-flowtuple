import os
import subprocess
from distutils.cmd import Command
from distutils.core import Extension, setup

os.putenv('LC_CTYPE', 'en_US.UTF-8')

pyflowtuple_version = '0.1'

cflags = ['-Wall', '-Wextra', '-Werror',
          '-Wno-unused-parameter', '-Wno-incompatible-pointer-types',
          '-Wno-cast-function-type', '-std=c99', '-D_FILE_OFFSET_BITS=64']

flowtuple = Extension('flowtuple',
                    libraries=['flowtuple'],
                    extra_compile_args=cflags + ['-DVERSION="%s"' % pyflowtuple_version],
                    language='C',
                    sources=[
                        'src/flowtuple.c',
                        'src/handle.c',
                        'src/header.c',
                        'src/trailer.c',
                        'src/interval.c',
                        'src/class.c',
                        'src/data.c',
                    ],
                    depends=[
                        'src/handle.h',
                        'src/header.h',
                        'src/trailer.h',
                        'src/interval.h',
                        'src/class.h',
                        'src/data.h',
                    ])


class TestCommand(Command):
    user_options = []

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        raise SystemExit(
            subprocess.call(['nosetests', 'tests'])
        )


setup(name='flowtuple',
      description='libflowtuple bindings for Python 3',
      author='Mark Weiman',
      author_email='mweiman@merit.edu',
      url='https://merit.edu',  # this needs to change
      ext_modules=[flowtuple],
      cmdclass={
          'test': TestCommand,
      })
