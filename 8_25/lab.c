#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sockets.c"

// !  ?  \  :  +  -  ^  *  <  "     #
// 1  2  3  4  5  6  7  8  9  10   11


int fields()
{
	printf("\n\t1 - название\t");
	printf("\t2 - ареал\t\n");
	printf("\t3 - возраст");
	printf("\t\t4 - цветение\t\n");
	printf("$  ");
	return get_num();
}

void help()
{
	printf("\n\t#\tстатус\n");
	printf("\t!\tсписок\n");
	printf("\t?\tпомощь\n");
	printf("\t\\\tвыход\n");
	printf("\t:id\tвывод записи\n");
	printf("\t+\tдобавить запись\n");
	printf("\t-id\tудалить запись\n");
	printf("\t^\tсортировка данных\n");
	printf("\t*id\tредактировать запись\n");
	printf("\t<i.csv\tимпортировать данные\n");
	printf("\t\"\tпоиск по полю\n\n$  ");
}

void banner()
{
	printf("\nвведите команду, \\ для выхода или ? для помощи\n$  ");
}

void remote() 
{
	char c = '1';
	while (c != '\n')
		c = getchar();
}


void show(FILE *db)
{
	int id = get_num();
	rec_show(id, db);
}

void add(FILE *db)
{
	plant plnt;
	char line[250];

	printf("имя:  ");
	strcpy(line,get_line(stdin));
	strcpy(plnt.name, line);

	printf("ареал:  ");
	strcpy(line,get_line(stdin));
	strcpy(plnt.range, line);

	printf("возраст:  ");
	strcpy(line,get_line(stdin));
	plnt.age = atoi(line);
	
	printf("цветение [y/n]:  ");
	strcpy(line,get_line(stdin));
	plnt.bloom = y_n(line);
	
	plnt.id = -1;
	plnt.del = 0;

	rec_add(plnt, db);
}

void delete(FILE *db)
{
	int id = get_num();
	printf("deleted\n");
	rec_delete(id, db);
}

void sort(FILE *db)
{
	int i = fields();

	rec_sort((i < 1 || i > 4) ? 1 : i  ,db);
}

void edit(FILE *db)
{
	char line[250];
	int field, id = get_num();
	rec_show(id, db);
	field = fields();
	printf("$  ");
	if (field && field < 5) {
		strcpy(line, get_line(stdin));
		rec_edit(id, field, line, db);
		printf("отредактировано\n");
	}
	else {
		printf("неверный ввод\n");
	}
}

void load(FILE *db) 
{
	int i = 0;
	char filename[101];
	strcpy(filename, get_line(stdin));

	/*
	while(filename[i] != '\n')
		i++;
	filename[i] = '\0';
	*/

	txt_load(filename, db);
}

void search(FILE *db)
{
	char line[151];
	int field = fields();

	field  = (field < 1 || field > 4) ? 1 : field; 
	printf("$  ");
	strcpy(line, get_line(stdin));
	rec_search(field, line, db);
}

void status()
{
	indx main;
	rewind(idx);
	fread(&main, sizeof(idx), 1, idx);
	rewind(idx);
	printf("**************************************\n");
	printf("*  %d записей и %d удаленных\n", main.id - main.del, main.del);
	printf("**************************************\n");
}

////////////////////////////////////////////////////////////////////////////////////
//	возвращает команду пользователя
char input() 
{
	char c = getchar();
	switch(c) {
		case '!':
			return 1;
		case '?':
			return 2;
		case '\\':
			return 3;
		case ':':
			return 4;
		case '+':
			return 5;
		case '-':
			return 6;
		case '^':
			return 7;
		case '*':
			return 8;
		case '<':
			return 9;
		case '"':
			return 10;
		case '#':
			return 11;
		default:
			return 0;
	}
}

char process(FILE *db)
{

	switch(input()) {
		case 3:
		return 0;

		case 1:
		remote();
		list(db);
		break;

		case 2:
		remote();
		help();
		return 1;

		case 4:
		show(db);
		break;

		case 5:
		remote();
		add(db);
		break;

		case 6:
		delete(db);
		break;

		case 7:
		remote();
		sort(db);
		break;

		case 8:
		edit(db);
		break;

		case 9:
		load(db);
		break;

		case 10:
		remote();
		search(db);
		break;

		case 11:
		remote();
		status();
		break;
		default:
		remote();
		banner();
		return 1;
	}

	printf("$  ");
	return 1;
}


int main()
{
	int i = 0;
	FILE *db = fopen("src/db.dat", "r");
	if (!db) 
		db = fopen("src/db.dat", "w+b");
	else {  
		fclose(db);
		db = fopen("src/db.dat", "r+b");
	}


	idx = fopen("src/idx.dat", "r");

	if (!idx) {
		idx = fopen("src/idx.dat", "w+b");
		rewind(idx);
		fwrite(&i, sizeof(int),1 , idx);
		fseek(db, sizeof(int), SEEK_SET);
		fwrite(&i, sizeof(int),1 , idx);
		rewind(idx);
	}
	else {
		fclose(idx);
		idx = fopen("src/idx.dat", "r+b");
	}

	banner();
	while(process(db));
	printf("\n");
	return 0;
}
