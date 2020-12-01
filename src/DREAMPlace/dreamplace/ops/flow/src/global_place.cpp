/*************************************************************************
    > File Name: global_place.h
    > Author: Chenzhong Luo
    > Date: Nov 2020
 ************************************************************************/

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <codecvt>
#include <locale>
#include <unistd.h>
#include <libgen.h>
#include "flow/src/global_place.h"
#include "utility/src/Msg.h"

DREAMPLACE_BEGIN_NAMESPACE

int GlobalPlace::run()
{

    if (!isDBReady()) {
        dreamplacePrint(kERROR, "DB not ready, quit.\n");
        return -1;
    }
    //well a tricky way to get path of target python module to start with. 
    std::wstring modulePath;
    char dest[PATH_MAX];
    memset(dest,0,sizeof(dest)); // readlink does not null terminate!
    if (readlink("/proc/self/exe", dest, PATH_MAX) == -1)
    {
        perror("readlink");
        return -1;
    }
    else
    {
        char* pathCopy = strdup(dest);
        std::string temp(dirname(pathCopy));
        modulePath = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t >().from_bytes(temp);
        std::wstring temp1(L"/../dreamplace");
        modulePath.append(temp1);
    }
    int rtn_value = 0;
    std::string json_file; // = getPara()->getJsonFile();
    //const std::wstring kModulePath(L"/usr/local/dreamplace");
    std::wstring kSitePaths(L"/home/opt/Anaconda3-5.2.0-Linux-x86_64/lib/python3.6/site-packages");
    std::wstring kSitePathsOrig(kSitePaths);
    const std::wstring kLib64(L"lib64");
    const std::wstring kLib(L"lib");
    const std::string kModuleName("GlobalPlaceRun");
    const std::string kFunctionName("global_placer_run");

    //sometimes site-packages have both lib/ & lib64/, while python_sitelib may contain only one.
    if (kSitePaths.find(kLib64) != std::string::npos)
    {
        kSitePaths.replace(kSitePaths.find(kLib64), kLib64.length(), kLib);
    }
    else if (kSitePaths.find(kLib) != std::string::npos)
    {
        kSitePaths.replace(kSitePaths.find(kLib), kLib.length(), kLib64);
    }

    //Py_IsolatedFlag = 0;
    //Py_NoSiteFlag = 0;
    //Py_NoUserSiteDirectory = 0;

    Py_Initialize();

    //update sys.path, somehow getpath() does not contain site_packages.
    std::wstring paths(Py_GetPath());
    paths.push_back(':');
    paths.append(kSitePaths);
    paths.push_back(':');
    paths.append(kSitePathsOrig);
    paths.push_back(':');
    paths.append(modulePath);
    PySys_SetPath(paths.c_str());

    PyObject *module_name = PyUnicode_FromString(kModuleName.c_str());
    PyObject *module = PyImport_Import(module_name);
    Py_XDECREF(module_name);
    if (module != NULL)
    {
        PyObject *func = PyObject_GetAttrString(module, kFunctionName.c_str());
        if (func && PyCallable_Check(func))
        {
            PyObject *value = PyObject_CallFunction(func, "s", json_file.c_str());
            Py_XDECREF(value);
            Py_DECREF(func);
        }         
        else
        {
            if (PyErr_Occurred())
            {
                PyErr_Print();
            }
            dreamplacePrint(kERROR, "Cannot find function: %s.\n", kFunctionName.c_str());
            rtn_value = -1;
        }
        Py_DECREF(module); 
    }
    else
    {
        PyErr_Print();
        dreamplacePrint(kERROR, "Failed to load module: %s.\n", kModuleName.c_str());
        rtn_value = -1;
    }
    Py_Finalize();

    return rtn_value;
}

DREAMPLACE_END_NAMESPACE

