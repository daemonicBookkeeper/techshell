techshell.o : example_tokenizer.c tokenizer.h techshell2.c
	gcc -o techshell example_tokenizer.c tokenizer.h techshell2.c
