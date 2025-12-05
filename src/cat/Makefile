GCC=gcc -std=c11 -Wall -Werror -Wextra
S21=s21_cat
CT=cat
DELET=*.o *.exe *.a

all: $(S21)

$(S21): $(CT).o
	$(GCC) $(CT).o -o $(S21)

$(CT).o: $(CT).c
	$(GCC) -c $(CT).c -o $(CT).o

del:
	rm -f 10.txt 11.txt

test:
	chmod +x test_cat.sh
	./test_cat.sh

clang:
	clang-format --style=file:../.clang-format -i *.c

clang_n:
	clang-format --style=file:../.clang-format -Werror -n *.c

clean:
	rm -rf $(DELET) $(S21)

rebuild: clean all
