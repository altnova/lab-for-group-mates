lab:
	rm lab
	gcc -w -o lab lab.c && ./lab

clean:
	rm -f src/idx.dat
	rm -f src/db.dat