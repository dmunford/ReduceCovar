## Required packages:
- ROOT6.xx
- jsoncpp-devel

After installing required packages run the following to create the executable.
```bash
g++ ReduceCovar.cxx -o ReduceCovar.exe `root-config --cflags --glibs` -ljsoncpp -O3
```

CALLING SEQUENCE:
```bash
    ReduceCovar.exe -v 1 -f /path/to/configname
```

Example Config:
```json
{
    "inputfile": "/home/dmunford/t2k/cc1pi/cov_test.root",
    "covarname": "cov_test",
    "ignorebins": [0,1,5],
    "combinebins": [[2,7], [3,4]]
}
```