SYSTEM     = x86-64_sles10_4.1
LIBFORMAT  = static_pic

# ---------------------------------------------------------------------         
# Compiler selection                                                            
# ---------------------------------------------------------------------         

CCC = g++

# ---------------------------------------------------------------------         
# Compiler options                                                              
# ---------------------------------------------------------------------         

#CCOPT = -m64 -O -fPIC -fexceptions -DNDEBUG -DIL_STD -g -Wall
CCOPT = -O3 -m64

# ---------------------------------------------------------------------         
# Link options and libraries                                                    
# ---------------------------------------------------------------------         

CCFLAGS = $(CCOPT) 
CCLNFLAGS = -lm -pthread 

#------------------------------------------------------------                   
#  make all      : to compile.                                     
#  make execute  : to compile and execute.                         
#------------------------------------------------------------    

ROUTE.exe: main.o ece556.o util.o RoutingInst.o Net.o
	/bin/rm -f ROUTE.exe
	$(CCC) $(LINKFLAGS) $(CCFLAGS) main.o ece556.o util.o RoutingInst.o Net.o $(CCLNFLAGS) -o ROUTE.exe

main.o: main.cpp ece556.h RoutingInst.h Net.h 
	/bin/rm -f main.o
	$(CCC) $(CCFLAGS) main.cpp -c

ece556.o: ece556.cpp ece556.h RoutingInst.cpp RoutingInst.h Net.cpp Net.h
	/bin/rm -f ece556.o
	$(CCC) $(CCFLAGS) ece556.cpp -c

util.o: util.cpp ece556.h RoutingInst.h Net.h
	/bin/rm -f util.o
	$(CCC) $(CCFLAGS) util.cpp -c

RoutingInst.o: RoutingInst.h RoutingInst.cpp Net.cpp Net.h
	/bin/rm -f RoutingInst.o
	$(CCC) $(CCFLAGS) RoutingInst.cpp -c

Net.o: Net.h Net.cpp
	/bin/rm -f Net.o	
	$(CCC) $(CCFLAGS) Net.cpp -c

clean:
	/bin/rm -f *~ *.o ROUTE.exe *.out
