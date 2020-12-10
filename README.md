# OpenEDI

#### Description
An open-source infrastructure for VLSI design automation tool set.  

#### Software Architecture
Key components include:
Database - OpenEDI database, core module.
Parser - parse standard data format and import data to OpenEDI databade.
CommandManager - manage commands.
MessageManager - manage messages.
IOManager - High-performance file IO.
GUI - Graphic user interface

#### Building Dependency (Recommend)

1. [GCC 6.5.0](http://ftp.tsukuba.wide.ad.jp/software/gcc/releases/gcc-6.5.0/gcc-6.5.0.tar.gz)

2. [Cmake 3.12.4](https://cmake.org/files/v3.12/cmake-3.12.4-Linux-x86_64.tar.gz)

3. [Qt 5.9.7](http://download.qt.io/archive/qt/5.9/5.9.7/qt-opensource-linux-x64-5.9.7.run)

4. Python 3.6

   Recommend to use [anaconda](https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/Anaconda3-5.2.0-Linux-x86_64.sh)

5. [Google test](https://gitee.com/mirrors/googletest)

   - Integrated as a submodule

6. [Pybind11](https://gitee.com/mirrors/pybind11)

   - Integrated as a submodule

7. [Boost](https://www.boost.org) 1.69

   - Must be found by CMake
   - Recommend to use [anaconda](https://anaconda.org) to install the library by `conda install -c anaconda boost`
   - Then `export BOOST_ROOT=/path/to/anaconda` so that CMake can find boost
   - If Boost is installed with other approaches, you can export `BOOST_ROOT` to wherever it is installed

8. Bison 3.0.4+

9. Flex 2.5.37+

10. [Tcl 8.6.9](https://downloads.sourceforge.net/tcl/tcl8.6.9-src.tar.gz)

11. [Gperftools 2.7.90](https://github.com/gperftools/gperftools/releases/download/gperftools-2.7.90/gperftools-2.7.90.tar.gz)

#### Fast deployment(Virtual machine image)

- Download and uncompress CentOS7Dev.zip, then import CentOS7Dev.ovf and launch the virtual machine.

```
Download url: https://pan.baidu.com/s/1JnxVadThOJL3K2jsscRaGA
Verification code: moqr
Username:root
Password:toor
```

- Before checkout openedi code, Please source /opt/devtoolset/devtoolset.bash first.

#### Installation

1.  Clone the repository with submodules.
~~~~~~~~~~~~~~~~~~~~~
git clone --recursive https://gitee.com/niiceda/open-edi.git
~~~~~~~~~~~~~~~~~~~~~
Alternatively, you can clone the repository and the submodules in two steps.
~~~~~~~~~~~~~~~~~~~~~
git clone https://gitee.com/niiceda/open-edi.git
cd open-edi
git submodule update --init
~~~~~~~~~~~~~~~~~~~~~

2.  Build the program with cmake.
~~~~~~~~~~~~~~~~~~~~~
mkdir build   # assume you are inside the root directory of the repository
cd build
cmake ..      # you may add -DCMAKE_INSTALL_PREFIX=your/install/path to specify the location
make          # you may add -j to turn on parallel building
make test     # optional step to run unittests
make docs     # optional step to generate HTML documentations
make install
~~~~~~~~~~~~~~~~~~~~~

#### Instructions

1.  Once installed (after make install), you can find the 'openEDA' binary under `your/install/path/bin/` directory.

Invoke the binary, try
openEDA> read_lef test.lef
openEDA> read_verilog test.v
openEDA> read_def test.def
openEDA> read_timing_library test.lib

2. 

#### Contribution

1.  Fork the repository
2.  Create Feat_xxx branch
3.  Commit your code
4.  Create Pull Request


#### Features

1.  You can use Readme\_XXX.md to support different languages, such as Readme\_en.md, Readme\_zh.md
2.  Gitee blog [blog.gitee.com](https://blog.gitee.com)
3.  Explore open source project [https://gitee.com/explore](https://gitee.com/explore)
4.  The most valuable open source project [GVP](https://gitee.com/gvp)
5.  The manual of Gitee [https://gitee.com/help](https://gitee.com/help)
6.  The most popular members  [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
