.. _documentmerge:

Performing a Document Merge
***************************

Document Merge (sometimes called "Mail Merge") is a powerful feature that
allows a unique label or card to be printed for each record in an external
data source.

The first step to performing a document merge is to prepare a source document
that contains your merge data.  This data could be mailing addresses or any
other data that you wish to create unique labels or cards for.  Currently
back-ends only exist for text files and the evolution data server -- others
are planned.  The currently supported text-file format is very simple:  each
line is a record; fields are delimited by commas (CSV), tabs, or colons; and
newlines can be embedded into fields by using the "\n" entity.  This file
could be created using any text editor or could be created by another program
or script.  A common way of creating CSV files is to export them from a
spreadsheet program or from address book applications.

.. note::
     To export a CSV file from **kaddressbook**, first select the
     contacts you wish to export. Click on the first contact, then press and
     hold **Ctrl** and click on the other desired contacts to select them.
     Then choose **File** ➡ **Export** ➡ **Export CSV File**.

     In the **Select Contacts** window, enable the button **Selected contacts**
     (which should be enabled by default anyway). Then click on **OK**.
     In the **Save As** dialog, choose the desired folder and a name
     for the file and click on **Save**.


.. note::
     To export a *CSV* file from **Thunderbird**, click on the **Address Book**
     toolbar button or choose **Tools** ➡ **Address Book** from the menu.
     Then select the contacts you wish to export. Click on the first contact,
     then press and hold **Ctrl** and click on the other desired contacts
     to select them. Then choose **Tools** ➡ **Export**.

     In the file chooser window, choose the desired folder and a name for the
     file and change the value of the drop-down button in the right bottom
     corner to **Comma Separated**. Finally, click on **Save**.

A label must then be configured to "point at" this data file. To configure the
merge properties of a document, choose **Objects** ➡ **Merge Properties** menu
item to display the **Merge Properties** dialog.  This dialog is used to
select the exact data file format and file name (location) of the merge data.

Finally, once the label has been configured for a data file, field keys can be
inserted into text objects and used as source or data for barcode objects and
image filenames for image objects. See :ref:`editing_object_properties` for more information
on using merge data for these object types.

Now that your label is configured, **gLabels** will print a unique label for
each record in your source document -- substituting fields from each record
for field keys in the all text, barcode, and image objects.

See :ref:`merge_examples` for a detailed tutorial on the document merge feature.
