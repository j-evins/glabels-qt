gLabels Linux Build Instructions
================================

## General
### Prerequisites

- g++
- CMake 2.8.12+
- Qt5 5.15+ Development Packages ( Qt5Core, Qt5Widgets, Qt5PrintSupport, Qt5Xml, Qt5Svg )
- zlib 1.2+ Development Package

> Even if the above library packages are installed, their corresponding development packages
> may also need to be installed.  Development packages are usually named something like
> libraryName-dev or libraryName-devel.

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

## Example: Ubuntu 19.04

### Installing Prerequisites
```
sudo apt install cmake
sudo apt install qtbase5-dev libqt5svg5-dev qttools5-dev zlib1g-dev
```
_QREncode (Optional)_
```
sudo apt install pkgconf libqrencode-dev
```
_Zint (Optional)_

Install zint from source:
```
wget https://downloads.sourceforge.net/project/zint/zint/2.6.3/zint-2.6.3_final.tar.gz
tar xzf zint-2.6.3_final.tar.gz
cd zint-2.6.3.src/
mkdir build && cd build && cmake .. && make
sudo make install
```
_GNU Barcode (Optional)_

As of version 0.99, GNU Barcode no longer installs its library.  So install 0.98 from source:
```
wget https://ftp.gnu.org/gnu/barcode/barcode-0.98.tar.gz
tar xzf barcode-0.98.tar.gz
cd barcode-0.98/
./configure && make
sudo make install
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
Fedora has a different package naming scheme that Ubuntu. This is to distinguish the QT5
packages from the QT3 and QT4 packages that they still support for compatibility.
If the Cmake pass or build has missing package errors or warnings, you can search for the needed
package with:
```
$ sudo dnf search qt5 |grep <package name substring>
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