GCC = g++ -std=c++17 -Wall -Werror -g

main: main.cc
	$(GCC) -o $@ $^

%.o: %.cc %.h
	$(GCC) -c $<

.PHONY: run
run: main
	@echo "Running ./main...\n"
	@./main

.PHONY: clean
clean:
	-rm -f *.o main
	-rm -rf main.dSYM


