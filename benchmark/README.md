First create the benchmark files:
```bash
python3 create_benchmark.py bench/
```
_Note: you can check the help of the script for additional options._

Then generate the build files:
```bash
cmake -S bench/ -B bench/build -DCMAKE_BUILD_TYPE=Debug
```

Then build with a tool that helps you time the build, like the linux command `time`:
```bash
time cmake --build bench/build -j
```
