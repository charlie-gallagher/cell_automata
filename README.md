# Elementary Cellular Automata

### Overview
This program implements one-dimensional elementary cellular automata with
nearest neighbor vision. You can print 1's and 0's to `stdout` or you can export a `.pbm` image where
every 1 becomes a black pixel and every 0 becomes a white pixel. For the moment,
file extensions are fairly 'dumb' in that the user is responsible for using an
appropriate filename and extension.

The available settings are:

- **Width** The number of cells in a line and the width in pixels of the output
- **Height** The number of timesteps to advance and the height in pixels of the
  output.
- **Rule** `cell` can handle any of the 256 possible rules from 0 to 255.
- **Initial position** This controls the first line. There are two initial
  positions: random and single. A random initial position has random 1's and 0's
across the first line. A single initial position has a single 1 in the center of
the line; all other cells are zero.
- **Output file** Must be a valid filename and have the extension `.pbm`.


The defaults for these are:

```
	Height		25
	Width		50
	Rule		26
	Initial		1
	Output		stdout
```


### Build
`cell` runs on both Linux and Windows, and it supports both `gcc` and `cl`
(Microsoft C/C++ Compiler). The two compilers use slightly different builds
and have different commandline interfaces. For `gcc`, compile with the
following command:

```
make -f make_linux
```

The commandline interface follows the POSIX standard of `-x`'s. The arguments
are parsed with `getopt` from `<unistd.h>`.

For Microsoft's C/C++ Compiler, there is a makefile called `make_win`. Build with

```
nmake /f make_win
```

The commandline interface uses the Windows typical `/x` style flags. I built this
parser myself, but it should support normal use fine because the commandline
syntax is extremely simple.


# Usage

### Examples
By default, running `cell` will evaluate to `stdout` using the defaults. This is equivalent to:

```
cell -h 25 -w 50 -r 26 -i 1

height: 25
width: 50
initial: 1
rule: 26
output: (null)
000: 00000000000000000000000001000000000000000000000000
001: 00000000000000000000000010100000000000000000000000
002: 00000000000000000000000100010000000000000000000000
003: 00000000000000000000001010101000000000000000000000
004: 00000000000000000000010000000100000000000000000000
005: 00000000000000000000101000001010000000000000000000
006: 00000000000000000001000100010001000000000000000000
007: 00000000000000000010101010101010100000000000000000
008: 00000000000000000100000000000000010000000000000000
009: 00000000000000001010000000000000101000000000000000
010: 00000000000000010001000000000001000100000000000000
011: 00000000000000101010100000000010101010000000000000
012: 00000000000001000000010000000100000001000000000000
013: 00000000000010100000101000001010000010100000000000
014: 00000000000100010001000100010001000100010000000000
015: 00000000001010101010101010101010101010101000000000
016: 00000000010000000000000000000000000000000100000000
017: 00000000101000000000000000000000000000001010000000
018: 00000001000100000000000000000000000000010001000000
019: 00000010101010000000000000000000000000101010100000
020: 00000100000001000000000000000000000001000000010000
021: 00001010000010100000000000000000000010100000101000
022: 00010001000100010000000000000000000100010001000100
023: 00101010101010101000000000000000001010101010101010
024: 01000000000000000100000000000000010000000000000001
```

This evaluates a line of 50 cells 25 times using rule 26, beginning with a single '1' cell in the center of the line. If you instead want to see what will happen with a random starting position, set `-i 0`. Use `cell --help` to see a help file. The help file, which is also included below, gives a description of the valid arguments along with a description of elementary cellular automata.

