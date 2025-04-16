gLabels MacOS Build Instructions
================================

## Prerequisites

```
brew install cmake
brew install qt@5
```

## Compile and Install

<pre>
cd <i>glabels_source_directory</i>
mkdir build
cd build
cmake -D CMAKE_PREFIX_PATH=/usr/local/opt/qt  ..
make
sudo make install
</pre>
