from setuptools import setup, find_packages, distutils
from setuptools.command.build_py import build_py as BaseBuild
import os, subprocess

class FBBuild(BaseBuild):
    def run(self):
        lib_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'flamebroiler')
        if subprocess.Popen("cd %s && make" % lib_path, shell=True).wait() != 0:
            raise distutils.errors.CompileError()
        BaseBuild.run(self)

f = open(os.path.join(os.path.dirname(__file__), 'README.md'))
readme = f.read()
f.close()

setup(
    name='flamebroiler',
    version=0.1,
    description='A hybrid-trie string matching C extension.',
    long_description=readme,
    author='Andrew Pendleton',
    author_email='apendleton@sunlightfoundation.com',
    url='http://github.com/apendleton/flamebroiler/',
    packages=find_packages(),
    license='BSD License',
    platforms=["any"],
    py_modules=["flamebroiler"],
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: BSD License',
        'Natural Language :: English',
        'Operating System :: OS Independent',
        'Programming Language :: Python',
        'Environment :: Web Environment',
    ],
    install_requires=["cffi"],
    include_package_data=True,
    package_data={'': ['*.so','*.dylib']},

    cmdclass={'build_py': FBBuild}
)
