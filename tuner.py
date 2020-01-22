from os import system, environ
from getopt import getopt
import sys
CC = ["g++", "icc"]
CC_FLAGS = {"g++" : "-fopenmp -mavx -std=c++11 -march=native -mtune=native",\
            "icc" : "-qopenmp -mavx -std=c++11 -march=native -mtune=native"}
CC_OPTIM_LEVEL = ["-O{}".format(i) for i in range(1, 4)]
app_name = "ttmatvec"
src_name = "ttmatvec.cpp ttmat.cpp ttvec.cpp"
versions = ["BASIC", "INDEXES", "SET_FIRST"]

options = sys.argv[2:]
options, _ = getopt(options, "v:c:O:")
for option in options:
  opt, val = option
  opt = opt[1] if len(opt) == 2 else opt[2:]
  if opt == "c":
    CC = val.split(",")
  elif opt == "v":
    versions = val.split(",")
  elif opt == "O":
    CC_OPTIM_LEVEL = val.split(",")
  else:
    print("Unkown option {}".format(opt))

if sys.argv[1] in ["compile", "all"]:
  # Compile with option
  for compiler in CC:
    flags = CC_FLAGS[compiler]
    for opt_level in CC_OPTIM_LEVEL:
      for version in versions:
        split_version = version.split(" ")
        definition = ""

        if not len(split_version) == 1:
          definition = " ".join(["-D{}".format(v) for v in split_version])
        else:definition = "-D{}".format(version)

        command = compiler + " " + flags + " " + opt_level + " " + definition
        command += " {} -o {}-{}{}-{} -lm".format(src_name, app_name, compiler, ''.join(opt_level.split(" ")), '-'.join(split_version)) 
        print(command)
        if system(command):
          print("Command \"{}\" has failed".format(command))

if sys.argv[1] in ["run", "all"]:
  print("compiler,version,optimization,time")
  for compiler in CC:
    for opt_level in CC_OPTIM_LEVEL:
      for version in versions:
        split_version = version.split(" ")

        command = "./" + app_name + "-" + compiler + ''.join(opt_level.split(" ")) + "-" + '-'.join(split_version)
        args = " -a A.bin -x x.bin -y y.bin"
        print("{},{},{},".format(compiler, "-".join(split_version), opt_level), end="", flush=True)
        awk  = "awk -F' ' '{print $3}'"
        if system(command + args + " | " + awk):
          print("Command \"{}\" has failed".format(command))

if sys.argv[1] in ["amdal"]:
  print("nthread,compiler,version,optimization,time")
  for i in [2*i for i in range(1, 11)]:
    environ["OMP_NUM_THREADS"]=str(i)
    for compiler in CC:
      for opt_level in CC_OPTIM_LEVEL:
        for version in versions:
          split_version = version.split(" ")
          command = "./" + app_name + "-" + compiler + opt_level + "-" + '-'.join(split_version)
          args = " -a A.bin -x x.bin -y y.bin"
          print("{},{},{},{},".format(i, compiler, ''.join(opt_level.split(" ")), version), end="", flush=True)
          awk  = "awk -F' ' '{print $3}'"
          if system(command + args + " | " + awk):
            print("Command \"{}\" has failed".format(command))
