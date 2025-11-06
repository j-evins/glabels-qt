gLabels Linux Build Instructions
================================

## General
### Prerequisites

- g++
- CMake 3.22+
- Qt6 6.2+ Development Packages ( Qt6Core, Qt6Widgets, Qt6PrintSupport, Qt6Xml, Qt6Svg, Qt6Test )


> Even if the above library packages are installed, their corresponding development packages
> may also need to be installed.  Development packages are usually named something like
> libraryName-dev or libraryName-devel.  Also, package names can vary significantly between
> distributions as well as between versions of the same distribution.  Finding the exact package
> names for your particular distribution, may require some trial-and-error and googling.

#### Optional Dependencies

- zlib 1.2+ Development Package
- gnu barcode 0.98+ (Distributions do not always install the `barcode.h` file needed for external projects)
- libqrencode 3.4= Development Package
- libzint 2.15+


### Compile and Install

gLabels uses the CMake meta build system.  Use the following commands to build and install gLabels:

<pre>
$ cd <i>glabels_source_directory</i>
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
</pre>


## Example: Ubuntu 24.04

### Installing Prerequisites
```
$ sudo apt install cmake
$ sudo apt install qt6-base-dev qt6-svg-dev qt6-tools-dev
```

_ZLib (Optional)_
```
$ sudo apt install zlib1g-dev
```

_QREncode (Optional)_
```
$ sudo apt install pkgconf libqrencode-dev
```
_Zint (Optional)_

Install zint from source:
```
$ wget https://downloads.sourceforge.net/project/zint/zint/2.15.0/zint-2.15.0-src.tar.gz
$ tar xzf zint-2.15.0-src.tar.gz
$ cd zint-2.15.0-src/
$ mkdir build && cd build && cmake .. && make
$ sudo make install
$ sudo ldconfig
```

_GNU Barcode (Optional)_

```
$ sudo apt install barcode
```

### Compile and Install gLabels

```
$ cd glabels-qt
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
```


## Example: Fedora 35

### Installing Prerequisites
We assume the build system already has things like cmake and the GNU C++ suite installed.

```
$ sudo dnf install qt5-qtbase-devel qt5-qtsvg-devel qt5-linguist qt5-qttools
```
These installs will pull in additional packages to fill out their prerequisites.
Fedora has a different package naming scheme than Ubuntu. This is to distinguish the QT6
packages from the QT3 and QT4 packages that they still support for compatibility.
If the Cmake pass or build has missing package errors or warnings, you can search for the needed
package with:
```
$ sudo dnf search qt6 |grep <package name substring>
```

### Compile and Install gLabels into /usr/local
```
$ cd glabels-qt
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install

```
