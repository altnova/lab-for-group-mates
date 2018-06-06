
# My little datatbase 0.1

## файлы

##lab.c 
содержит пользоватеьский интерфейс, принимает команды 
пользователя и передает их исполняющим функциям 

##sockets.c 	
содежит сокеты. на самом деле не сокеты, но надо сделать туповато


##basics.c
содержит функции, которые используются в сокетах и структуры indx и teacher

в процессе работы используются два файла: `src/idx.dat` и `src/db.dat`

пользователь может попросить обнулить и залить заново базу данных из введенного файла (делиметром является '\n')


##rc/idx.dat
бинарный файл, который 	содержит одну структуру indx
то есть три int 

первый int `indx.id` 	содержит последний исползованый id
второй int `indx.del`	содержит количество удалений записей 
третий int `indx.free`	содержит количество удаленных записей, т. е. те, на место которых можно что-то записать

таким образом при любой операции с увеличением/уменьщением кол-ва записей используется модификация файла idx.dat 

создается, если его нет, и обнуляется, если загружаестя из file.txt

##src/db.dat
бинарный файл со структурами вида teacher 
создается, если его нет, и обнуляется, если загружается из file.txt