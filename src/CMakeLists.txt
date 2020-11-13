# utility 
add_subdirectory(util)
# geometry 
add_subdirectory(geo)
# core database 
add_subdirectory(db)
# iterators 
#add_subdirectory(iter)
# python binding 
if(EDI_BIND_PYTHON)
  add_subdirectory(python)
endif(EDI_BIND_PYTHON)
# tcl binding 
#if(EDI_BIND_TCL)
  add_subdirectory(tcl)
#endif(EDI_BIND_TCL)
# data in/out
add_subdirectory(parser)
# gui
add_subdirectory(gui)
# place
add_subdirectory(DREAMPlace)
# app main
add_subdirectory(app)
