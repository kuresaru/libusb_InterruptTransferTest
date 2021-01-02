test: main.c
	gcc -o $@ $< -Wl,-Bstatic -lusb-1.0