make && cat testmem.in | valgrind --leak-check=full ./game.exe
rm tmpsave
