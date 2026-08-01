# Quick Start Guide — gLabels Label Designer

Welcome aboard! This quick-start guide gets you from "just installed gLabels" to "printed my first sheet of labels" in a few short steps. It's a friendly shortcut through the full **USER-MANUAL.md** — perfect if you want to dive in right away and look up the details later.

**Applies to:** glabels-qt 3.99 / 4.0 development branch

> Want the full picture? The complete [User Manual](USER-MANUAL.md) covers every corner of gLabels in depth. This guide just gets you moving fast.

---

## 1. Two Concepts to Know Before You Start

gLabels keeps things refreshingly simple once you know these two words:

- **Template** — describes the *physical* product you're printing on (an Avery sheet, a Herma sheet, a Dymo roll, ...). It's not your design — it's the paper.
- **Project** — your actual design: the text, images, barcodes, and settings you place on top of a template. Saved as a `.glabels` file.

In short: *the template is the paper, the project is what you put on it.* Everything else builds on that.

---

## 2. Start Your First Project

1. Launch gLabels — you'll land on the **Welcome** tab.
2. Click **New...** — this opens the **Select Product** dialog.
3. On the **Search all** tab, type a brand or part number (e.g. "Avery 5160") and pick your product from the list. Don't have your exact product? No worries — chapter 3 of the manual shows you how to design a custom template.
4. Click **OK**.

You're now looking at the **Edit** tab with a blank label outline, ready to go.

---

## 3. Add Some Objects

The **Edit** tab is your canvas, and the toolbar on the left is your toy box:

| Tool     | Adds...                         |
|----------|---------------------------------|
| Text     | Any text you like               |
| Box      | A rectangle/frame               |
| Line     | A straight divider              |
| Ellipse  | A circle or oval                |
| Image    | A logo or picture               |
| Barcode  | Code39, QR, EAN13, and more     |

**To create an object:** click its tool, then click-and-drag on the canvas. Release, and it's created and selected — the toolbar automatically hops back to the Arrow (selection) tool so you can fine-tune it right away.

Once an object is selected, the **Object Editor** panel on the right lets you set exact position, size, colors, fonts, and more.

**Handy tricks:**
- Control-click to select multiple objects.
- Drag corner handles to resize, drag the body to move.
- Use **Objects → Align** or **Objects → Center** to line things up neatly.
- Cut/copy/paste/delete all work exactly as you'd expect (Ctrl+X/C/V, Del).

---

## 4. Fill In the Blanks: Variables & Merge

Here's the part that makes gLabels really shine — and the part that trips up newcomers. Both use the same `${name}` placeholder syntax, so let's clear it up right away with one simple question:

```
Do you want to print MANY different labels at once?
  yes → use MERGE
  no  → Do you want to reuse the same value in several spots on ONE label?
          yes → use VARIABLES
          no  → just type the text — no magic needed!
```

### Variables — reuse a value across one design

1. Go to the **Variables** tab, click **Add**.
2. Give it a **Name** (e.g. `company`), a **Type** (String, Integer, Float, or Color), and a starting **Value**.
3. Back on the **Edit** tab, type `${company}` anywhere you'd like that value to appear.
4. Change the variable once — every instance updates. ✨

### Merge — print a whole stack from a data file

1. Go to the **Merge** tab.
2. Pick the **Format** that matches your file (CSV, TSV, etc. — with or without a header row).
3. Click **Browse...** and select your data file. The **Records** table fills in instantly.
4. On the **Edit** tab, use `${column-name}` placeholders (e.g. `${Name}`, `${City}`) in your text objects.
5. Head to **Print** — the preview now shows one page per record!

> **Tip:** Merge fields and variables can even share a name — the merge field simply wins, letting the variable act as a friendly fallback.

For the full syntax (formatting numbers, default values, optional newlines) check out [`SUBSTITUTION-FIELD-SPEC.md`](SUBSTITUTION-FIELD-SPEC.md) — it's more powerful than it first looks!

---

## 5. Print It!

Head to the **Print** tab — this is where the magic becomes paper (or PDF).

1. **Destination** — pick a printer from the list, or choose to export as a **PDF**.
2. **Print range** — print all pages, a specific range, or start at a particular label position (great for reusing a partially-used sheet!).
3. **Options** — toggle **print outlines** or **crop marks** if you want cutting guides.
4. Check the **preview** on the right — it shows your actual merged data.
5. Hit **Print**.

Need advanced printer settings like duplex or paper tray? Click **Use system print dialog...** for full OS-level control.

> **Printing on a roll?** If your labels come out joined together or cut in the wrong spot, that's a printer-driver setting, not a gLabels one — check the **Advanced → Cut** option in your system print dialog.

---

## 6. Save Your Work

Don't forget: **File → Save** (or **Save As**) stores your design as a `.glabels` file. gLabels will remember it on the **Welcome** tab and in **File → Open Recent**, so picking up where you left off is effortless.

---

## 7. Where to Go From Here

You now know enough to design and print your first batch of labels — nice work! For anything you want to dig deeper into, the full [User Manual](USER-MANUAL.md) has you covered:

| Want to know more about...           | See chapter                      |
|--------------------------------------|----------------------------------|
| Designing a custom template          | Chapter 3.3                      |
| Object properties in detail          | Chapter 4                        |
| All the tabs explained               | Chapter 5                        |
| Merge & Variables, in depth          | Chapter 6                        |
| Printing options in detail           | Chapter 7                        |
| Batch printing from the command line | Chapter 8 (`glabels-batch`)      |
| The `.glabels` XML file format       | Chapter 9                        |
| Getting help & reporting bugs        | Chapter 10                       |

Happy labeling!

---

*This quick-start guide is licensed under the GNU General Public License v3.0, same as gLabels itself.*
