default:
	gcc -o program main.c file-handler.c libraries/config_reader.c libraries/linked_list.c
warn:
	gcc -w -o program main.c file-handler.c libraries/config_reader.c libraries/linked_list.c
