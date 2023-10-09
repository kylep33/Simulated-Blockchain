# Kyle Pereira 823 517 000
# CXX Make variable for compiler
CC=g++
# -std=c++11  C/C++ variant to use, e.g. C++ 2011
# -Wall       show the necessary warning files
# -g3         include information for symbolic debugger e.g. gdb 
CCFLAGS=-std=c++11 -Wall -g3 -c

# object files
OBJS = producer.o consumer.o log.o main.o 

PROGRAM = cryptoexc 

# The program depends upon its object files
$(PROGRAM) : $(OBJS)
	$(CC) -pthread -o $(PROGRAM) $(OBJS)

main.o : main.cpp
	$(CC) $(CCFLAGS) main.cpp

producer.o : producer.cpp producer.h
	$(CC) $(CCFLAGS) producer.cpp

consumer.o : consumer.cpp consumer.h 
	$(CC) $(CCFLAGS) consumer.cpp

log.o : log.cpp log.h
	$(CC) $(CCFLAGS) log.cpp


# vaddr_tracereader.o : vaddr_tracereader.cpp vaddr_tracereader.h
# 	$(CC) $(CCFLAGS) vaddr_tracereader.cpp

# # memory.o : memory.cpp memory.h
# # 	$(CC) $(CCFLAGS) memory.cpp

# page_table.o : page_table.cpp page_table.h
# 	$(CC) $(CCFLAGS) page_table.cpp

# print_helpers.o : print_helpers.cpp print_helpers.h
# 	$(CC) $(CCFLAGS) print_helpers.cpp

# tlb.o : tlb.cpp tlb.h
# 	$(CC) $(CCFLAGS) tlb.cpp


clean :
	rm -f *.o *~ $(PROGRAM)