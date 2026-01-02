glabels-batch-qt
================

SYNOPSIS
--------

**glabels-batch-qt** [*options*] *FILE*

DESCRIPTION
-----------

:program:`glabels-batch-qt` is a utility for printing a glabels project file
from the command line.  This command takes exactly one project file, FILE.  If FILE
is "-", it expects an XML glabels project to be provided on standard input.


OPTIONS
-------

.. program:: glabels-batch-qt

.. option::  -h, --help

	     Displays usage information and exits.

.. option::  -v, --version

             Displays brief version information and exits.

.. option::  -V, --Version

             Displays detailed version information and exits.

.. option:: -p <printer>, --printer <printer>

	    Send output to <printer>.

.. option::  -i <source>, --input <source>

	     Set merge input to <source> (typically a filename). If source is "-", read from standard input.
	     This option overrides the merge source specified in the original glabels project file.

.. option::  -o [<filename>], --output [<filename>]

	     Set output filename to <filename>. If filename is "-", write to standard output.
	     (Default="output.pdf")

.. option::  -s <n>, --sheets <n>

             For simple projects, sets number of full sheets to <n>. (Default=1)

.. option::  -c <n>, --copies <n>

	     Set number of copies to <n>. (Default=1)

.. option::  -a, --collate

	     Collate merge copies.

.. option:: -g, --group

	     Start each merge group on a new page.

.. option::  -f <n>, --first <n>

	     Set starting position to <n>. (Default=1)

.. option::  -l, --outlines

             Print label outlines.

.. option::  -m, --crop-marks

             Print crop marks.

.. option::  -r, --reverse

             Print in reverse (mirror image).

.. option::  -D <var>=<value>, --define <var>=<value>

	     Set initial value of user variable <var> to <value>.

FILES
-----

.. describe:: ${prefix}/share/glabels-qt/templates/

	      Directory containing predefined product templates distributed with glabels.

.. describe:: ${XDG_CONFIG_HOME}/glabels.org/glabels-qt/

	      Directory containing user defined product templates created with the
	      gLabels Product Template Designer.  **Do not place manually created
	      templates here!**

.. describe:: ${HOME}/.glabels/

              Directory for manually created product templates.

KNOWN ISSUES
------------

On GNU/Linux platforms, :program:`glabels-batch-qt` must be run from within a windowing environment, such as X or Wayland.  This is because Qt requires a suitable platform to obtain system font information.  A work-around is to run :program:`glabels-batch-qt` using a virtual X server environment:

.. code:: shell

	  xvfb-run glabels-batch-qt -o output.pdf myProject.glabels


REPORTING BUGS
--------------

Bugs and feature requests can be reported via the gLabels issue tracking system at GitHub (<https://github.com/j-evins/glabels-qt/issues>).  You will need a GitHub account to submit new issues or to comment on existing issues.

SEE ALSO
--------

:manpage:`glabels-qt(1)`

The gLabels homepage at <http://glabels.org/>.
