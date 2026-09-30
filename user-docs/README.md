gLabels User Documentation
==========================

This directory contains source files for generating man pages and the glabels user's manual.
The `_build` directory contains pre-built versions of these documents.  These pre-built
documents are not normally rebuilt as part of the glabels build process.  They do not need to
be re-built unless there are changes to the `.rst` source files.


Prerequisites
-------------

Rebuilding user documentation has the following dependencies:

- sphinx-build
- qthelp sphinx builder
- qhelpgenerator

Pre-build Man pages and user documentation is normally must be rebuilt manually after modifying rst
files.  Requires sphinx-build installed on system.


Rebuilding
----------

Rebuilding is done from the project binary directory (usually `glabels-qt/build`).

### Man pages
```
$ make manpages
```

### User manual
```
$ make qthelp-manual
```

### Both man pages and user manual
```
$ make user-docs
```

Simple HTML Pages
-----------------

Optionally, you can build an HTML version of the user manual, which is suitable to peruse
using a web browser.  The HTML version is not currently installed or distributed in
a pre-built form.
```
$ make html-manual
```


Notes for Ubuntu 24.04
----------------------

### To install sphinx-build
```
$ sudo apt install python3-sphinx
```

### To install qthelp sphinx builder
```
$ sudo apt install python3-sphinxcontrib.qthelp
```

### To install qhelpgenerator
```
$ sudo apt install qt6-documentation-tools
```

### To make sure qtchooser defaults to qt6 apps
```
$ export QT_SELECT=qt6
```
