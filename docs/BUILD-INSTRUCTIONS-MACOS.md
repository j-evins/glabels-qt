gLabels MacOS Build Instructions
================================

## Prerequisites
```
brew install cmake
brew install qt@6
```

## Optional dependencies
```
brew install zlib
brew install qrencode
brew install zint
```

## Compile and Install

<pre>
export SDKROOT=$(xcrun --show-sdk-path)
# Detect architecture and set appropriate paths
if [[ "$(uname -m)" == "arm64" ]]; then
    # Apple Silicon
    CMAKE_PATH="/opt/homebrew/bin/cmake"
    CMAKE_PREFIX_PATH="/opt/homebrew/opt"
else
    # Intel
    CMAKE_PATH="/usr/local/bin/cmake"
    CMAKE_PREFIX_PATH="/usr/local/opt"
fi

cd <i>glabels_source_directory</i>
mkdir build
cd build
$CMAKE_PATH -DCMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH/qt@6 ..
make
sudo make install
</pre>
