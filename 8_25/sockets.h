#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "basics.h"

//////////////////////////////////////////////////////////////////////////
//							ДОБАВЛЕНИЕ									//

void rec_add(plant plnt, FILE *db) 
{
	fseek(db, 0, SEEK_END);
	if (plnt.id == -1) {
		inc_idx(0);
		fread(&plnt.id, sizeof(int), 1, idx);
	}
	printf("создана запись [%d]\n", plnt.id);
	fwrite(&plnt, sizeof(plant), 1, db);
}


//////////////////////////////////////////////////////////////////////////
//						ПОИСК СТРУКТУРЫ									//		

void rec_show(int id, FILE *db)
{
	int ptr = find_rec(id, db);
	if (ptr >= 0) {
		print_table(ptr, db);
		BAR("\n");
	}
	else
		printf("запись не существует\n");
}


//////////////////////////////////////////////////////////////////////////
//						УДАЛЕНИЕ 										//

void rec_delete(int id, FILE *db)
{
	int ptr = find_rec(id, db);
	char c = 1;

	if (ptr == -1) 
		printf("нет такой записи\n");
	else {
		inc_idx(1);
		fseek(db, ptr + sizeof(int), SEEK_SET);
		fwrite(&c, 1, sizeof(char), db);
		printf("запись %d удалена\n", id);
	}
}

//////////////////////////////////////////////////////////////////////////
//						РЕДАКТИРОВАНИЕ									//

void rec_edit(int id, int field, char* line, FILE *db)
{
	plant plnt;
	long long i;
	int k, ptr = find_rec(id, db);

	if (ptr == -1)
		return;

	plnt = read_struct(ptr, db);
	rec_delete(id, db);

	switch (field) {
		case 1:
			strcpy(plnt.name, line);
			break;
		case 2:
			strcpy(plnt.range, line);
			break;

		case 3:
			plnt.age = atoi(line);
			break;
		case 4:
			plnt.bloom = y_n(line);
			break;
	}	

	plnt.del = 0;
	plnt.id = -1;

	rec_add(plnt, db);
} 


//////////////////////////////////////////////////////////////////////////
//					ПОИСК ПО ЗАДАННОМУ ПОЛЮ								//

void rec_search(int field, char* line, FILE *db)
{
	plant plnt;
	int ptr = 0, par;
	rewind(db);
	while (!feof(db)) {
		plnt = read_struct(ptr, db);
		if (feof(db)) {
			BAR("\n");
			return;
		}
		par = scan_struct(plnt, field, line);
		if (par && !plnt.del) 
			print_table(ptr, db);
		ptr += sizeof(plant);
	}
	BAR("\n");
}

//////////////////////////////////////////////////////////////////////////
//							ЗАГРУЗКА 									//

void txt_load(char* filename, FILE* db)
{
	plant plnt;
	int i = 0;
	FILE *f = fopen(filename, "r");
	if (!f) {
		printf("нет такого файла\n");
		return ;
	}


	fwrite(&i, sizeof(int), 1, idx);
	fwrite(&i, sizeof(int), 1, idx);
	rewind(idx);
	if (db) {
		fclose(db);
		db = fopen("src/db.dat", "w+b");
	}
	rewind(db);
	rewind(f);

	while(!(feof(f))) {

		plnt = get_struct(f);

		if (!feof(f) && !plnt.del) { 
			plnt.id = ++i;
			inc_idx(0);
			write_struct(plnt, db);
	    }
	}
	rewind(idx);
	printf("загружено\n");
	fclose(f);
}

//////////////////////////////////////////////////////////////////////////
//							ВЫВОД	 									//

void list(FILE *db)
{
	int ptr;
	plant plnt;

	status();
	printf("\n");
	print_banner();
	rewind(db);

	for (ptr = ftell(db); !(feof(db)); ptr = ftell(db)) {
		fread(&plnt, sizeof(plant), 1, db);
		if (!plnt.del && !feof(db)) 
			print_table(ptr, db);
	}
	BAR("\n");
}

//////////////////////////////////////////////////////////////////////////
//							СОРТИРОВКА 									//

void rec_sort(int field, FILE* db) 
{
	indx in;
	int i, j, size;
	plant *array, plnt;
	rewind(idx);
	fread(&in, sizeof(indx), 1, idx);
	size = in.id - in.del;
	array = (plant*)malloc(sizeof(plant) * size);
	status();
	rewind(db);

	// заполнение массива структур из базы
	for (i = 0; i < size; i++) {
		fread(&array[i], sizeof(plant), 1, db);
		if (array[i].del)
			i--;
	}


	// 	пузырьковая сортировка
	for (i = 0; i < size - 1; i++) {
		for (j = 0; j < size - i - 1; j++) {
			if (field_cmp(array[j], array[j+1], field) > 0) {
				plnt = array[j + 1];
				array[j + 1] = array[j];
				array[j] = plnt;
			}
		}
	}

	print_banner();
	for (i = 0; i < size; i++) 
		print_table(find_rec(array[i].id, db), db);

	BAR("\n");

	free(array);
}

