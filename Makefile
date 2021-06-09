build:
	gcc process_generator.c -o process_generator.out
	gcc clk.c -o clk.out
	gcc scheduler.c -o scheduler.out
	gcc process.c -o process.out
#	gcc test_generator.c -o test_generator.out

clean:
	rm -f *.out
	ipcrm -a
	rm -f *.log
	rm -f *.perf

all: clean build run

run:
	./process_generator.out processes.txt 4

gen:
	gcc test_generator.c -o test_generator.out
	./test_generator.out