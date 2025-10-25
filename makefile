default:
	gcc -o program main.c functions.c file-handler.c libraries/config_reader.c libraries/linked_list.c
warn:
	gcc -w -o program main.c functions.c file-handler.c libraries/config_reader.c libraries/linked_list.c
