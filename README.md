# OpenEDI

#### Description
An open-source electronic data infrastructure for VLSI design automation. 

#### Software Architecture
Software architecture description

#### Building Dependency (Recommend)

1. GCC 6.5.0

2. Cmake 3.12.4

3. Qt 5.9.7

4. Python 3.6

   Recommend to use [anaconda](https://anaconda.org)

5. [Google test](https://gitee.com/mirrors/googletest)

   - Integrated as a submodule 

6. [Pybind11](https://gitee.com/mirrors/pybind11)

   - Integrated as a submodule 

7. [Boost](https://www.boost.org) 1.69

   - Must be found by CMake 
   - Recommend to use [anaconda](https://anaconda.org) to install the library by `conda install -c anaconda boost`
   - Then `export BOOST_ROOT=/path/to/anaconda` so that CMake can find boost 
   - If Boost is installed with other approaches, you can export `BOOST_ROOT` to wherever it is installed

8. Bison 3.0.4

9. Flex 2.5.37

10. Tcl 8.6.9

11. Gperftools 2.7.90

#### Fast deployment(Virtual machine image)

- Download and uncompress CentOS7Dev.zip, then import CentOS7Dev.ovf and launch the virtual machine.

```
Download url: https://pan.baidu.com/s/1K7JNgm6UtjpXOc4fUZG62A 
Verification code: u6p7 
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
