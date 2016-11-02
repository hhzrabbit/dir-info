dirStats: main.c
	gcc main.c -g -lm -o dirStats

run: dirStats
	./dirStats

clean:
	rm *~
