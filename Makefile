prefix ?= /usr/local
HFILES = ini.h
CFILES = ini.c
OBJ = ini.o
PCFILE = libini.pc

LIB_VERSION = 0.1
LIB = libini.a
LIB_SHARED = libini.so.$(LIB_VERSION)

TESTS = test/test1.c
DEBUG = -g -fsanitize=address -Wall -Wextra


# Create .o file
$(OBJ): ini.h ini.c
	$(CC) -c -o $@ $(CFILES)

install: $(LIB) $(LIB_SHARED) $(HFILE)
	@echo "Installing files to $(prefix)"

	install -d ${prefix}/include ${prefix}/lib
	install ini.h ${prefix}/include
	install $(LIB) ${prefix}/lib
	install $(LIB_SHARED) ${prefix}/lib

# Create .a file
$(LIB): $(OBJ)
	ar -cvq $@ $^

# Create .so file
$(LIB_SHARED): $(OBJ)
	$(CC) -shared -o $@ $^

# Execute the tests
check: test/test1.c test/test1.ini
	@echo "Running tests..."
	@$(CC) $(DEBUG) -o test/test1 test/test1.c
	@ cd test && ./test1
	@if [ $$? -eq 0 ]; then \
		echo "Tests passed!"; \
	else \
		echo "Test execution failed with exit code $$?."; \
	fi


clean:
	rm -f $(OBJ) $(LIB) $(LIB_SHARED)

.PHONY: install clean check