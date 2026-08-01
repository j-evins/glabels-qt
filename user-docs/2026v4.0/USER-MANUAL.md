# gLabels Label Designer — Minimal User Manual

**Manual version:** 1.0 (text-complete, illustrations pending)
**Applies to:** glabels-qt 3.99 / 4.0 development branch

>Versioning note: gLabels is under active development. Features,
>menu names, and dialog layouts may change between releases. This manual
>is maintained alongside the source tree; if you are using a different
>release, some details may differ. Always cross-check with the actual
>application if in doubt.

**License:** This manual is free documentation, licensed under the
[GNU General Public License v3.0](../LICENSE) or (at your option) any
later version. You are free to copy, modify, and redistribute it under
the same terms as gLabels itself.

### Contents

1. [Introduction](#1-introduction)
2. [Core Concepts](#2-core-concepts)
3. [Creating a New Project](#3-creating-a-new-project)
4. [Working with Objects](#4-working-with-objects)
5. [The Tabs](#5-the-tabs)
6. [Merge and Variables](#6-merge-and-variables)
7. [Printing](#7-printing)
8. [Batch Mode (CLI)](#8-batch-mode-cli)
9. [File Format (optional)](#9-file-format-optional)
10. [Getting Help](#10-getting-help)

---

## 1. Introduction

gLabels is a graphical application for designing and printing labels,
business cards, and similar small-format print products. It ships with a
large database of pre-defined product templates (Avery, Herma, Zweckform,
Dymo, Brother, and many others) and lets you compose labels from text,
boxes, lines, ellipses, images, and barcodes. Labels can be printed
individually or in bulk from an external data source.

A companion command-line tool, `glabels-batch`, can print gLabels
projects without the graphical interface — useful for automation and
server-side workflows.

### What this manual covers

This is a *minimal* manual. It explains the concepts and workflows that
are specific to gLabels. It does **not** explain general computer use,
such as:

- opening and saving files via the standard file dialog,
- using a printer dialog,
- installing the application,
- basic text editing or copy/paste.

If you can perform ordinary desktop tasks, you should be able to follow
this manual without extra guidance.

### What this manual does not cover

- Installation and build instructions — see `BUILD-INSTRUCTIONS-LINUX.md`,
  `BUILD-INSTRUCTIONS-WINDOWS.md`, and `BUILD-INSTRUCTIONS-MACOS.md` in
  this directory.
- A complete reference of every menu item — the menus are largely
  self-explanatory once the core concepts are understood.
- Localization and translation — see `TRANSLATIONS.md` in this directory.

### Where to go next

If you are new to gLabels, read **Chapter 2 — Core Concepts** first. It
introduces the small set of ideas that the rest of the manual builds on.
If you already know the basics and want to print many different labels
from a list, jump to **Chapter 6 — Merge and Variables**.

---

## 2. Core Concepts

gLabels is built around a small number of concepts. Understanding these
four before touching the UI prevents most of the confusion new users
report.

### 2.1 Template

A **template** describes the physical medium you print on: the page
size, the shape and size of each label, how many labels fit on a sheet,
and their arrangement. gLabels ships with templates for thousands of
products from many vendors. You can also design your own template with
the built-in Template Designer.

Examples of templates:

- "Avery 5160" — a US-letter sheet with 30 rectangular address labels.
- "Herma 8752" — an A4 sheet with 24 round labels.
- "Dymo 99012" — a continuous roll of name-badge labels.

A template is *not* your design. It is only the description of the
physical product.

### 2.2 Project

A **project** is your design: the template you chose, plus everything
you placed on the label — text, images, barcodes, and so on — together
with any variables or merge settings.

A project is saved as a single XML file (extension `.glabels`). Opening
that file restores the full design.

> **Template vs. project, in one line:**
> The template answers *"what paper am I printing on?"*;
> the project answers *"what do I want on it?"*

### 2.3 Object

An **object** is a single element placed on a label. gLabels supports
six object types:

| Object   | Typical use                                   |
|----------|-----------------------------------------------|
| Text     | Names, addresses, any free text               |
| Box      | Rectangular frames, borders                   |
| Line     | Separators, dividers                          |
| Ellipse  | Round/oval frames                             |
| Image    | Logos, pictures                               |
| Barcode  | Code39, EAN13, QR, DataMatrix, POSTNET, ...   |

Every object has properties (position, size, color, line width, font,
...) that can be edited in the **Edit** and **Properties** tabs. Objects
can be selected, moved, resized, rotated, flipped, aligned, ordered
front-to-back, copied, and deleted — the operations you would expect
from any drawing tool.

### 2.4 Merge and Variables — the two "fill-in" mechanisms

This is the concept that causes the most confusion, so it is introduced
here briefly and explained in full in **Chapter 6**.

Both mechanisms let you put values into your labels that are not typed
directly into the design. They use the same `${ name }` placeholder
syntax, which is precisely why they are easy to mix up.

- **Variables** are values you define once and reuse within a *single*
  label design. Example: a company name that appears in three places on
  one label and that you want to change in a single spot.
  → *One design, one printed label, reusable values.*

- **Merge** is a mechanism for producing *many different labels* from a
  single design, using an external data file (CSV, TSV, ...). Each row
  in the file becomes one printed label.
  → *One design, many printed labels, each with different data.*

A simple decision guide:

```
Do you want to print many different labels at once?
  yes → use Merge (Chapter 6.3)
  no  → Do you want to reuse a value in several places on one label?
          yes → use Variables (Chapter 6.2)
          no  → just type the text directly; you need neither
```

It is also possible to combine the two — see Chapter 6.4.

### 2.5 The workspace

When no project is open, gLabels shows the **Welcome** screen: start a
new project, open an existing one, or pick a recent file.

Once a project is open, the main window shows five tabs along the left
side. You move between them by clicking the corresponding button.

| Tab            | Purpose                                                |
|----------------|--------------------------------------------------------|
| **Edit**       | The canvas. Place, move, and arrange objects here.     |
| **Properties** | Template and page settings: product, size, orientation.|
| **Merge**      | Connect an external data file and preview its records. |
| **Variables**  | Define and edit user variables.                        |
| **Print**      | Print range, copies, collation, outlines, crop marks.  |

The **Edit** tab is where you spend most of your time. The other tabs
are configuration and output. Chapter 5 describes each tab in more
detail, including Welcome.

> `[TODO: screenshot of the main window showing the five tabs]`

---

## 3. Creating a New Project

A new gLabels project always begins by choosing a **template** — the
physical product you will print on. There are two ways to get a template:

1. **Select an existing product** from the built-in database (the
   common case — thousands of products are included).
2. **Design a custom template** with the Template Designer wizard (for
   products not in the database, or for unusual media).

### 3.1 Starting a new project

From the **Welcome** tab, click **New...**. (If you already have a
project open, use **File → New** from the menu.)

This opens the **Select Product** dialog.

> `[TODO: screenshot of the Welcome tab with the "New..." button]`

### 3.2 Selecting an existing product

The Select Product dialog has two tabs at the top:

| Tab           | Purpose                                                   |
|---------------|-----------------------------------------------------------|
| **Search all**| Search the entire product database by text, with filters. |
| **Recent**    | Pick from products you have used recently.                |

#### Searching the database

On the **Search all** tab:

1. Type part of the brand name, part number, or description into the
   **Search** field. The list updates as you type.
2. Optionally narrow the results with the filters on the left:
   - **Filter by paper size**: check *ISO sizes*, *US sizes*, and/or
     *Other*.
   - **Filter by category**: choose *All* categories, or *Selected* and
     tick the categories you want (e.g. *Mailing*, *Business Card*).
3. Click a product in the list to select it. The **Product information**
   panel on the right shows vendor, part number, description, page size,
   label size, and layout.
4. Click **OK** (or press Enter) to confirm, or **Cancel** to abort.

> `[TODO: screenshot of the Select Product dialog, Search all tab]`

#### Using recent products

If you print the same products repeatedly, the **Recent** tab lists them
without needing to search. Click a product, then **OK**.

### 3.3 Designing a custom template

If your product is not in the database, click **Template Designer...**
(File → Template Designer from the main menu). This opens a wizard that
walks you through creating a template step by step.

The wizard pages are:

1. **Intro** — choose how to start:
   - **Copy/Edit Product**: begin from an existing product and modify
     its dimensions. Useful when a product is almost, but not quite,
     what you need.
   - **New Product**: start from scratch.

2. **Name** — enter a **Brand** (e.g. "Acme"), a **Part #** (e.g.
   "8163A"), and a **Description** (e.g. "Mailing Labels"). These
   identify your template in the database.

3. **Page size** — choose the page size (e.g. A4, US Letter) from the
   list, or enter custom dimensions. For roll labels, set the **Roll
   width** and **Height** instead of a fixed page size.

4. **Shape** — choose the label shape:
   - *Rectangular or square* (may have rounded corners),
   - *Round*,
   - *Elliptical*,
   - *CD/DVD* (including credit-card CDs).
   For continuous-roll media, the wizard offers a **Continuous** path
   instead of a fixed shape.

5. **Shape-specific dimensions** — depending on the shape chosen, enter
   the label dimensions, corner radius, margins, and waste area.

6. **Number of layouts** — most products need only **one layout** (a
   single grid of identical labels). Choose **two layouts** only for
   products where labels alternate in position on the sheet. (If you
   need more than two layouts, the template must be edited manually in
   XML — see Chapter 9.)

7. **Layout details** — set the number of labels across and down, the
   spacing between them, and the margins from the sheet edge. A
   **Print test sheet** button lets you verify the layout on paper
   before saving.

8. **Apply** — review your choices. Click **Save** to add the template
   to your personal template database, **Back** to revise, or
   **Cancel** to discard.

Custom templates you create are stored separately from the built-in
database and remain available in future sessions.

> `[TODO: screenshot of the Template Designer wizard, Shape page]`

### 3.4 Sheet labels vs. roll labels

The workflow differs slightly depending on the medium:

- **Sheet labels** (the common case): you print on a sheet of paper or
  cardstock containing multiple labels. The template defines a fixed
  page size and a grid of labels on it. After printing, you peel the
  labels from the sheet.

- **Roll labels**: you print on a continuous roll (typically with a
  label printer such as Dymo or Brother). The template defines a roll
  width and a label height, but no fixed page length — the printer
  feeds the roll and cuts between labels. Roll templates use the
  **Continuous** shape in the Template Designer.

The distinction matters mainly when choosing or designing a template
and when printing (Chapter 7). Within the editor, both behave the same.

### 3.5 After choosing a template

Once you confirm a template (from the database or the designer),
gLabels creates a new, empty project and switches to the **Edit** tab.
The canvas shows the outline of your labels; you can now start placing
objects (Chapter 4).

The project is not yet saved. Use **File → Save** (or **Save As**) to
store it as a `.glabels` file. gLabels remembers your recently used
projects on the Welcome tab and in the **File → Open Recent** menu.

---

## 4. Working with Objects

Objects are the elements you place on a label: text, boxes, lines,
ellipses, images, and barcodes. This chapter covers how to create,
select, move, resize, transform, align, order, copy, and delete them,
and how to edit their properties.

### 4.1 Creating objects

Objects are created from the **Edit** tab. The toolbar on the left side
of the editor offers a tool for each object type:

| Tool       | Creates                                         |
|------------|-------------------------------------------------|
| Arrow      | Selection mode (no new object).                 |
| Text       | A text box.                                     |
| Box        | A rectangle (may have rounded corners).         |
| Line       | A straight line.                                |
| Ellipse    | An ellipse or circle.                           |
| Image      | An image object (loads from a file).            |
| Barcode    | A barcode (Code39, EAN13, QR, ...).             |

To create an object:

1. Click the tool for the desired object type.
2. Click and drag on the canvas to define the object's initial size
   and position. (For a line, drag from one endpoint to the other.)
3. Release the mouse. The object is created and selected.

After creating an object, the toolbar returns to the Arrow (selection)
tool automatically, so you can immediately adjust the new object.

> `[TODO: screenshot of the editor toolbar showing the object tools]`

### 4.2 Selecting objects

- **Select a single object**: click it with the Arrow tool.
- **Add to / remove from the selection**: Shift-click an object.
- **Select all objects**: **Edit → Select All** (or Ctrl+A).
- **Deselect all**: **Edit → Unselect All** (or click on empty canvas).

When an object is selected, it shows a set of **handles** (small
squares) at its corners and edges that you can drag to resize it.

### 4.3 Moving and resizing

- **Move an object**: drag its body (not a handle) to a new position.
- **Resize an object**: drag one of its handles. Drag a corner handle
  to resize both dimensions; drag an edge handle to resize one
  dimension.
- **Exact position and size**: use the **position/size** tab of the
  Object Editor (see 4.7) to enter numeric values.

### 4.4 Transforming objects

The **Objects** menu (and the editor toolbar) offers transformations:

| Action              | Menu path                             |
|---------------------|---------------------------------------|
| Rotate left 90°     | Objects → Transform → Rotate Left     |
| Rotate right 90°    | Objects → Transform → Rotate Right    |
| Flip horizontally   | Objects → Transform → Flip Horizontal |
| Flip vertically     | Objects → Transform → Flip Vertical   |

Free rotation by an arbitrary angle is available in the
**position/size** tab of the Object Editor.

### 4.5 Aligning and centering

When multiple objects are selected, the **Objects → Align** menu lets
you align them to each other:

- Align Left / HCenter / Right
- Align Top / VCenter / Bottom

The **Objects → Center** menu centers the selection on the label:

- Center (both directions)
- Center Horizontally
- Center Vertically

### 4.6 Ordering, copying, and deleting

Objects are drawn in a stacking order. The **Objects → Order** menu
changes this order:

- Raise to Top — bring the selection to the front.
- Lower to Bottom — send the selection to the back.

Standard clipboard operations work as expected:

| Action   | Menu path                | Shortcut |
|----------|--------------------------|----------|
| Cut      | Edit → Cut               | Ctrl+X   |
| Copy     | Edit → Copy              | Ctrl+C   |
| Paste    | Edit → Paste             | Ctrl+V   |
| Delete   | Edit → Delete            | Del      |

Pasting places a copy of the clipboard contents at the pointer
position (or at the original position if the pointer is not over the
canvas). Pasted objects become the new selection.

### 4.7 The Object Editor

When an object is selected, the **Object Editor** panel (typically on
the right side of the Edit tab) shows the properties of that object.
The properties shown depend on the object type. The editor is organized
in tabs; the available tabs per object type are:

| Object   | Tabs shown                                  |
|----------|---------------------------------------------|
| Text     | text, position/size, shadow                 |
| Box      | line/fill, position/size, shadow            |
| Ellipse  | line/fill, position/size, shadow            |
| Line     | line/fill, position/size, shadow            |
| Image    | image, position/size, shadow                |
| Barcode  | barcode, position/size                      |

If multiple objects are selected, the Object Editor shows the common
properties and applies changes to all selected objects. If no object is
selected, the editor is disabled.

> `[TODO: screenshot of the Object Editor with a text object selected]`

### 4.8 Common properties: position, size, and shadow

The **position/size** tab is shared by all object types. It contains:

- **Position** — the X and Y coordinates of the object's origin,
  measured from the top-left corner of the label.
- **Size** — for rectangular objects: width and height. For lines:
  length and angle. For images: width and height, plus a button to
  reset to the image's original size.
- **Lock aspect ratio** — when enabled, resizing one dimension keeps
  the other in proportion.

The **shadow** tab (not available for barcodes) adds a drop shadow to
the object:

- **Shadow enabled** — toggle the shadow on or off.
- **X / Y offset** — how far the shadow is displaced from the object.
- **Opacity** — how transparent the shadow is (0–1).
- **Color** — the shadow's color (may be a fixed color or a
  merge/variable-driven color; see Chapter 6).

### 4.9 Object-type-specific properties

#### Text objects

The **text** tab controls the textual content and typography:

- **Text** — the text to display. May contain `${...}` substitution
  fields (see Chapter 6 and `SUBSTITUTION-FIELD-SPEC.md`).
- **Font family, size, weight, italic, underline** — standard font
  properties. These apply to the entire text object.
- **Text color** — may be fixed or merge/variable-driven.
- **Horizontal alignment** — left, center, right, or justified.
- **Vertical alignment** — top, center, or bottom.
- **Wrap mode** — none, word, or anywhere (where to break long lines).
- **Line spacing** — extra spacing between lines.
- **Auto-shrink** — if enabled, the font size is automatically reduced
  so that the text fits within the object's bounds.

A small **insert substitution field** button next to the text editor lets you insert a
merge field or variable placeholder without typing the `${...}` syntax
by hand.

#### Box, ellipse, and line objects

The **line/fill** tab controls the outline and fill:

- **Line width** — thickness of the outline (may be zero for no
  outline).
- **Line color** — may be fixed or merge/variable-driven.
- **Fill color** — the interior color (boxes and ellipses only; lines
  have no fill). May be fixed or merge/variable-driven.

For boxes, the corner radius (if any) is set on the template, not per
object.

#### Image objects

The **image** tab controls the source image:

- **File** — the path to the image file. Use the browse button to pick
  a file. The image is embedded in the project when saved, so the
  external file is no longer needed after saving.
- **Reset to original size** — restores the image's natural pixel
  dimensions.

Supported formats include PNG, JPEG, and SVG. The path may itself
contain `${...}` substitution fields, so the image can vary per merge
record (e.g. `${photo}`).

#### Barcode objects

The **barcode** tab controls the barcode content and appearance:

- **Data** — the text or numeric data to encode. May contain `${...}`
  substitution fields.
- **Style** — type: the barcode symbology (Code39, Code39 Extended, EAN13,
  UPC-A, POSTNET, QR Code, DataMatrix, ONECODE, CEPNET, ...). The
  available styles depend on which optional backend libraries
  (GNU Barcode, libqrencode, libzint) were compiled in.
- **Show text** — whether to print the human-readable text below the
  barcode.
- **Checksum** — whether to append a checksum digit (where the
  symbology supports it).
- **Color** — the barcode color (may be fixed or merge/variable-driven).
- **Format digits** — controls numeric formatting of the data via the
  substitution-field format modifier (see `SUBSTITUTION-FIELD-SPEC.md`).

A small **insert substitution field** button lets you insert a merge field or
variable placeholder into the data field.

> `[TODO: screenshot of the Object Editor with a barcode object selected]`

### 4.10 Colors and merge/variable-driven properties

Several color properties (text color, line color, fill color, shadow
color, barcode color) can be either:

- a **fixed color** that you pick from the color palette, or
- a **merge/variable-driven color** that takes its value from a merge
  field or user variable at print time.

This is useful for things like color-coding labels by category. The
mechanism is the same `${...}` substitution used for text — see
Chapter 6 for details.

---

## 5. The Tabs

gLabels organizes its work into tabs along the left side of the main
window. Each tab is a self-contained activity; you move between them by
clicking the corresponding button. This chapter gives a short overview
of each tab and when to use it. The Merge and Variables tabs are
covered in more depth in Chapter 6.

| Tab            | Use it to...                                          |
|----------------|-------------------------------------------------------|
| **Welcome**    | start or open a project.                              |
| **Edit**       | place and arrange objects on the label.               |
| **Properties** | change the template, page size, or orientation.       |
| **Merge**      | connect an external data file for bulk printing.      |
| **Variables**  | define reusable values for the current project.       |
| **Print**      | preview and print the final result.                   |

> `[TODO: screenshot of the main window showing the tabs after welcome]`

### 5.1 Welcome tab

The Welcome tab is the start screen. It appears when you launch gLabels
without opening a project, and whenever no project is loaded.

It offers three actions:

- **New...** — opens the Select Product dialog (Chapter 3.2) to start a
  new project.
- **Browse...** — opens an existing `.glabels` project file.
- **Recent** — re-opens a recently used project.

Once a project is open, the Welcome tab is hidden; you can
return to it to start another project in a new window.

### 5.2 Edit tab

The Edit tab is the design canvas. This is where you spend most of your
time. It contains:

- The **canvas** in the center, showing the label(s) on the sheet (or
  the roll). Objects are placed and edited here.
- The **object toolbar** on the upper left, with the tools to create text,
  box, line, ellipse, image, and barcode objects (Chapter 4.1).
- The **Object Editor** panel on the right, showing the properties of
  the currently selected object (Chapter 4.7).
- A **status bar** at the top, showing zoom level.

The Edit tab is also where you perform object operations via the menu
and toolbar: undo/redo, cut/copy/paste, align, center, order, transform
(Chapter 4).

The **View** menu controls the canvas display:

- **Grid** — toggle a grid overlay (useful for precise placement).
- **Markup** — toggle markup lines (e.g. label outlines, margins).
- **Zoom In / Out / 1:1 / Fit** — change the zoom level.

### 5.3 Properties tab

The Properties tab shows the template and page settings of the current
project. It is divided into three sections:

- **Product** — read-only information about the current template:
  vendor, part number, description, page size, label size, and layout.
  A **Change product** button opens the Select Product dialog
  (Chapter 3.2) to switch to a different template without losing your
  design.

- **Adjustable Parameters** — for templates that support it (typically
  continuous-roll labels), the **Label length** can be adjusted here.
  This is useful for roll labels where the exact length varies.

- **Orientation** — choose **Horizontal** or **Vertical** orientation.
  This rotates the label layout on the page. For roll labels, this
  effectively chooses whether the label is wider than tall or vice
  versa.

> `[TODO: screenshot of the Properties tab]`

### 5.4 Merge tab

The Merge tab connects an external data file to the project so that
many different labels can be printed from a single design. It is
covered in full in Chapter 6.3; this is a short overview.

The tab has two sections:

- **Source** — choose the merge **Format** (CSV, TSV, text with colon
  or semicolon separators, with or without a header row) and the
  **Location** of the data file. Use the **Browse...** button to pick a
  file.

- **Records** — a table showing the records from the data file, with
  checkboxes to include or exclude individual records. Buttons:
  **Select all**, **Unselect all**, and **Reload** (re-read the file
  after it has changed on disk).

Once a merge source is connected, the field names from the data file
become available as `${field-name}` placeholders in text and barcode
objects (Chapter 4.9). The Merge tab does not change the design itself;
it only links the data.

### 5.5 Variables tab

The Variables tab manages user variables for the current project. It is
covered in full in Chapter 6.2; this is a short overview.

The tab shows a table of all variables defined in the project, with
three buttons:

- **Add** — create a new variable. Opens the Edit Variable dialog.
- **Edit** — modify the selected variable.
- **Delete** — remove the selected variable.

Each variable has the following properties (set in the Edit Variable
dialog):

- **Name** — the identifier used in `${name}` placeholders. Must start
  with a letter or underscore and contain only letters, digits, and
  underscores.
- **Type** — one of:
  - *String* — arbitrary text.
  - *Integer* — a whole number.
  - *Floating point* — a decimal number.
  - *Color* — a color value (for color-driven properties).
- **Value** — the initial value of the variable.
- **Increment** — whether and how the variable is incremented during
  printing:
  - *Never* — the value stays constant.
  - *Per item* — incremented for each label.
  - *Per copy* — incremented for each copy.
  - *Per page* — incremented for each page.
- **Step size** — the amount to add on each increment (for numeric
  types).

Incrementing variables are useful for things like serial numbers or
batch codes that change from label to label.

> `[TODO: screenshot of the Variables tab with the Edit Variable dialog open]`

### 5.6 Print tab

The Print tab is where you produce the final output. It is covered in
full in Chapter 7; this is a short overview.

The tab has four sections:

- **Destination** — choose the printer or an output file. A **Use
  system print dialog...** button gives access to advanced printer
  options.

- **Print range** — choose what to print:
  - *Pages* — print a range of pages.
  - *Positions* — print starting from a specific label position on the
    sheet (useful for re-using partially printed sheets).

- **Merge control** (only visible when a merge source is connected) —
  *Copies*, *Collated / Uncollated*, and whether merge groups start on
  a new page or are contiguous.

- **Options** — *print outlines*, *print crop marks*, and *print in
  reverse (mirror image)*.

A preview area shows the page(s) that will be printed, with **Page**
navigation. The **Print** button sends the job to the chosen
destination.

### 5.7 How the tabs fit together

A typical workflow moves through the tabs roughly top to bottom:

1. **Welcome** — start or open a project.
2. **Properties** — confirm or change the template and orientation.
3. **Edit** — design the label: place objects, set their properties.
4. **Variables** — (optional) define reusable values.
5. **Merge** — (optional) connect an external data file.
6. **Print** — produce the output.

You can switch back and forth at any time. For example, you might add a
variable, return to the Edit tab to use it in a text object, then go to
Merge to connect a data file, and finally Print. Changes in any tab are
reflected immediately in the others.

---

## 6. Merge and Variables

This is the chapter that most new users need. Merge and variables are
the two mechanisms that let you put values into your labels that are
not typed directly into the design. They share the same `${ name }`
placeholder syntax, which is precisely why they are easy to mix up.

If you read only one chapter, read this one.

### 6.1 What is the difference?

A one-line summary of each:

- **Variables** — values you define once and reuse within a *single*
  label design. The number of printed labels does not change.
- **Merge** — a mechanism for producing *many different labels* from a
  single design, using an external data file. Each row in the file
  becomes one printed label.

A decision guide:

```
Do you want to print many different labels at once?
  yes → use Merge (6.3)
  no  → Do you want to reuse a value in several places on one label?
          yes → use Variables (6.2)
          no  → just type the text directly; you need neither
```

You can also combine the two (6.4).

### 6.2 Variables: reusable values within one design

A **variable** is a named value that you define in the project and
refer to from one or more objects. When you change the variable's value,
every object that uses it updates automatically.

#### When to use variables

- A company name or logo caption that appears in several places on the
  label and that you want to change in a single spot.
- A batch code or version string that you want to centralize.
- A color that should vary based on a single value (color-typed
  variable, used in a color property — see 4.10).
- A serial number that increments from label to label (using the
  *Increment* feature).

#### Defining a variable

Variables are managed on the **Variables** tab (Chapter 5.5). Click
**Add** to open the Edit Variable dialog. Each variable has:

- **Name** — the identifier used in `${name}` placeholders. Must start
  with a letter or underscore and contain only letters, digits, and
  underscores (e.g. `company`, `batch_no`, `_color1`).
- **Type** — one of:
  | Type            | Use                                  |
  |-----------------|--------------------------------------|
  | String          | arbitrary text                       |
  | Integer         | a whole number                       |
  | Floating point  | a decimal number                     |
  | Color           | a color value (for color properties) |
- **Value** — the initial value of the variable.
- **Increment** — whether and how the value is incremented during
  printing:
  | Increment   | When the value advances              |
  |-------------|--------------------------------------|
  | Never       | the value stays constant             |
  | Per item    | for each label                       |
  | Per copy    | for each copy                        |
  | Per page    | for each page                        |
- **Step size** — the amount added on each increment (for numeric
  types). For example, a step size of `1` with increment *Per item*
  produces 1, 2, 3, 4, ... on successive labels.

Variables can also be defined from the command line when using
`glabels-batch`, with the `--define name=value` option (Chapter 8).
Command-line definitions override the project's initial values.

#### Using a variable in an object

In any text or barcode object, type `${name}` where you want the
variable's value to appear. For example, with a variable named
`company`:

```
© 2026 ${company}
```

When printed, `${company}` is replaced by the variable's value. The
Object Editor also has a small **insert field** button next to text and
barcode data fields that inserts the placeholder for you, so you do not
have to type the `${...}` syntax by hand.

Variables can be used in any text property, in barcode data, in image
file paths (e.g. `${photo}.png`), and in color properties (for
color-typed variables).

#### Worked example: one label with a reusable company name

1. On the Variables tab, click **Add**.
2. Set Name = `company`, Type = String, Value = `Acme Corp.`.
3. On the Edit tab, create a text object and type:
   `Distributed by ${company}`.
4. Create a second text object elsewhere on the label and type:
   `${company} — Quality Guaranteed`.
5. Print. Both objects show `Acme Corp.`.
6. To change the name everywhere, edit the `company` variable on the
   Variables tab and re-print. You do not touch the text objects.

> `[TODO: screenshot of the Variables tab and the Edit Variable dialog]`

### 6.3 Merge: many labels from an external data file

A **merge** connects an external data file to the project. Each row in
the file becomes one printed label. The columns (fields) of the file
are referenced by name in `${field-name}` placeholders.

#### When to use merge

- Address labels from a guest list.
- Name badges from a registration file.
- Product labels with per-item data (SKU, price, barcode).
- Any situation where you have many similar labels that differ only in
  their data.

#### Supported file formats

The merge format is chosen on the **Merge** tab (Chapter 5.4). The
available formats are:

| Format                                              | Separator | Header row |
|-----------------------------------------------------|-----------|------------|
| Text: Comma Separated Values (CSV)                  | comma     | no         |
| Text: Comma Separated Values (CSV), keys on line 1  | comma     | yes        |
| Text: Tab Separated Values (TSV)                    | tab       | no         |
| Text: Tab Separated Values (TSV), keys on line 1    | tab       | yes        |
| Text: Colon Separated Values                        | colon     | no         |
| Text: Colon Separated Values, keys on line 1        | colon     | yes        |
| Text: Semicolon Separated Values                    | semicolon | no         |
| Text: Semicolon Separated Values, keys on line 1    | semicolon | yes        |

The "keys on line 1" variants treat the first line of the file as a
header row that names the columns. The non-keys variants name the
columns `0`, `1`, `2`, ... by position.

> **Tip:** If your data contains commas (or the chosen separator) inside
> field values, enclose those values in double quotes. For example:
> `"Smith, John",123 Main St,Anytown`

#### Connecting a merge source

1. Go to the **Merge** tab.
2. In the **Source** section, choose the **Format** that matches your
   file.
3. Click **Browse...** next to **Location** and pick the data file.
4. The **Records** section now shows a table of the file's contents.
   Each row is a record; each column is a field. Checkboxes let you
   include or exclude individual records.
5. Use **Select all** / **Unselect all** to toggle all records at once.
6. If the file changes on disk after you connected it, click **Reload**
   to re-read it.

Once connected, the field names become available as `${field-name}`
placeholders in text and barcode objects.

#### Using merge fields in an object

In a text or barcode object, type `${field-name}` where you want the
value to appear. For example, with a CSV file that has a column named
`Name`:

```
Hello, ${Name}!
```

When printed, one label is produced per record, and each label shows
the corresponding name. The Object Editor's **insert field** button
lists the available field names so you do not have to type them.

#### Worked example: address labels from a CSV file

Suppose you have `guests.csv` (CSV with keys on line 1):

```
Name,Street,City,Zip
Alice,1 Oak St,London,1000
Bob,2 Pine St,Paris,2000
Carol,3 Maple St,Berlin,3000
```

1. Open or create a project with an address-label template (e.g.
   Avery 5160).
2. On the Merge tab, choose format *Text: Comma Separated Values (CSV),
   keys on line 1* and select `guests.csv`.
3. On the Edit tab, create a text object and type:
   ```
   ${Name}
   ${Street}
   ${City} ${Zip}
   ```
4. Go to the Print tab. The preview now shows three pages of labels —
   one per record — each with a different address.
5. Click **Print**.

> `[TODO: screenshot of the Merge tab with records loaded, and the Print preview showing merged labels]`

### 6.4 Combining merge and variables

Merge and variables can be used together in the same project. The
substitution order, when a placeholder `${name}` is evaluated, is:

1. If a **merge field** with that name exists in the current record,
   its value is used.
2. Otherwise, if a **user variable** with that name exists, its value
   is used.
3. Otherwise, the placeholder's **default value** (set with the `=`
   modifier, see 6.5) is used, or empty if no default is set.

This means a merge field and a variable can share the same name; the
merge field takes precedence. This is useful for cases where you want a
value to come from the data file when available, but fall back to a
fixed value otherwise.

#### Worked example: merge with a fixed company variable

Building on the previous example, suppose each address label should
also show a fixed company name. You do not want to put the company name
in the CSV (it would be the same on every row), so you use a variable:

1. On the Variables tab, add a variable `company` = `Acme Corp.`
2. On the Edit tab, add to the text object:
   ```
   ${Name}
   ${Street}
   ${City} ${Zip}
   ${company}
   ```
3. Print. Each label shows the per-record address plus the fixed
   company name.

If a record happened to have a field named `company`, that field's
value would override the variable for that record.

### 6.5 Placeholder syntax and modifiers

The full placeholder syntax is documented in
[`SUBSTITUTION-FIELD-SPEC.md`](SUBSTITUTION-FIELD-SPEC.md). This
section gives a short summary.

The basic form is:

```
${field-name}
```

Modifiers can be added after the name, separated by colons:

```
${field-name:modifier1:modifier2:...}
```

The available modifiers are:

| Modifier | Syntax                            | Effect                                                    |
|----------|-----------------------------------|-----------------------------------------------------------|
| Format   | `%[flags][width][.precision]type` | Format the value like printf (see below).                 |
| Default  | `=value`                          | Use `value` if the field/variable is empty or undefined.  |
| Newline  | `n`                               | Prepend a newline if the field/variable is non-empty.     |

#### Format modifier

The format modifier is a subset of the C `printf` format string. The
`type` character selects how the value is interpreted and printed:

| Type     | Meaning                              |
|----------|--------------------------------------|
| `d`, `i` | signed integer                       |
| `u`      | unsigned integer                     |
| `f`, `F` | floating point, fixed notation       |
| `e`, `E` | floating point, exponential notation |
| `g`, `G` | floating point, auto                 |
| `x`, `X` | unsigned hexadecimal                 |
| `o`      | unsigned octal                       |
| `s`      | string                               |

Optional flags: `-` (left-align), `+` (force sign), ` ` (space for
positive), `0` (zero-pad). Width and precision work as in printf.

Examples:

- `${count:%04d}` — integer, zero-padded to 4 digits (e.g. `0042`).
- `${price:%.2f}` — floating point, 2 decimals (e.g. `3.50`).
- `${name:%-20s}` — string, left-aligned in a 20-character field.

#### Default value modifier

`${field:=default}` uses `default` when the field or variable is empty
or undefined. The characters `:`, `}`, and `\` must be backslash-escaped
in the default value (`\:`, `\}`, `\\`).

Example: `${Title:=Mr.} ${Name}` prints `Mr. Alice` if the `Title`
field is empty.

#### Newline modifier

`${field:n}` prepends a newline to the value, but only if the field is
non-empty. This is intended for optional address lines:

```
${Name}
${Street}${Unit:n}
${City} ${Zip}
```

If `Unit` is empty, no extra blank line appears; if it is set (e.g.
`Apt 5`), it is printed on its own line after the street.

### 6.6 Common mistakes and FAQ

**"My variable prints literally as `${name}` instead of its value."**
The name in the placeholder does not match any defined variable. Check
spelling and case — names are case-sensitive. Also check that the
variable is actually defined on the Variables tab.

**"I expected thirty different labels but got only one."** You used a
variable where you needed a merge. Variables produce one value per
project; merge produces one value per record. Connect a data file on
the Merge tab and use `${field-name}` placeholders.

**"My CSV with commas in fields breaks."** Enclose field values that
contain the separator in double quotes: `"Smith, John",...`. If your
data uses a different separator throughout, pick a matching format
(semicolon, colon, tab).

**"The first row of my CSV is shown as data, not as column names."**
You picked the non-keys variant. Switch to the "keys on line 1" format
for your file.

**"I want a value that comes from the file when available, but a fixed
fallback otherwise."** Use the default-value modifier:
`${field:=fallback}`, or define a variable with the same name — merge
fields take precedence, so the variable acts as the fallback.

**"I want a serial number that increments per label."** Define an
integer variable with Increment = *Per item* and Step size = `1`.
Reference it as `${serial}` (or `${serial:%04d}` for zero-padding).

**"Do variables or merge fields take precedence?"** Merge fields take
precedence. If both exist with the same name, the merge field's value
is used for that record; the variable is used only when no merge field
matches.

---

## 7. Printing

The **Print** tab produces the final output: paper labels on a printer,
or a PDF file. This chapter covers the options on the Print tab and
the specifics of printing sheet labels versus roll labels.

### 7.1 The Print tab overview

The Print tab is divided into four sections, with a preview area and a
Print button at the bottom:

- **Destination** — where to send the output.
- **Print range** — which pages or positions to print.
- **Merge control** — (only when a merge source is connected) copies,
  collation, and grouping.
- **Options** — outlines, crop marks, reverse printing.

A preview area shows the page(s) that will be printed, with **Page** 
navigation to move between pages.

> `[TODO: screenshot of the Print tab]`

### 7.2 Destination

The **Destination** section offers two ways to send output:

- A drop-down list of **available printers** on the system. The list is
  refreshed automatically when printers are added or removed. Select a
  printer to send the job there when you click **Print**.
- An option to **print to a PDF file**. The default filename is derived
  from the project name; you can change it in the file dialog that
  appears when you click **Print**.

The **Use system print dialog...** button opens the operating system's
native print dialog, which gives access to advanced printer-specific
options (paper tray, duplex, quality, color management, etc.) that the
built-in gLabels controls do not expose.

### 7.3 Print range

The **Print range** section controls which part of the output is
printed. It has two modes, selected by radio buttons:

#### Pages

Print a range of pages. Enter the **from** and **to** page numbers.
This is useful when a large job produces many pages and you only want
to print a subset — for example, to test the first page before
committing to the whole batch.

#### Positions

Print starting from a specific **position** (label slot) on the sheet,
up to a **last position**. This is useful for re-using partially
printed sheets: if you have a sheet of 30 labels and you have already
used the first 12, you can start printing at position 13 to avoid
wasting labels.

Position numbering starts at 1 in the top-left corner of the sheet
and proceeds left-to-right, top-to-bottom.

### 7.4 Merge control

The **Merge control** section appears only when a merge source is
connected (Chapter 6.3). It controls how the merge records are laid
out across copies and pages.

#### Copies

The number of copies to print of each merge record. For example, with
3 records and 2 copies, you get 6 labels total: each record twice.

#### Collation

Two options, shown with example layouts for 3 records and 3 copies:

- **Uncollated** (e.g. `1,1,1   2,2,2   3,3,3`) — all copies of record
  1 are printed first, then all copies of record 2, and so on. This is
  efficient when you want to sort the output by record.
- **Collated** (e.g. `1,2,3   1,2,3   1,2,3`) — the records are printed
  in order, and the whole sequence is repeated for each copy. This is
  useful when each copy is a complete set.

#### Grouping

Two options for how merge groups are laid out across pages:

- **Merge groups are contiguous** — groups flow from one page to the
  next without page breaks. This packs the maximum number of labels per
  sheet.
- **Merge groups start on a new page** — each group begins at the top
  of a fresh page. Use this when you want to keep groups visually
  separated, for example when each group belongs to a different
  recipient.

#### Start groups at position

A spin box (only visible in merge mode) that sets the position on the
sheet where the first merge group begins. Like the Positions option in
the Print range, this lets you re-use partially printed sheets.

### 7.5 Options

The **Options** section contains three checkboxes:

- **print outlines** — draw a thin outline around each label. Useful
  for alignment testing or when you want to cut labels manually from
  plain paper.
- **print crop marks** — print small corner marks at the edges of each
  label, used as cutting guides. Typically used for professional
  finishing.
- **print in reverse (i.e. a mirror image)** — print the entire output
  mirrored. This is needed for transparent or iron-on transfer media
  where the label is applied face-down: the mirror image ensures the
  text reads correctly after transfer.

### 7.6 The preview

The preview area at the right side of the Print tab shows the pages that
will be printed, with the actual merged data substituted into the
objects. Use the **Page** spin box and the **of** label to navigate
between pages.

The preview reflects all Print tab settings: print range, merge control,
and options. If you change the merge source or a variable, switch to
the Print tab (or click within it) to refresh the preview.

### 7.7 Printing

Click the **Print** button to send the job to the selected destination.

- If a printer is selected, the job is sent directly to that printer
  using the current settings.
- If PDF output is selected, a file dialog asks for the output filename;
  the PDF is then written to that location.

For advanced printer options (duplex, tray, quality), use the **Use
system print dialog...** button instead, which hands off to the
operating system's native print dialog.

### 7.8 Sheet labels vs. roll labels

The printing workflow differs slightly between the two media types:

#### Sheet labels

- The template defines a fixed page size (e.g. A4, US Letter) with a
  grid of labels.
- The **Print range** and **Start groups at position** options are
  relevant: they let you re-use partially printed sheets.
- **Copies** in merge control multiplies the number of labels per
  record.

#### Roll labels

- The template defines a roll width and a label height, but no fixed
  page length. The printer feeds the roll and cuts between labels.
- The **Positions** option in the print range is less relevant,
  because there is no fixed grid of slots to skip.
- **Copies** in merge control multiplies the number of labels per
  record, as with sheet labels.
- Some roll printers require the **print in reverse** option for
  certain media types; check your printer's documentation.

> **Tip — where does the roll get cut?**
> With roll labels it is not always obvious where exactly the printer
> will cut between labels. The cut behavior is controlled by the printer
> driver, not by gLabels. To set it, use the **Use system print
> dialog...** button on the Print tab, then open the **Advanced** tab
> in the printer dialog. There you will find a **Cut** option. Most
> users will want **Trim pages** (cut after each label/page), but
> other options may be available depending on the printer driver
> (e.g. no cut, cut at the end of the job). If your labels come out
> joined together or cut in the wrong place, this is the setting to
> check.

### 7.9 Tips and common issues

**"The preview is empty or shows only one label."** Check that a
merge source is connected (Merge tab) if you expected multiple labels,
or that your objects actually contain `${...}` placeholders. Without
placeholders, every label looks the same.

**"The first page prints fine but the rest are blank."** You may have
set the Print range to a single page. Switch to **Pages** with a wider
range, or use the default (all pages).

**"My labels are misaligned on the sheet."** Check that the template
matches the actual product you are printing on. Use the Properties
tab's **Change product** button if the wrong template is selected.
For small drift, use **print outlines** on plain paper first to test
alignment before committing to label stock.

**"I want to print on a partially used sheet."** Use **Positions** in
the Print range, setting the start position to the first unused label
slot (counting from 1 in the top-left).

**"I need to send the job to a printer with specific settings
(duplex, tray, quality)."** Use the **Use system print dialog...**
button to access the operating system's native print dialog, which
exposes all printer-specific options.

**"My roll labels come out joined together or cut in the wrong
place."** The cut position is controlled by the printer driver, not
by gLabels. Use the **Use system print dialog...** button, open the
**Advanced** tab, and set the **Cut** option — usually **Trim pages**.
See the tip in section 7.8 for details.

---

## 8. Batch Mode (CLI)

gLabels ships with a command-line tool, **`glabels-batch`**, that prints
a `.glabels` project without the graphical interface. It is useful for
automation, server-side workflows, and scripting.

This chapter is a short reference. For the concepts behind merge,
variables, and the print options, see Chapters 6 and 7.

### 8.1 Usage

```
glabels-batch [options] project.glabels
```

Exactly one project file is required. Use `-` as the filename to read
the project from stdin.

### 8.2 Options

| Short | Long          | Argument    | Default        | Effect                                      |
|-------|---------------|-------------|----------------|-------------------------------------------------------------------|
| `-p`  | `--printer`   | printer     | system default | Send output to the named printer.                                 |
| `-o`  | `--output`    | filename    | `output.pdf`   | Write output to a file (PDF). Use `-` for stdout.                 |
| `-i`  | `--input`     | source      | (none)         | Set the merge input source (typically a file). Use `-` for stdin. |
| `-s`  | `--sheets`    | n           | 1              | Print n full sheets (simple projects, no merge).                  |
| `-c`  | `--copies`    | n           | 1              | Print n copies (merge projects, or partial sheets).               |
| `-f`  | `--first`     | n           | 1              | Start at position n on the sheet (1-based).                       |
| `-a`  | `--collate`   | (flag)      | off            | Collate merge copies.                                             |
| `-g`  | `--group`     | (flag)      | off            | Start each merge group on a new page.                             |
| `-l`  | `--outlines`  | (flag)      | off            | Print label outlines.                                             |
| `-m`  | `--crop-marks`| (flag)      | off            | Print crop marks.                                                 |
| `-r`  | `--reverse`   | (flag)      | off            | Print in reverse (mirror image).                                  |
| `-D`  | `--define`    | var=value   | (none)         | Set a user variable; can be repeated.                             |
|       | `--help`      |             |                | Show help and exit.                                               |
|       | `--version`   |             |                | Show version and exit.                                            |
| `-V`  | `--Version`   |             |                | Show detailed version information and exit.                       |

### 8.3 Output destination

Pick exactly one of:

- `-p printer` — send to a printer by name.
- `-o file.pdf` — write to a PDF file. Use `-o -` to write to stdout.
- (neither) — send to the system default printer.

### 8.4 Merge input

`-i source` connects a merge data file to the project, just like the
Merge tab in the GUI. Use `-i -` to read from stdin. The merge format is
taken from the project file's saved merge settings.

### 8.5 Variables

`-D name=value` sets a user variable on the command line, overriding
the project's initial value. Repeat the option for multiple variables.
For example:

```
glabels-batch -D company=Acme -D batch=42 project.glabels
```

See Chapter 6.2 for variables and Chapter 6.5 for the `${...}`
placeholder syntax.

### 8.6 Examples

Print a project to the default printer:
```
glabels-batch project.glabels
```

Generate a PDF from a project with a CSV merge source:
```
glabels-batch -i guests.csv -o labels.pdf project.glabels
```

Print 3 copies of a merged project, collated, starting at position 5:
```
glabels-batch -i guests.csv -c 3 -a -f 5 -p MyPrinter project.glabels
```

Print with a variable overridden, to PDF on stdout:
```
glabels-batch -D company="Acme Corp." -o - project.glabels > out.pdf
```

---

## 9. File Format (optional)

This chapter is for power users who want to inspect, generate, or
programmatically edit `.glabels` project files. Most users do not need
it.

### 9.1 Overview

A `.glabels` project file is XML. The structure is defined by the DTD
in [`templates/glabels-4.0.dtd`](../templates/glabels-4.0.dtd) within
the source tree. The top-level element is **`Glabels-document`**:

```
Glabels-document
├── Template          (the product template, inline)
├── Objects+          (one or more object groups)
├── Merge?            (optional merge source)
├── Variables?        (optional user variables)
└── Data*             (optional embedded image files)
```

The `version` attribute on `Glabels-document` identifies the format
version. gLabels 4 writes version 4.0; the legacy gLabels 3 format is
still read by a separate parser but is not written.

### 9.2 Units and lengths

Lengths are written as a number followed by a unit, e.g. `1.5 in`,
`10 mm`, `36 pt`, `2 cm`, `1 pc`. The supported units are:

| Unit | Meaning     |
|------|-------------|
| `pt` | points      |
| `in` | inches      |
| `mm` | millimeters |
| `cm` | centimeters |
| `pc` | picas       |

Internally gLabels stores all lengths in points; other units are
converted on read and write.

### 9.3 Template section

The `Template` element inlines the product template so that a project
file is self-contained. Its attributes include `brand`, `part`,
`description`, and the page dimensions. The label shape is one of:
`Label-rectangle`, `Label-round`, `Label-ellipse`, `Label-cd`, or
`Label-continuous` (roll labels). Each contains one or more `Layout`
elements describing the grid of labels on the sheet.

See the DTD for the full attribute list.

### 9.4 Objects section

Each `Objects` element groups the objects for one label face (or one
side of a two-sided product). It has an `id` and a `rotate` flag. Its
children are the individual objects:

| Element          | Object type |
|------------------|-------------|
| `Object-text`    | text        |
| `Object-box`     | box         |
| `Object-line`    | line        |
| `Object-ellipse` | ellipse     |
| `Object-image`   | image       |
| `Object-barcode` | barcode     |

All objects share `x`, `y` (position) and most share `w`, `h` (size),
plus optional affine transform (`a0`–`a5`) and shadow attributes.
Shape objects (`box`, `ellipse`, `line`) have line and fill color
attributes; colors may be a fixed RGB integer or a `*_field` name
pointing to a merge/variable field. Text objects carry font,
alignment, wrap, and color attributes, with text content in child `<p>`
elements. Barcode objects carry `backend`, `style`, `data`, `text`,
and `checksum` attributes.

### 9.5 Merge section

The optional `Merge` element records the merge format and source
location:

```xml
<Merge type="text/csv" src="/path/to/guests.csv"/>
```

The `type` identifies the backend (e.g. `text/csv`, `text/csv-keys`,
`text/tsv`, `text/colon`, `text/semicolon`, ...). The `src` is the
path to the data file, which is read at print time.

### 9.6 Variables section

The optional `Variables` element lists user-defined variables used in
the project (for example in text objects via `${name}`):

```xml
<Variables>
  <Variable type="string" name="company" value="Acme Corp."
            increment="never" stepSize="0"/>
  <Variable type="numeric" name="serial" value="1"
            increment="per_merge_record" stepSize="1"/>
</Variables>
```

Each Variable is empty (no child elements) and has these attributes,
all required by the DTD:

| Attribute   | Meaning                                                                        |
|-------------|--------------------------------------------------------------------------------|
| `type`      | `string` or `numeric`                                                          |
| `name`      | identifier used in field references                                            |
| `value`     | initial value (written as text in the XML)                                     |
| `increment` | when the value changes: `never`, `per_copy`, `per_merge_record`, or `per_page` |
| `stepSize`  | amount added on each increment (typically unused when `increment` is `never`)  |

This section is a summary; the DTD is authoritative. Behaviour of
increments at print time is defined by the application, not by the
file format alone.

### 9.7 Data section (embedded images)

Image objects reference an external file by path, but when the project
is saved the image data is also embedded in the `Data` section so the
project remains portable. Each `File` element holds one image:

```xml
<Data>
  <File name="logo.png" mimetype="image/png" encoding="base64">
    iVBORw0KGgoAAAANSUhEUgAA...
  </File>
</Data>
```

`mimetype` is `image/png` or `image/svg+xml`; `encoding` is `base64`
(typical) or `cdata`.

### 9.8 A minimal example

```xml
<?xml version="1.0"?>
<Glabels-document version="4.0">
  <Template brand="Avery" part="5160" ...>
    <Label-rectangle id="0" width="2.625 in" height="1 in" round="0 pt">
      <Layout nx="3" ny="10" x0="0.156 in" y0="0.5 in"
              dx="2.834 in" dy="1 in"/>
    </Label-rectangle>
  </Template>
  <Objects id="0" rotate="false">
    <Object-text x="0.1 in" y="0.1 in" w="2.4 in" h="0.3 in"
                 align="left" valign="top">
      <p>Hello, ${Name}!</p>
    </Object-text>
  </Objects>
  <Merge type="text/csv-keys" src="guests.csv"/>
</Glabels-document>
```

This project prints "Hello, *Name*!" for each record in `guests.csv`.

### 9.9 Notes for tool authors

- The DTD is the authoritative reference; this chapter is a summary.
- gLabels is tolerant of missing optional attributes (they take
  documented defaults).
- The file may be gzip-compressed; the parser auto-detects this. This
  is why a `.glabels` file may appear as binary in a text editor.
- When generating files programmatically, validate against the DTD
  before relying on the output.

---

## 10. Getting Help

This final chapter lists where to find more help, how to report bugs,
and how to contribute to gLabels.

### 10.1 In-application help

The **Help** menu in the main window offers three items:

- **User Manual** — opens the user documentation.
- **Report a Bug** — opens the Report Bug dialog, which collects
  version and system information and provides a button to launch the
  issue tracker in your web browser. Use the **Copy** button to copy
  the diagnostic information so you can paste it into a bug report.
- **About** — opens the About dialog, showing the version, license, and
  credits. The **License** button shows the full GPLv3 text; the
  **Website** button opens the project homepage.

### 10.2 Reporting bugs

Bugs are tracked on GitHub at
<https://github.com/j-evins/glabels-qt/issues>.

Before filing a new bug:

1. **Search** the existing issues to avoid duplicates. If you find an
   open issue that matches, add your information as a comment.
2. If none matches, [open a new issue]
   (<https://github.com/j-evins/glabels-qt/issues/new>).
3. Include a **clear title and description**, your gLabels version and
   operating system, and — if possible — a **small example project
   file** (`.glabels`) that reproduces the problem. The Report Bug
   dialog's diagnostic text is a good starting point.

### 10.3 Contributing

Contributions are welcome. The project's contribution guide is in
[`docs/CONTRIBUTING.md`](CONTRIBUTING.md) in the source tree. In short:

- **Bug fixes** — open a pull request with a clear description and the
  relevant issue number. Please read [`docs/CODING-STYLE.md`](CODING-STYLE.md)
  first.
- **New features** — open an issue first to discuss your plans, then
  submit a pull request.
- **New product templates** — read
  [`docs/PRODUCT-TEMPLATES.md`](PRODUCT-TEMPLATES.md) and
  [`docs/TEMPLATE-STYLE.md`](TEMPLATE-STYLE.md), then attach your
  template file(s) to an issue.
- **Translations** — see [`docs/TRANSLATIONS.md`](TRANSLATIONS.md).
- **Packaging** — help is wanted for Windows, macOS, and Linux
  (Flatpak, Snap). Open an issue if you can help.

### 10.4 Build instructions

For building gLabels from source, see:

- [`docs/BUILD-INSTRUCTIONS-LINUX.md`](BUILD-INSTRUCTIONS-LINUX.md)
- [`docs/BUILD-INSTRUCTIONS-WINDOWS.md`](BUILD-INSTRUCTIONS-WINDOWS.md)
- [`docs/BUILD-INSTRUCTIONS-MACOS.md`](BUILD-INSTRUCTIONS-MACOS.md)

### 10.5 License

gLabels is free software licensed under the **GNU General Public
License v3.0**. See [`LICENSE`](../LICENSE) in the project root for
the full text. The bundled `glbarcode` library is licensed under the
LGPL v3; the template database files are under the MIT/X license. See
the [`README.md`](../README.md) for details.

This manual is licensed under the same GPLv3 terms as gLabels itself.

---

*End of the gLabels Minimal User Manual.*
