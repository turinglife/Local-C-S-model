# author: Xin Zhu

# set compile arguments
CC = gcc
CFLAGS = -g -Wno-unused-result
//CFLAGS = 

# avoid warning info about "up to date"
.PHONY: target

# generate object file and execute file
target: client.o threadpool.o itfd.o
	$(CC) $(CFLAGS) client.o -o client
	$(CC) $(CFLAGS) itfd.o threadpool.o -lpthread -o itfd
	@echo "====================================================================="
	@echo "=                        compile successfully                       ="
	@echo "====================================================================="


client.o:
	$(CC) $(CFLAGS) -c ./client.c -O -o $@

threadpool.o:
	$(CC) $(CFLAGS) -c ./threadpool.c -o $@

itfd.o:
	$(CC) $(CFLAGS) -c ./itfd.c -o $@


# clean rubbish
clean:
	rm -f *.o client itfd
	@echo "====================================================================="
	@echo "=                        clean successfully                         ="
	@echo "====================================================================="

