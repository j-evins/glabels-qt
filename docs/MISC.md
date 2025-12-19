gLabels Branching Model
=======================

At any time gLabels will typically have 2 active branches: a stable branch and a development branch (master).  Other branches may also exist to work on major features.


Stable branch
-------------

A stable branch will maintained in a git branch named glabels-major.minor.
E.g. "glabels-4.0".  Some characteristics of a stable branch are

- Prerequisites are fixed for the lifetime of the branch.
- The primary purpose of releases within a branch is to fix bugs.
- Besides bug fixes, releases may also include new and updated translations,
  updated documentation, and new label templates.
- No new features will be introduced by a release within a
  stable branch.


Unstable/development branch
---------------------------

The current development branch is maintained in the Git master branch.
Some characteristics of the development branch are

- Prerequisites are volatile and may change over time.
- Features and file formats may be volatile and change over time.
- No attempt will be made to maintain backwards compatibility between commits
- Every effort will be made to make sure the development branch will build and run without error.
- There will be no official unstable releases based on this branch.  However, CI snapshots may be made available.

Major feature branches
----------------------

Major features that could risk breaking the development branch will be developed in their own branches.


gLabels Version numbering
=========================

* Versions are numbered major.minor.micro (e.g. 4.0.3)
* major version indicates a major change in architecture or technology
  - 2.x.x was based on gtk+-2
  - 3.x.x was based on gtk+-3
  - 4.x.x is based on qt-6
* minor version indicates a minor change in features or design
  - A new stable branch will be created for each new minor version
* micro version indicates a bugfix release
  - See above for release characteristics within a stable branch
 
