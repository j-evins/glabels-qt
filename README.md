![gLabels Label Designer](glabels/images/glabels-label-designer.png)

![Cover Image](docs/images/cover-image.png)

[![Multi-Platform Build Tests](https://github.com/j-evins/glabels-qt/actions/workflows/build-tests.yml/badge.svg?branch=master&event=push)](https://github.com/j-evins/glabels-qt/actions/workflows/build-tests.yml)

*******************************************************************************

## What is gLabels-qt?

gLabels-qt is the development version of the next major version of gLabels (a.k.a. glabels-4).


## What's new in gLabels 4?

- A complete rewrite, based on the Qt5 framework.
- A new UI layout based on common activities.
- Cross-platform support
- User-defined variables
- Support for continuous-roll labels
- Many new product templates


## Download

### Latest Release

There are currently no official releases of gLabels 4.

### Continuous Integration Snapshots

Continuous integration snapshots are not official releases.  These snapshots represent the latest
bleeding-edge development (unstable) code.  Please, DO NOT use it in a production environment.  Do not
expect compatibility or consistency of features between snapshots.

Pre-release Linux [AppImage](http:appimage.org) and Windows installer binaries are available in
**[Releases](https://github.com/j-evins/glabels-qt/releases)**.

Some third-party packages are also available:


| Platform  | Files                                                                                | Notes                                                         |
|:----------|:-------------------------------------------------------------------------------------|:--------------------------------------------------------------|
| Archlinux | [Archlinux User Repository Page](https://aur.archlinux.org/packages/glabels-qt-git/) | Maintained by [Mario Blättermann](https://github.com/mariobl) |
| Ubuntu    | [PPA Page](https://code.launchpad.net/~krisives/+archive/ubuntu/glabels-qt)          | Maintained by [Kristopher Ives](https://github.com/krisives)  |



## Build Instructions

- [Linux Build Instructions](docs/BUILD-INSTRUCTIONS-LINUX.md)
- [Windows Build Instructions](docs/BUILD-INSTRUCTIONS-WINDOWS.md)
- [Mac Build Instructions](docs/BUILD-INSTRUCTIONS-MACOS.md)


## Help Needed

Please see [docs/CONTRIBUTING.md](docs/CONTRIBUTING.md).


## License

gLabels-qt is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

gLabels-qt is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

See [LICENSE](LICENSE) in this directory.

The following sub-components are also made available under less
restrictive licensing:

### Glbarcode

   gLabels-qt currently includes a version of the glbarcode++ library, located in
   the "glbarcode/" subdirectory.  It is licensed under the GNU LESSER GENERAL
   PUBLIC LICENSE (LGPL); either version 3 of the License, or (at your option)
   any later version.  See [glbarcode/LICENSE](glbarcode/LICENSE).

### Template Database

   The XML files in the "templates/" subdirectory constitute the glabels
   label database.  No copyright is claimed on the facts contained within
   the database and can be used for any purpose.  The files themselves are
   licensed using the MIT/X license.  See [templates/LICENSE](templates/LICENSE).

