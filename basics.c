#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//////////////////////////////////////////////////////////

FILE* idx;

typedef struct Teacher {
	int id;
	char del;
	char name[150];
	int qual;
	char un[150];
	char subj[100];
	char adrs[150];
	long long tel;
} teacher;


typedef struct Idx {
	int id;
	int del;
	int free;
} indx;

void status();

void BAR(char* s)
{
	printf("==============================================================");
	printf("==============================================================\n%s", s);
}

// число из stdin
int get_num()
{
	char input[251];
	fgets(input, 250, stdin);
	return atoi(input);
}

// 	строка до переноса каретки или '\0' в потоке filename
char* get_line(FILE *filename)
{
	int i;
	char *line, c = '1';
	line = malloc(sizeof(char)* 150);

	for (i = 0; c != '\n' && i < 150 && c != '\0' && !feof(filename); i++) {
		c = getc(filename);
		if (c != '\n' && c != '\0')
			line[i] = c;
		else 
			line[i] = '\0';
	}

	if (i == 150) {
		line[149] = '\0';
		while (c != '\n' && c != '\0')
			c = getc(filename);
	}
	return line;
}

// структура из текста. делиметр '\n'
teacher get_struct(FILE* txt)
{
	int ptr;
	char line[150]; 
	teacher abc;
	
	abc.del = 0;

	strcpy(abc.name, get_line(txt));

	if (feof(txt)) {
		abc.del = 1;
		return abc;
	}


	strcpy(line, get_line(txt));
	abc.qual = atoi(line);

	strcpy(abc.un, get_line(txt));
	strcpy(abc.subj, get_line(txt));
	strcpy(abc.adrs, get_line(txt));

	strcpy(line, get_line(txt));
	abc.tel = atoll(line);

	return abc;
}

// возвращает структуру по указателю в базе данных
teacher read_struct(int ptr, FILE *db)
{
	teacher abc;
	fseek(db, ptr, SEEK_SET);
	fread(&abc, sizeof(teacher),1, db);
	return abc;
}

//	выписывант стуктуру в конец базы данных
void write_struct(teacher abc, FILE *db) 
{
	fwrite(&abc, sizeof(teacher),1, db);
}


void print_banner()
{
	BAR("");
	printf("id\t  имя\t\t квалификация\t   мет. объед.\t\t предмет\t\t адреc\t\t телефон\n");
}

// выписывает структуру по указателю
void print_table(int ptr, FILE* db)
{
	teacher abc = read_struct(ptr, db);

	BAR("");
	printf("[%d]  %s\t\t   ", abc.id, abc.name);
		
	switch (abc.qual) {
		case 1:
		printf("первая\t");
		break;

		case 2:
		printf("вторая\t");
		break;

		case 3:
		printf("высшая\t");
		break;

		default:
		printf("--\t\t");
	}

	printf("     %s\t\t %s\t\t %s\t\t%lli\n", abc.un, abc.subj, abc.adrs, abc.tel);
}


//	ищет запись по id
// возвращает указатель или -1
int find_rec(int id, FILE *db)
{
	int ptr = 0;
	teacher abc;
	rewind(db);
	while (!feof(db)) {
		abc = read_struct(ptr, db);
		if (abc.id == id && !abc.del)
			return ptr;
		ptr = ftell(db);
	}

	return -1;
}


//	проверяет структуру на значение в заданном поле
char scan_struct(teacher abc, int field, char* line)
{
	if (abc.del)
		return 0;

	if (field == 2 || field == 6) { 
		if (field == 2) 
			if (abc.qual == atoi(line))
				return 1;
		

		if (field == 6) 
			if (abc.tel == atoll(line))
				return 1;
	}
	else {
		switch (field) {
			case 1:
			if (strcasestr((char*)abc.name, line) != NULL)
				return 1;
			break;

			case 3:
			if (strcasestr((char*)abc.un, line) != NULL)
				return 1;
			break;

			case 4:
			if (strcasestr((char*)abc.subj, line) != NULL)
				return 1;
			break;

			case 5:
			if (strcasestr((char*)abc.adrs, line) != NULL)
				return 1;
			break;
		}
	}
	return 0;
}

//	увеличивает одну из частей индекса на 1
void inc_idx(int k)
{
	indx main;
	rewind(idx);
	fread(&main, sizeof(indx), 1, idx);

	if (k == 1)
		main.del = main.del + 1;

	if (k == 2)
		main.free = main.free + 1;

	if (k == 0)
		main.id = main.id + 1;

	rewind(idx);
	fwrite(&main, sizeof(indx),1,idx);
	rewind(idx);
}

//	увеличивает одну из частей индекса на 1
void dec_idx(int k)
{
	indx main;
	rewind(idx);
	fread(&main, sizeof(indx), 1, idx);
	if (k == 1)
		main.del = main.del - 1;
	if (k == 2)
		main.free = main.free -1 ;
	if (k == 0)
		main.id = main.id - 1;

	rewind(idx);
	fwrite(&main, sizeof(indx),1,idx);
	rewind(idx);
}

void null_idx()
{
	int i = 0;
	rewind(idx);
	fwrite(&i, sizeof(int),1 , idx);
	fwrite(&i, sizeof(int),1 , idx);
	fwrite(&i, sizeof(int),1 , idx);
	rewind(idx);
}


int field_cmp(teacher a, teacher b, int field)
{
	switch(field) {
		case 1:
		return strcmp(a.name, b.name);
		case 2:
		return (a.qual > b.qual) ? 1 : 0;
		case 3:
		return strcmp(a.un, b.un);
		case 4:
		return strcmp(a.subj, b.subj);
		case 5:
		return strcmp(a.adrs, b.adrs);
		case 6:
		return (a.tel > b.tel) ? 1 : 0;
	}
}
