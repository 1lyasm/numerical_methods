compile:
	clang -g -fsanitize=address -Wall \
		-Wno-unsafe-buffer-usage \
		-Wno-format-nonliteral \
		-Wno-padded \
		src/main.c -o main
run:
	./main
