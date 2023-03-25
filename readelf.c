#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *input_path;


int main(int argc, char **argv) {
	if (argc == 1) {
		printf("Usage: %s <filename>\n", argv[0]);
		return 0;
	}

	input_path = argv[1];

	ELF *elf = malloc(sizeof(ELF));

	FILE *f = fopen(input_path, "rb");

	if (f == NULL) {
		printf("Файл %s не найден!\n", input_path);
		return 2;
	}

	fread(elf, sizeof(ELF), 1, f);

	if (strcmp(elf->magic, "ELF\0\0\0") != 0) {
		printf("Ошибка чтение заголовка!\n");
		fclose(f);
		free(elf);
		return 1;
	}

	printf("Заголовок:\n\tверсия: %d\n\tтип: %d\n\tточка входа: %d\n\tсекция кода: %d\n\tсекция имён: %d\n\tсекция адресов: %d\n",
			elf->version, elf->type, elf->entry,
			elf->code_entry, elf->name_entry, elf->addr_entry);


	printf("\nСекция кода:\n");

	if (elf->code_entry == 0) {
		printf("\tотсутсвует\n");
	} else {
		Code_sec *code;
		unsigned long size;
		fread(&size, sizeof(long), 1, f);

		code = malloc(size);

		code->size = size;
		fread(code->data, size - sizeof(long), 1, f);

		printf("\tразмер: %d\n",
				code->size);

		putc('\t', stdout);

		for (int i = 0; i < code->size - sizeof(long); i++) {
			printf("%02x ", code->data[i] & 0xff);
		}

		putc('\n', stdout);

		free(code);
	}


	printf("\nСекция имён:\n");

	if (elf->name_entry == 0) {
		printf("\tотсутсвует\n");
	} else {
		Name_sec *name;
		unsigned long size;
		fread(&size, sizeof(long), 1, f);

		name = malloc(size);

		name->size = size;
		fread(name->elems, size - sizeof(long), 1, f);

		printf("\tразмер: %d\n",
				name->size);

		unsigned long count = (size - sizeof(long)) / sizeof(Name_sec_elem);

		for (int i = 0; i < count; i++) {
			printf("\t%s: %d\n",
					name->elems[i].name, name->elems[i].offset);
		}

		free(name);
	}


	printf("\nСекция адресов:\n");

	if (elf->addr_entry == 0) {
		printf("\tотсутсвует\n");
	} else {
		Addr_sec *addr;
		unsigned long size;
		fread(&size, sizeof(long), 1, f);

		addr = malloc(size);

		addr->size = size;
		fread(addr->elems, size - sizeof(long), 1, f);

		printf("\tразмер: %d\n",
				addr->size);

		unsigned long count = (size - sizeof(long)) / sizeof(Addr_sec_elem);

		for (int i = 0; i < count; i++) {
			printf("\t%d: %s\n",
					addr->elems[i].offset, addr->elems[i].name);
		}

		free(addr);
	}

	free(elf);

	fclose(f);
}
