
  all:	normal_mode.c	normal_help.c	command_mode.c	terminal_cdr.c	terminal_alter.c	terminal_dir.c
	gcc	-o	filemanager	normal_mode.c	normal_help.c	command_mode.c	terminal_cdr.c	terminal_alter.c	terminal_dir.c

  clean: 
	$(RM)	normal
