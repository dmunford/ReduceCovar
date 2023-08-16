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

# Input Example 10x10
|   | 0 | 1 | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  |
| - | - | - | -- | -- | -- | -- | -- | -- | -- | -- |
| 0 | 0 | 0 | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  |
| 1 | 0 | 1 | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  |
| 2 | 0 | 2 | 4  | 6  | 8  | 10 | 12 | 14 | 16 | 18 |
| 3 | 0 | 3 | 6  | 9  | 12 | 15 | 18 | 21 | 24 | 27 |
| 4 | 0 | 4 | 8  | 12 | 16 | 20 | 24 | 28 | 32 | 36 |
| 5 | 0 | 5 | 10 | 15 | 20 | 25 | 30 | 35 | 40 | 45 |
| 6 | 0 | 6 | 12 | 18 | 24 | 30 | 36 | 42 | 48 | 54 |
| 7 | 0 | 7 | 14 | 21 | 28 | 35 | 42 | 49 | 56 | 63 |
| 8 | 0 | 8 | 16 | 24 | 32 | 40 | 48 | 56 | 64 | 72 |
| 9 | 0 | 9 | 18 | 27 | 36 | 45 | 54 | 63 | 72 | 81 |

# Output Example 5x5
|   | 0  | 1  | 2  | 3  | 4  |
| - | -- | -- | -- | -- | -- |
| 0 | 81 | 63 | 54 | 72 | 81 |
| 1 | 63 | 49 | 42 | 56 | 63 |
| 2 | 54 | 42 | 36 | 48 | 54 |
| 3 | 72 | 56 | 48 | 64 | 72 |
| 4 | 81 | 63 | 54 | 72 | 81 |