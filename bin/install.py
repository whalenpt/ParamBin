#!/usr/bin/env python

"""--------------------------------------------------------------------------
   
    Author: "Patrick Whalen"
    Email: "pwhalen@email.arizona.edu"
    Status: "Development"
    Date: "(MM/DD/YY)"
    Description: (Enter Program Description)

--------------------------------------------------------------------------"""

# IMPORTS

import os
import sys
import shutil

INC_DIR = "include"
INSTALL_DIR = "/usr/local/"
#INSTALL_DIR = os.path.expanduser("~")

def install(lib_name,lib_ver):

    dylib_name = "lib"+lib_name+"."+lib_ver+".dylib"
    if not os.path.isfile("./"+dylib_name):
        print("install.py could not find the dynamic library {}".format(dylib_name))
        print("EXITING!")
        sys.exit()

    bin_file = os.path.join(INC_DIR,"parambin.hpp")
    if not os.path.isfile(bin_file):
        print("install.py could not find the header file {}".format(bin_file))
        print("EXITING!")
        sys.exit()
    includes = ["parambin.hpp"]

    target_dir = os.path.join(INSTALL_DIR,"lib")
    os.makedirs(target_dir,exist_ok=True)

    # shuthil.copyfile overwrites target_dest if it exists
    target_dest = os.path.join(target_dir,dylib_name)
    shutil.copyfile(dylib_name,target_dest)

    # os.symlink throws a FileExistsError exception if the link already exists 
    dylib_symbolic_name = "lib"+lib_name+".dylib"
    target_symbolic_dest = os.path.join(target_dir,dylib_symbolic_name)
    try:
        os.symlink(target_dest,target_symbolic_dest)
    except FileExistsError:
        os.unlink(target_symbolic_dest)
        os.symlink(target_dest,target_symbolic_dest)

    target_dir = os.path.join(INSTALL_DIR,"include")
    os.makedirs(target_dir,exist_ok=True)

#    src_loc = os.path.join(INC_DIR,"parambin.hpp")
#    target_dest = os.path.join(target_dir,"parambin.hpp")
#    shutil.copyfile(src_loc,target_dest)

    for fn in includes:
        src_loc = os.path.join(INC_DIR,fn)
        target_dest = os.path.join(target_dir,fn)
        shutil.copyfile(src_loc,target_dest)

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("""install.py requires 2 command line arguments:
             "Argument 1 - the library name.
             "Argument 2 - the version number""")
        print("EXITING!")
        sys.exit()
    lib_name = sys.argv[1]
    lib_ver = sys.argv[2]
    install(lib_name,lib_ver)








