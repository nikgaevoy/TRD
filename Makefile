main: format
	lualatex --shell-escape main && lualatex --shell-escape main && lualatex --shell-escape main

format:
	code/formatted/format.sh
