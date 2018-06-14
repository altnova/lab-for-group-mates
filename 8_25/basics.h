#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//////////////////////////////////////////////////////////

FILE* idx;

typedef struct Plant {
	int id;
	char del;
	char name[250];
	char range[250];
	int age;
	char bloom;
} plant;


typedef struct Idx {
	int id;
	int del;
} indx;

void status();

void BAR(char* s)
{
	printf("====================================================");
	printf("====================================================\n%s", s);
}

//	определяет да или нет
char y_n(char* line) {
	if (line[0] == 'y' || line[1] == 'y' || line [0] == 'Y' || line[1] == 'Y')
		return 1;
	return 0;
}

// 	строка до переноса каретки или '\0' в потоке filename
char* get_line(FILE *filename)
{
	int i;
	char *line, c = '1';
	line = (char*)malloc(sizeof(char)* 250);

	for (i = 0; c != '\n' && i < 250 && c != '\0' && !feof(filename); i++) {
		c = getc(filename);
		if (c != '\n' && c != '\0')
			line[i] = c;
		else 
			line[i] = '\0';
	}

	if (i == 250) {
		line[249] = '\0';
		while (c != '\n' && c != '\0')
			c = getc(filename);
	}
	return line;
}

// число из stdin
int get_num()
{
	char input[251];
	strcpy(input, get_line(stdin));
	return atoi(input);
}

// структура из текста. делиметр '\n'
plant get_struct(FILE* txt)
{
	char line[250]; 
	plant plnt;
	plnt.del = 0;

	strcpy(plnt.name, get_line(txt));

	if (feof(txt)) {
		plnt.del = 1;
		return plnt;
	}

	strcpy(plnt.range, get_line(txt));

	strcpy(line, get_line(txt));
	plnt.age = atoi(line);

	strcpy(line, get_line(txt));
	if (line[0] == 'y' || line[1] == 'y') 
		plnt.bloom = 1;
	else 
		plnt.bloom = 0;

	return plnt;
}

// возвращает структуру по указателю в базе данных
plant read_struct(int ptr, FILE *db)
{
	plant plnt;
	fseek(db, ptr, SEEK_SET);
	fread(&plnt, sizeof(plant),1, db);
	return plnt;
}

//	выписывант стуктуру в конец базы данных
void write_struct(plant plnt, FILE *db) 
{
	fwrite(&plnt, sizeof(plant),1, db);
}


void print_banner()
{
	BAR("");
	printf("id\t  название\t\t  ареал\t\t\t\tвозраст\t\t цветение\t\n");
}

// выписывает структуру по указателю
void print_table(int ptr, FILE* db)
{
	plant plnt = read_struct(ptr, db);

	BAR("");
	printf("[%d]  %s\t\t\t %s\t\t\t  %d\t\t ", plnt.id, plnt.name, plnt.range, plnt.age);
		
	switch (plnt.bloom) {
		case 1:
		printf("цветет\t");
		break;

		case 2:
		printf("не цветет\t");
		break;

		default:
		printf("--\t\t");
	}

	printf("\n");
}


//	ищет запись по id
// возвращает указатель или -1
int find_rec(int id, FILE *db)
{
	int ptr = 0;
	plant plnt;
	rewind(db);
	while (!feof(db)) {
		plnt = read_struct(ptr, db);
		if (plnt.id == id && !plnt.del)
			return ptr;
		ptr = ftell(db);
	}

	return -1;
}


//	проверяет структуру на значение в заданном поле
char scan_struct(plant plnt, int field, char* line)
{
	if (plnt.del)
		return 0;


		switch (field) {
			case 1:
			if (strstr((char*)plnt.name, line) != NULL)
				return 1;
			break;

			case 2:
			if (strstr((char*)plnt.range, line) != NULL)
				return 1;

			case 3:
			if (atoi(line) == plnt.age)
				return 1;
			break;

			case 4:
			if ( (y_n(line) && plnt.bloom) || (!y_n(line) && !plnt.bloom))
				return 1;
			break;
		}
	return 0;
}

//	увеличивает одну из частей индекса на 1
void inc_idx(int k)
{
	indx main;
	rewind(idx);
	fread(&main, sizeof(indx), 1, idx);
	if (k)
		main.del += 1;
	else
		main.id += 1;

	rewind(idx);
	fwrite(&main, sizeof(indx),1,idx);
	rewind(idx);
}

int field_cmp(plant a, plant b, int field)
{
	switch(field) {
		case 1:
		return strcmp(a.name, b.name);
		case 2:
		return strcmp(a.range, b.range);
		case 3:
		return (a.age > b.age) ? 1 : 0;
		case 4:
		return (a.bloom > b.bloom) ? 1 : 0;
	}
}