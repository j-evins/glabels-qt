.. _spreadsheet_export:

Export CSV files from various applications
******************************************

------------------------
Spreadsheet applications
------------------------

.. More options: Microsoft Excel, Collabora Calc …?

LibreOffice Calc
----------------

Choose :menuselection:`File --> Save as…`. In the :guilabel:`Filter` box,
select :guilabel:`Text CSV`. (Optional) Set the field options for the
:file:`.csv` file. Select :guilabel:`Edit filter settings`. In the
:guilabel:`Export of text files` dialog, select the options that you want.
Click :guilabel:`OK` and then :guilabel:`Save`.

Calligra Sheets
---------------

Choose :menuselection:`&File --> E&xport…` or
:menuselection:`&File --> Save &As…`. The file saver dialog shows the current
filename, currently probably with an :file:`.ods` extension (the default in
:program:`Calligra Sheets`). Open the :guilabel:`&File type` drop-down list and
choose :guilabel:`CSV document`. The filename extension changes automatically
to :file:`.csv`. Then click on :guilabel:`Save`.

Google Sheets
-------------

.. Please verify this on ChromeOS; this is the behavior in the web interface

Choose :menuselection:`File --> Download --> Comma Separated Values (CSV)`.
The file will be downloaded automatically into your configured download folder.
The original filename will be suffixed with :file:` - SheetX` where :file:`X`
is the number of the sheet (should usually be irrelevant for our purposes).

Gnumeric
--------

:program:`Gnumeric` doesn't offer any options to export :file:`.csv` files
directly from the user interface. But it comes with the command line tool
:command:`ssconvert`, which can convert to various formats, including
:file:`.csv`. For example, to convert a native :program:`Gnumeric` XML file:

.. code-block:: console

   $ ssconvert -T Gnumeric_stf:stf_csv infile.gnumeric outfile.csv

To see the accepted input file formats, run:

.. code-block:: console

   $ ssconvert --list-exporters

---------------------------------
Mail Clients and PIM Applications
---------------------------------

Mozilla Thunderbird
-------------------

You can export an address book of your choice from :program:`Thunderbird`.
Click on the address book icon in the vertical toolbar on the left or press
:kbd:`Ctrl-2` to open the address book view. Right-click on one of your address
books and choose :guilabel:`Export…`. A file chooser dialog will be opened. On
top of the window you can enter a name and then choose a location for the to be
exported file. With the drop-down menu in the bottom right corner, you can
choose a file type; we need :guilabel:`Comma Separated (UTF-8)` for our
purposes. Note, the exported :file:`.csv` file will contain many header entries
which we don't need at all. It makes sense to open the exported file in a text
editor (or even a spreadsheet application and re-export it as :file:`.csv`
file) to remove the unneeded entries.

Evolution
---------

:program:`Evolution` doesn't offer any options to export :file:`.csv` files
directly from the user interface. However, you can export :file:`.vcf` files
from your address book entries and convert them externally to :file:`.csv`
files. To do so, click on the addressbook icon in the bottom left corner of the
window. Then right-click on an address book in the list. Choose
:guilabel:`Save Address Book as vCard`. A file chooser dialog will be opened.
On top of the window you can enter a name and then choose a location for the to
be exported file. Regarding the final :file:`.vcf` to :file:`.csv` conversion,
see the footnote below [#f1]_.

Additionally, :program:`evolution-data-server` comes with a (somewhat hidden)
command line tool which exports the :program:`Evolution` addressbook to
:file:`.csv`. For example:

.. code-block:: console

   $ /usr/libexec/evolution-data-server/addressbook-export \
     --format=csv --output=/home/USERNAME/mycontacts.csv

The location and syntax of this utility has changed over time. See
`this forum thread on AskUbuntu <https://askubuntu.com/questions/653555/how-to-export-the-evolution-addressbook-as-csv-file>`__.

KAddressBook
------------

:program:`KAddressBook` doesn't offer any options to export :file:`.csv` files
directly from the user interface. However, you can export :file:`.vcf` files
from your address book entries and convert them externally to :file:`.csv`
files. To do so, choose :menuselection:`&File --> Export --> Export vCard X`,
where :guilabel:`X` is one of three offered vCard versions. A new window will
be opened where you can adjust some options, like the contacts you like to
export, and the to be exported fields. Click on :guilabel:`OK`. A file saver
dialog will be opened. You can enter a name and then choose a location for the
to be exported file. Click on :guilabel:`OK` to save the file. Regarding the
final :file:`.vcf` to :file:`.csv` conversion, see the footnote below [#f1]_.


.. [#f1] How to convert a :file:`.vcf` file into :file:`.csv` is out of the
   scope of this manual. There are some tools which do the job, like
   `vCard Studio <https://app.zdechov.net/vcard-studio>`__, or various online
   services (however, in this case you will have to upload and download your
   contact lists unencrypted).
