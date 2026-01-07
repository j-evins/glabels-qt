Glabels Coding Style
====================

This file describes the coding style used in all glabels source code.  Any
patches or pull requests should adhere to this style.


Formatting
----------

### Tabs vs. Spaces

Use spaces for all code indentation and line-to-line alignment of code elements.
(e.g. aligning function arguments).  This ensures that the code displays
correctly everywhere.


### Indentation Style

Glabels code uses the Allman[^1] style (a.k.a "BSD Style") of code indentation.
I.e. the brace associated with a control statement is placed on the next line,
indented to the same level as the control statement.  Statements within the
braces are indented to the next level.

```
while ( condition )
{
	something();
	somethingElse();
}

if ( condition2 )
{
	handleCondition2();
}
else
{
	somethingElse();
}
```

Also applies to function, class and namespace declaration statements.



### Parenthesis

- Never place spaces between a function name and its opening paren.
- Always place a space between a control statement and its opening paren.
- Never use parens in return statements when not necessary.


Naming
------

### Type Names

- Start with a capital letter.
- Each new word is capitalized.
- No underscores.


### Variable Names

- Start each variable name with a lowercase letter.
- Each subsequent word is capitalized.
- No underscores.
- Data members start with a lowercase "m" with the 2nd letter capitalized.
- Use "i" prefix for indexes and "n" prefix for total number of indexes.


### Function names

- Start each function name with a lowercase letter.
- Each subsequent word is capitalized.
- No underscores.


### Constant Names

- TBD (currently not uniform)


Code Organization
-----------------

Generally code is organized into modules.  Usually a module defines a single
class or a small namespace of functions/constants/etc.  A module is defined by
two files: a header file (its specification) and an implementation file.
Header filenames have a `.hpp` extension[^2] and implementation filenames have a
`.cpp` extension.


### Self-contained Headers

Header files should be self-contained.  I.e. they should not require any
prerequisite includes.  To enforce this requirement, an implementation file
shall include its header file before any other includes.


### Multiple Inclusion Guards

All header files should have an ifndef guard to prevent multiple inclusion.

```
#ifndef ns_Module_hpp
#define ns_Module_hpp

...

#endif // ns_Module_h
```


### Include Directives

Header files should be included in the following order.

1. header file for this module (e.g. this would be `Foo.hpp` in `Foo.cpp`).
2. Glabels header files.
3. Qt header files
3. C++ system header files (e.g. STL files)
4. C system header files (preference is for the C++ version if available,
   e.g. \<cmath> instead of \<math.h>.
5. Other libraries' header files

Paths used in include directives should always be relative to either the
glabels source directory or an appropriate base directory for each library.
They should NEVER include UNIX directory shortcuts such as "." (the current
directory) or ".." (the parent directory).

Angle brackets ("<>") should be used for inclusion of all external header files
(such as C/C++ and Qt header files). Double quotes should be used for all
glabels header files.

Do not use forward declarations to any external entities.  Use the appropriate
include directives instead.


### Namespaces

- Private definitions are placed in unnamed namespaces to limit scope to the current translation unit.
- All other glabels code is placed in the top-level "glabels" namespace.
- Nested namespace declarations should follow the more concise C++17 syntax
```
namespace glabels::model
{
        ...
}
```


Notes
-----

### Emacs

The following is an excerpt from my emacs configuration supporting the style
```
(defun my-c++-hook ()
    (setq c-default-style "bsd"
          c-basic-offset 8
          indent-tabs-mode nil
          show-trailing-whitespace t))

(add-hook 'c++-mode-hook 'my-c++-hook)
```


References
----------
[^1]: https://en.wikipedia.org/wiki/Indent_style#Allman_style
[^2]: [C++ Weekly - Ep 442 - Stop Using .h For C++ Header Files!](https://www.youtube.com/watch?v=mr3sOT-Delg)
