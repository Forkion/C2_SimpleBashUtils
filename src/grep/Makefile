GCC=gcc -std=c11 -Wall -Werror -Wextra
G21=s21_grep
GP=grep
PC=parser
DELET=*.o *.exe *.a

all:$(G21)

$(G21): $(GP).o $(PC).o
	$(GCC) $(GP).o $(PC).o -o $(G21)

$(GP).o: $(GP).c
	$(GCC) -c $(GP).c -o $(GP).o

$(PC).o: $(PC).c
	$(GCC) -c $(PC).c -o $(PC).o

del:
	rm -f 10.txt 11.txt grep.txt s21_grep.txt

test:
	chmod +x test_grep.sh
	./test_grep.sh

clang:
	clang-format --style=file:../.clang-format -i *.c *.h

clang_n:
	clang-format --style=file:../.clang-format -Werror -n *.c *.h

clean:
	rm -rf $(DELET) $(G21)

rebuild: clean all
