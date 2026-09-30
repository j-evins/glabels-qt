.. _subst_spec:

DRAFT gLabels Substitution Field Specification
**********************************************

.. warning::
    This is a very early draft specification.  There is no guarantee that any
    of the capabilities will be implemented as described in gLabels 4.0.
    Prior to 4.0, this specification is subject to change.

This specification describes gLabels substitution fields.  Substitution fields
can be embedded in the data of gLabels text and barcode objects.  When printing,
these fields are replaced with their respective values.  Fields can represent
document merge fields or user defined variables.

In its simplest and most common form, the format is simply `${ field-name }`.
For example `${FIRST_NAME}`.  However, modifiers can be added to the field to
control how values are printed.

Syntax
------

The general syntax of a substitution field is

.. code-block::

   substitution-field = "${" field-name [ ":" modifiers ] "}" ;
   modifiers          = modifier [ ":" modifiers ] ;
   modifier           = format-modifier | default-value-modifier | new-line-modifier;


Field Names
-----------

Field names can refer to either [Document Merge Fields] :ref:`document-merge-fields`
or :ref:`user_variables`.  If a document merge field and a user variable share
the same name, the document merge field takes precidence.  Its syntax is simply:

.. code-block::

   field-name = merge-field-name | user-variable-name ;

.. _document-merge-fields:

Document Merge Fields
^^^^^^^^^^^^^^^^^^^^^

Document merge fields are the primary source of substitution fields.  A document
merge field represents a field from an external data source, such as a CSV file.
The valid syntax for a document merge field name is determined by the merge
source, with the following exception.  Merge field names cannot contain either
a colon (":") or closing curly bracket ("}").

.. _user_variables:

### User Variables
""""""""""""""""""

Substitution fields can also refer to user variables.  The syntax for valid
user variable names is

.. code-block::

   letter = "a" | "b" | ... | "z" | "A" | ... | "Z";
   digit = "0" | "1" | "2" | ... | "9";
   user-variable-name = ( letter | "_" ) , { letter | digit | "_" } ;



Modifiers
---------

### Format-Modifier (`%`)


A format modifier is used to control the format of numerical and string values.
It is a subset of a single printf format placeholder.  Its syntax is

.. code-block::

   format-modifier = "%" [ flags ] [ width ] [ "." precision ] type ;


### Flags

The Flags field can be zero or more (in any order) of:

+-------------+-----------------------------------------------------------------------------------------------------------------+
| Character   | Description                                                                                                     |
+=============+=================================================================================================================+
| `-` (minus) | Left-align the output of this placeholder. (the default is to right-align the output).                          |
+-------------+-----------------------------------------------------------------------------------------------------------------+
| `+` (plus)  | Prepends a plus for positive signed-numeric types. positive = `+`, negative = `-`.                              |
|             | (the default doesn't prepend anything in front of positive numbers).                                            |
+-------------+-----------------------------------------------------------------------------------------------------------------+
| ` ` (space) | Prepends a space for positive signed-numeric types. positive = ` `, negative = `-`.                             |
|             | This flag is ignored if the + flag exists. (the default doesn't prepend anything in front of positive numbers). |
+-------------+-----------------------------------------------------------------------------------------------------------------+
| `0` (zero)  | When the `width` option is specified, prepends zeros for numeric types. (the default prepends spaces).          |
|             | For example, `${a:=3:%2X}` produces `3`, while `${a:=3:%02X}` produces `03`.                                    |
+-------------+-----------------------------------------------------------------------------------------------------------------+

### Width

The Width field specifies a minimum number of characters to output, and is
typically used to pad fixed-width fields in tabulated output, where the fields
would otherwise be smaller, although it does not cause truncation of oversized
fields.

### Precision

The Precision field usually specifies a maximum limit on the output, depending
on the particular formatting type. For floating point numeric types, it
specifies the number of digits to the right of the decimal point that the
output should be rounded. For the string type, it limits the number of
characters that should be output, after which the string is truncated.

### Type


The Type field can be any of:

+-----------+------------------------------------------------------------------------------------------------------------------------------------------+
| Character | Description                                                                                                                              |
+===========+==========================================================================================================================================+
| `d`, `i`  | integer value as a signed decimal number. %d and %i are synonymous.                                                                      |
+-----------+------------------------------------------------------------------------------------------------------------------------------------------+
| `u`       | integer value as an unsigned decimal number.                                                                                             |
+-----------+------------------------------------------------------------------------------------------------------------------------------------------+
| `f`, `F`  | floating point value in normal (fixed-point) notation. f and F only differs in how the strings for an infinite number or NaN are printed |
|           | (inf, infinity and nan for f, INF, INFINITY and NAN for F).                                                                              |
+-----------+------------------------------------------------------------------------------------------------------------------------------------------+
| `e`, `E`  | floating point value in standard form ([-]d.ddd e[+/-]ddd). An E c letter E (rather than e) to introduce the exponent.                   |
|           | The exponent always contains at least two digits; if the value is zero, the exponent is 00.                                              |
+-----------+------------------------------------------------------------------------------------------------------------------------------------------+
| `g`, `G`  | floating point value in either normal or exponential notation, whichever is more appropriate for its magnitude.                          |
|           | g uses lower-case letters, G uses upper-case letters.                                                                                    |
|           | This type differs slightly from fixed-point notation in that insignificant zeroes to the right of the decimal point are not included.    |
|           | Also, the decimal point is not included on whole numbers.                                                                                |
+-----------+------------------------------------------------------------------------------------------------------------------------------------------+
| `x`, `X`  | integer value as an unsigned hexadecimal number. x uses lower-case letters and X uses upper-case.                                        |
+-----------+------------------------------------------------------------------------------------------------------------------------------------------+
| `o`       | integer value as an unsigned octal number.                                                                                               |
+-----------+------------------------------------------------------------------------------------------------------------------------------------------+
| `s`       | string value.                                                                                                                            |
+-----------+------------------------------------------------------------------------------------------------------------------------------------------+

### Default-Value-Modifier (`=`)


The default value modifier is used to set a default value for the field if its
value is undefined or empty. Its syntax is

.. code-block::

   default-value-modifier = "=" value ;


Several characters must be backslash-escaped if they are included in value.
These are `:`, `}`, and `\`.

+-----------------+-------------------+
| Escape sequence | actual character  |
+=================+===================+
| `\:`            | colon `:`         |
+-----------------+-------------------+
| `\}`            | right bracket `}` |
+-----------------+-------------------+
| `\\`            | backslash `\`     |
+-----------------+-------------------+

.. note::
   This modifier does not modify the value of a variable, it only uses this
   value if the variable is not defined.  For example, if the variable `${x}`
   is undefined, the string "`${x:=1} ${x:=2}`" would be printed as "`1 2`".
   Otherwise, if `${x}` was defined as "`3`", it would be printed as "`3 3`".

### New-Line-Modifier ('n')

This modifier is used to prepend a newline to the printed value, if the value
is defined and non-empty.  Its syntax is simply:

.. code-block::

   new-line-modifier = "n" ;


For example, this modifier is primarily intended to handle the following use case:

.. code-block::

   ${NAME}
   ${ADDR1}${ADDR2:n}
   ${CITY} ${STATE} ${ZIP}

`${ADDR2}` would be printed on its own line, only if it is set and non-empty.

