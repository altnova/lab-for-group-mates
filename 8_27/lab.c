#include <stdio.h>
#include "sockets.h"
#include <stdlib.h>
#include <string.h>


// !  ?  \  :  +  -  ^  *  <  "     #
// 1  2  3  4  5  6  7  8  9  10   11


int fields()
{
	printf("\n\t\t1 - имя\t");
	printf("\t2 - квалификация\t");
	printf("3 - мет. объединение\n");
	printf("\t\t4 - предмет\t");
	printf("5 - адрес\t");
	printf("\t6 - телефон\n\n");
	printf("$  ");
	return get_num();
}

void help()
{
	printf("\n#\tстатус");
	printf("\n!\tсписок\n");
	printf("?\tпомощь\n");
	printf("\\\tвыход\n");
	printf(":id\tвывод записи\n");
	printf("+\tдобавить запись\n");
	printf("-id\tудалить запись\n");
	printf("^\tсортировка данных\n");
	printf("*id\tредактировать запись\n");
	printf("<i.csv\tимпортировать данные\n");
	printf("\"\tпоиск по полю\n\n$  ");
}

void banner()
{
	printf("\nвведите команду, \\ для выхода или ? для помощи\n$  ");
}

// отмотка пробелов до конца ввода
void remote() 
{
	char c = '1';
	while (c != '\n')
		c = getchar();
}

// показывает запись по id
void show(FILE *db)
{
	rec_show(get_num(), db);
}

// создается структура, заполняется и запизивается в базу данных
void add(FILE *db)
{
	teacher abc;

	printf("имя:  ");
	strcpy(abc.name, get_line(stdin));
	
	printf("квалификация:  ");
	abc.qual = atoi(get_line(stdin));

	printf("мет. объединение:  ");
	strcpy(abc.un, get_line(stdin));
	
	printf("предмет:  ");
	strcpy(abc.subj, get_line(stdin));

	printf("адрес:  ");
	strcpy(abc.adrs, get_line(stdin));
	
	printf("телефон:  ");
	abc.tel = atoll(get_line(stdin));

	rec_add(abc, db);
}

void delete(FILE *db)
{
	rec_delete(get_num(), db);
}

// если введено неверное число, то сортируется по имени
void sort(FILE *db)
{
	int i = fields();

	if (i < 1 || i > 6)
		rec_sort(1, db);
	else 
		rec_sort(i, db);
}

// редактирование записи по одному полю
void edit(FILE *db)
{
	char line[150];
	int field, id = get_num();
	rec_show(id, db);
	field = fields();
	printf("введите новые данные\n$  ");
	if (field && field < 7) {
		strcpy(line, get_line(stdin));
		rec_edit(id, field, line, db);
		printf("отредактировано\n");
	}
	else {
		printf("неверный ввод\n");
	}
}

// загрузка из текстового файла
void load(FILE *db) 
{
	int i = 0;
	char filename[101];
	strcpy(filename, get_line(stdin));
	while(filename[i] != '\n' && i < 100 && filename[i] != '\0')
		i++;
	filename[i] = '\0';
	csv_load(filename, db);
}

// поиск по полю
void search(FILE *db)
{
	char line[151];
	int field = fields();
	if (field < 1 || field > 6)
		field = 1;
	printf("введите ключевое слово/значение\n$  ");
	strcpy(line, get_line(stdin));
	rec_search(field, line, db);
}

void status()
{
	indx main;
	rewind(idx);
	fread(&main, sizeof(indx), 1, idx);
	rewind(idx);
	printf("*************************************************\n");
	printf("*  %d записей  |  %d удалений  |  %d удаленных\t*\n", (main.id - main.del), main.del, main.free);
	printf("*************************************************\n");
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
		null_idx();
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
