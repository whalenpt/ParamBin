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
import glob


INC_DIR = "include"
INSTALL_DIR = "/usr/local/"
#INSTALL_DIR = os.path.expanduser("~")

def uninstall(lib_name,lib_ver):
    target_dir = os.path.join(INSTALL_DIR,"include")

    for fn in os.listdir(INC_DIR):
        if fn.endswith("h") or fn.endswith("hpp"):
            inc_file = os.path.join(target_dir,fn)
            try:
                os.remove(inc_file)
            except OSError:
                pass


    target_dir = os.path.join(INSTALL_DIR,"lib")
    dylib_symbolic_name = "lib"+lib_name+".dylib"
    target_symbolic_dest = os.path.join(target_dir,dylib_symbolic_name)

    try:
        os.unlink(target_symbolic_dest)
    except OSError:
        pass

    dylib_name = "lib"+lib_name+"."+lib_ver+".dylib"
    target_dest = os.path.join(target_dir,dylib_name)

    try:
        os.remove(target_dest)
    except OSError:
        pass

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("""uninstall.py requires 2 command line arguments:
             "Argument 1 - the library name.
             "Argument 2 - the version number""")
        print("EXITING!")
        sys.exit()
    lib_name = sys.argv[1]
    lib_ver = sys.argv[2]
    uninstall(lib_name,lib_ver)



