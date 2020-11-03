# OpenEDI

#### Description
An open-source electronic data infrastructure for VLSI design automation. 

#### Software Architecture
Software architecture description

#### Software Dependency

1.  [Google test](https://gitee.com/mirrors/googletest)
    - Integrated as a submodule 
2.  [Pybind11](https://gitee.com/mirrors/pybind11)
    - Integrated as a submodule 
3.  [Boost](https://www.boost.org)
    - Must be found by CMake 
    - Recommend to use [anaconda](https://anaconda.org) to install the library by `conda install -c anaconda boost`
    - Then `export BOOST_ROOT=/path/to/anaconda` so that CMake can find boost 
    - If Boost is installed with other approaches, you can export `BOOST_ROOT` to wherever it is installed

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

1.  Once installed (after make install), you can find the `.so` under `your/install/path/lib/` directory. 
You can go to the install directory and start python to try it out. 
Suppose you have a following python script called `test.py`. 

~~~~~~~~~~~~~~~~~~~~~
from lib import openedi as edi 
# access the enum types 
signal_direct = edi.SignalDirection.kInput 
print(signal_direct)
# access the print functions 
edi.ediPrint(edi.MessageType.kInfo, "test message type %s\n" % (edi.MessageType.kInfo))
~~~~~~~~~~~~~~~~~~~~~
Invoking python3 by (assume CMake finds python3 during compilation)

~~~~~~~~~~~~~~~~~~~~~
python3 test.py
~~~~~~~~~~~~~~~~~~~~~
will execute the script and print following messages 

~~~~~~~~~~~~~~~~~~~~~
SignalDirection.kInput
[INFO   ] test message type MessageType.kInfo
~~~~~~~~~~~~~~~~~~~~~

2.  xxxx 

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