If an output file is specified, it is written as a [Raw PBM file](http://netpbm.sourceforge.net/doc/pbm.html). This is easily converted to PNG with ImageMagick, as shown below. The user bears complete responsibility for file naming at the moment, although this is going to change soon.


### Image Processing
The images export as `.pbm`, but this is a short step from PNG, for example. You
can use `imagemagick`'s `convert` to convert to PNG and resize.

```
convert out.pbm -filter Point -resize 800% out.png
```

This converts to PNG at 800% size with no loss in the sharpness of the pixels.
Note that the order and capitalization of the options matters. I've done this for
all rules starting with a single bit, which you can browse in the gallery.


### To-do

- Better output file handling
- Export to PNG rather than PBM
- Write Windows CLI

### Help file
```
Elementary cellular automata

cell [--help] [-h height] [-w width] [-i initial_position] [-r rule]
	[-o output_file]

	--help		Print this help
	-h 		Number of timesteps [1, 19999]
	-w 		Width of the cell line [1, 19999]
	-i 		Initial position** [0,1]
	-r 		Rule [0,255]
	-o		Output file***

**Takes 1 for a single '1' in the middle of the row and 0 for a random row.
***Output file must have the extension '.pbm'

Defaults:
	Height		25
	Width		50
	Rule		26
	Initial		1
	Output		stdout

Details
-------
This program implements one-dimensional cellular automata with uniform
treatment and vision of 1. Consider a circular line of 1's and 0's, so that
each 1 and 0 has exactly one neighbor.

	t=0	011001011000010

You must imagine that the elements on the left and right are neighbors. This
is timestep zero. Each cell makes a decision about what it will be at the next
timestep, a '1' or a '0'. It makes this decision by considering itself and its
two nearest neighbors and consulting a rule such as:

         v
	Situation	010
	Result  	 0

When a 1 has neighbors 0 and 0, it changes to a zero. There are 8 permutations
of 3 bits, so a complete rule must account for 8 situations.

	111	110	101	100	011	010	001	000
	 1	 0	 0	 1 	 0	 1	 1	 1

This is a complete ruleset. Rules are composed of 8 bits, so they can be (and
traditionally are) represented as decimal integers from 0 to 255. Also by
tradition, the possible 3-bit inputs are given in reverse order from 111 to
000 in decreasing base-2 integers. This gives a standard set of 256 rules
which are referred to by their decimal equivalents. The above rule is Rule 151
(0b10010111 = 0n151).

Each cell makes its own decision, and then the next timestep arrives and the
cells change simultaneously. The initial position makes a large difference on
the outcome. Starting with a single 1 in the middle of the line (-i 1) often
has pleasing but predictable results. Starting with a random row, on the other
hand, is less consistent but has potential for more interesting patterns.
Also, the result is different every time. Unfortunately, because the number of
cells in a row is customizable, there is no simple way to add support for
other initial arrangements. It's possible that in the future, I will add more
default initial positions that can be generated for any size line.

-----------------------
Charlie Gallagher, 2021
```

# Selected Gallery
All 256 images are available in the gallery folder. Here, I selected some of the more
interesting patterns. The images are 100x100 (expanded to 400x400 px) with a single
initial position.

#### Rule 13
![rule 13](https://github.com/charlie-gallagher/cell_automata/blob/main/gallery/rule_013.png)

#### Rule 18
![rule 18](https://github.com/charlie-gallagher/cell_automata/blob/main/gallery/rule_018.png)

#### Rule 30
![rule 30](https://github.com/charlie-gallagher/cell_automata/blob/main/gallery/rule_030.png)

#### Rule 45
![rule 45](https://github.com/charlie-gallagher/cell_automata/blob/main/gallery/rule_045.png)

#### Rule 57
![rule 57](https://github.com/charlie-gallagher/cell_automata/blob/main/gallery/rule_057.png)

#### Rule 73
![rule 73](https://github.com/charlie-gallagher/cell_automata/blob/main/gallery/rule_073.png)

#### Rule 129
![rule 129](https://github.com/charlie-gallagher/cell_automata/blob/main/gallery/rule_129.png)

#### Rule 153
![rule 153](https://github.com/charlie-gallagher/cell_automata/blob/main/gallery/rule_153.png)

#### Rule 193
![rule 193](https://github.com/charlie-gallagher/cell_automata/blob/main/gallery/rule_193.png)

---
Charlie Gallagher, 2021
