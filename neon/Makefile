CC		= gcc
SRC_CC          = $(shell find ./ -name '*.c')

.PHONY : all
all : rgb matrix

rgb : rgb.c
	$(CC) -g -o3 rgb.c -o exe_rgb_o3
	objdump -d exe_rgb_o3 > disasm_rgb_o3

matrix : matrix.c
	$(CC) -g -o3 matrix.c -o exe_matrix_o3
	objdump -d exe_matrix_o3 > disasm_matrix_o3

.PHONY: clean
clean :
	rm exe* disasm*
