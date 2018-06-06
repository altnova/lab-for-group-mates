#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "basics.c"

void rec_add(teacher abc, FILE *db);

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

	if (ptr == -1) {
		printf("нет такой записи\n");
		return;
	}
	inc_idx(2);
	inc_idx(1);
	fseek(db, ptr + sizeof(int), SEEK_SET);
	fwrite(&c, 1, sizeof(char), db);
	printf("запись %d удалена\n", id);
	
}

//////////////////////////////////////////////////////////////////////////
//						РЕДАКТИРОВАНИЕ									//

void rec_edit(int id, int field, char* line, FILE *db)
{
	teacher abc;
	long long i;
	int k, ptr = find_rec(id, db);

	if (ptr == -1)
		return;

	abc = read_struct(ptr, db);
	rec_delete(id, db);

	switch (field) {
		case 1:
			strcpy(abc.name, line);
			break;
		case 2:
			abc.qual = atoi(line);
			break;

		case 3:
			strcpy(abc.un, line);
			break;
		case 4:
			strcpy(abc.subj, line);
			break;
		case 5:
			strcpy(abc.adrs, line);
			break;
		case 6:
			abc.tel = atoll(line);
	}	
	rec_add(abc, db);
} 


//////////////////////////////////////////////////////////////////////////
//					ПОИСК ПО ЗАДАННОМУ ПОЛЮ								//

void rec_search(int field, char* line, FILE *db)
{
	teacher abc;
	int ptr = 0, par;
	rewind(db);
	while (!feof(db)) {
		abc = read_struct(ptr, db);
		if (feof(db)) {
			BAR("\n");
			return;
		}
		par = scan_struct(abc, field, line);
		if (par && !abc.del) 
			print_table(ptr, db);
		ptr += sizeof(teacher);
	}
	BAR("\n");
}

//////////////////////////////////////////////////////////////////////////
//							ЗАГРУЗКА 									//

void csv_load(char* filename, FILE* db)
{
	teacher abc;
	int i = 0;
	FILE *f = fopen(filename, "r");
	if (!f) {
		printf("нет такого файла\n");
		return ;
	}
	// обнуление индекса
	null_idx();

	if (db) {
		fclose(db);
		db = fopen("src/db.dat", "w+b");
	}
	rewind(db);
	rewind(f);

	while(!(feof(f))) {
		abc = get_struct(f);

		if (!feof(f) && !abc.del) { 
			abc.id = ++i;
			inc_idx(0);
			write_struct(abc, db);
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
	teacher abc;

	status();
	printf("\n");
	print_banner();
	rewind(db);

	for (ptr = ftell(db); !(feof(db)); ptr = ftell(db)) {
		fread(&abc, sizeof(teacher), 1, db);
		if (!abc.del && !feof(db)) 
			print_table(ptr, db);
	}
	BAR("\n");
}


//////////////////////////////////////////////////////////////////////////
//							ДОБАВЛЕНИЕ									//

void rec_add(teacher abc, FILE *db) 
{
	int ptr;
	indx a;
	teacher d;
	fseek(db, 0, SEEK_END);
	abc.del = 0;
	inc_idx(0);
	rewind(idx);
	fread(&a, sizeof(indx), 1, idx);
	abc.id = a.id;
	rewind(db);

	if (a.free) {
		d.del = 0;
		dec_idx(2);
		while (!d.del && !feof(db)) {
			ptr = ftell(db);
			fread(&d, sizeof(teacher), 1 , db);
		}
		if (feof(db))
			fseek(db, 0, SEEK_END);
		fseek(db, ptr, SEEK_SET);
	}
	else 
		fseek(db, 0, SEEK_END);

	fwrite(&abc, sizeof(teacher), 1, db);

	printf("создана запись [%d]\n", abc.id);
	
}

//////////////////////////////////////////////////////////////////////////
//							СОРТИРОВКА 									//

void rec_sort(int field, FILE* db) 
{
	indx in;
	int i, j, size;
	teacher *array, abc;
	rewind(idx);
	fread(&in, sizeof(indx), 1, idx);
	size = in.id - in.del;
	array = malloc(sizeof(teacher) * size);
	status();
	rewind(db);

	// заполнение массива структур из базы
	for (i = 0; i < size; i++) {
		fread(&array[i], sizeof(teacher), 1, db);
		if (array[i].del)
			i--;
	}

	// 	пузырьковая сортировка
	for (i = 0; i < size - 1; i++) {
		for (j = 0; j < size - i - 1; j++) {
			if (field_cmp(array[j], array[j+1], field) > 0) {
				abc = array[j + 1];
				array[j + 1] = array[j];
				array[j] = abc;
			}
		}
	}

	print_banner();
	for (i = 0; i < size; i++) 
		print_table(find_rec(array[i].id, db), db);

	BAR("\n");
	free(array);
}

//////////////////////////////////////////////////////////////////////////
//							ЧИСТКА										//

