# Team Reference Document ![CI](https://github.com/nikgaevoy/TRD/workflows/CI/badge.svg?event=push)

_Made by SPb SU LOUD Enough._

[The current version.](main.pdf)

[The version for the World Finals.](TRD.pdf)

## Building

	make

In case you want to do all the steps manually, compile using LuaTeX:

	lualatex --shell-escape main

Note that several runs are required for the correct number of pages and the counter in the header. 

Code formatting is done by ```clang-format```. Run:

	./code/formatted/format.sh
