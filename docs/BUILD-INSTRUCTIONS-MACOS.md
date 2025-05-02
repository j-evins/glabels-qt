gLabels MacOS Build Instructions
================================

## Prerequisites

For Intel Macs
```
brew install cmake
brew install qt@5
```

For Apple Silicon Macs
```
brew install gcc@14
brew install cmake
brew install qt@5
```



## Compile and Install

<pre>
cd <i>glabels_source_directory</i>
mkdir build
cd build

# For Intel Macs
cmake -D CMAKE_PREFIX_PATH=/usr/local/opt/qt@5  ..

# For Apple Silicon Macs
cmake -D CMAKE_PREFIX_PATH="/opt/homebrew/opt/qt@5" -DCMAKE_C_COMPILER=/opt/homebrew/bin/gcc-14 -DCMAKE_CXX_COMPILER=/opt/homebrew/bin/g++-14  ..

make
sudo make install
</pre>
