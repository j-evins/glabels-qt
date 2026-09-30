.. _merge_examples:

Example projects
****************

Here are some examples of projects which use the document merge features:

--------------------
Example 1: Name Tags
--------------------

In this example we are organizing an orientation party for the new crew members
of our ship.  We have a list of freshman crew members that we created in a
spreadsheet application and exported as the following CSV file.  We could have
created this file by using a text editor, but heck it is the 23rd century.

.. note::
   If you want to know how to export CSV files from various spreadsheet
   applications, see :ref:`spreadsheet_export`.

.. code-block::

   Name,Department,SN
   "Jim Kirk",Management,"SC937-0176 CEC"
   "Mr. Spock",Sciences,S179-276SP
   "Leonard McCoy",Medicine,unknown
   "Montgomery Scott",Engineering,SE-197-54


In **gLabels** we have created a new **glabels** document using the Avery 5395
"Name Badge Labels" template.  Next we click on **Merge** in the sidebar to
open the merge settings panel.  We use this to select the source type (in our
case CSV with keys on line 1) and the merge source (filename) as shown.

.. figure:: ./figures/merge_properties.png
   :alt: Merge properties

We can also view each record in more detail by clicking on the appropriate
expander (the little triangles) as shown.  Once we are satisfied with the
selections in this dialog we click **OK** to accept the changes.

Now we start adding objects to our **glabels** document as shown.

.. figure:: ./figures/merge-ex1-2.png
   :alt: Adding objects

In this example we have added three text objects and a barcode object.  The
first text object contains only simple literal text ("Hello, my name is").
The second text object contains a single merge field ("${Name}") corresponding
to the first field of a record (first column of a line) which contains the new
crew member's full name.  The third text field contains both literal text
("Department: ") followed by a single merge field ("${Department}")
corresponding to the second field or the crew member's department.  The barcode
object is configured to use field (or key) "SN" which contains our crew
member's starfleet serial number.

Now we can print our name tags by selecting the **File** ➡ **Print** menu item,
or by clicking on **Print** in the sidebar. This will display print dialog as
shown below.

.. figure:: ./figures/merge-ex1-3.png
   :alt: Printing name tags

The print preview seems to be OK so far. Just to make sure our labels are going
to look good, we select the **Print outlines** option.

Everything looks good, so we can now load up our printer with the appropriate
label stock, click on **Print** to print our name tags and start beaming our
guests aboard.

-------------------------
Example 2: Address labels
-------------------------

In this example we are going to throw a party and need to print mailing
addresslabels for our invitations.  We have a list of our closest friends that
we created in the spreadsheet application and exported as the following CSV
file.  It should be noted that not everyone has a middle initial or a two line
address.

.. code-block::

   LAST,FIRST,MI,ADDR1,ADDR2,CITY,STATE,ZIP
   ,,,,,,,
   Critter,Ess,S,"123 Faux St.",,Alexandria,VA,22310
   Doe,John,,"Apt 1X","451 Mystery St.",Trenton,NJ,08601
   Summers,Joyce,,"1630 Revello Dr",,Sunnydale,CA,95037
   McGarret,Steve,O,"404 Piikoi Street",,Honolulu,HI,96813
   Kramer,Cosmo,,"Apt 5B","129 W. 81 St.","New York",NY,10024-7207

In **glabels** we have created a new **glabels** document using the Avery 5512
"Address Labels" template.  Next we click on **Merge** in the sidebar to display
the **Merge Properties** dialog.  open the merge settings panel. We use this
to select the source type (in our case CSV with keys on line 1) and the merge
source (filename) as shown.

.. figure:: ./figures/merge-ex2-1.png
   :alt: Merge properties dialog

Before applying the merge source, we uncheck the first record since it only
contains empty fields.  It is an artifact of our original spreadsheet and would
simply waste our first label.  We could also unselect any other records that we
didn't want to print a label for.

Now we start adding objects to our **glabels** document as shown.

.. figure:: ./figures/merge-ex2-2.png
   :alt: Adding objects

In this example we have a single text object.  This text object contains all of
our merge fields organized on multiple lines as a mailing address. Notice that
fields ${ADDR1} and ${ADDR2} are each located with no other text on their own
lines.  When **glabels** encounters a field as the only text on a line, it will
not expand the line if the field is empty.

Now we can print our address labels by clicking on **Print** in the sidebar:

.. figure:: ./figures/merge-ex2-3.png
   :alt: Printing address labels

Just to make sure our labels are going to look okay, we select the
**Print outlines** option.

Everything looks good, so we can now load up our printer with the appropriate
label stock, print our address labels and start mailing our party invitations.
