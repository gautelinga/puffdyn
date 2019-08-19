from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
import setuptools
import sys

__version__ = '0.0.1'


class get_pybind_include(object):
    def __init__(self, user=False):
        self.user = user

    def __str__(self):
        import pybind11
        return pybind11.get_include(self.user)


ext_modules = [
    Extension(
        '_puffdyn',
        ['src/Node.cpp',
         'src/Queue.cpp',
         'src/Parameters.cpp',
         'src/io.cpp',
         'src/pybind11.cpp',
         ],
        include_dirs=[
            get_pybind_include(),
            get_pybind_include(user=True),
        ],
        language='c++',
        libraries=[],
    ),
]


def has_flag(compiler, flagname):
    import tempfile
    with tempfile.NamedTemporaryFile('w', suffix='.cpp') as f:
        f.write('int main (int argc, char **argv) { return 0; }')
        try:
            compiler.compile([f.name], extra_postargs=[flagname])
        except setuptools.distutils.errors.CompileError:
            return False
    return True


def cpp_flag(compiler):
    flags = ['-std=c++17', '-std=c++14', '-std=c++11']

    for flag in flags:
        if has_flag(compiler, flag):
            return flag

    raise RuntimeError('Unsupported compiler.')


class BuildExt(build_ext):
    """A custom build extension for adding compiler-specific options."""
    c_opts = {
        'msvc': ['/EHsc'],
        'unix': [],
    }

    if sys.platform == 'darwin':
        c_opts['unix'] += ['-stdlib=libc++', '-mmacosx-version-min=10.7']

    def build_extensions(self):
        ct = self.compiler.compiler_type
        opts = self.c_opts.get(ct, [])
        if ct == 'unix':
            opts.append("-DVERSION_INFO=\"{}\"".format(
                self.distribution.get_version()))
            opts.append(cpp_flag(self.compiler))
            if has_flag(self.compiler, "-fvisibility=hidden"):
                opts.append("-fvisibility=hidden")
            opts.append("-g0")
            opts.append("-O3")
            opts.append("-ffast-math")
            opts.append("-lstdc++")
        elif ct == 'msvc':
            opts.append("/DVERSION_INFO=\\\"{}\\\"".format(
                self.distribution.get_version()))
        for ext in self.extensions:
            ext.extra_compile_args = opts
        build_ext.build_extensions(self)


setup(
    name="puffdyn",
    packages=find_packages(),
    version=__version__,
    author="Gaute Linga",
    author_email="gaute.linga@mn.uio.no",
    url="https://github.com/gautelinga/puffdyn",
    description="Simulating puff-puff dynamics",
    long_description="",
    ext_modules=ext_modules,
    install_requires=["pybind11>=2.3"],
    setup_requires=["pybind11>=2.3"],
    python_requires=">=3",
    cmdclass={'build_ext': BuildExt},
    zip_safe=False,
    )
