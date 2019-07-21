BIN = bin
LIB = lib
SRC = src
INC = include
OBJ = obj

CXX = g++
CFLAGS = -g -c -I$(INC)
LDFLAGS = -L$(LIB) -lm -lmain

all : $(BIN)/main $(BIN)/obtener_resultados $(BIN)/obtener_resultados2

# ************ Compilación de módulos ************
	
$(OBJ)/random_ppio.o : $(SRC)/random_ppio.c 
	$(CXX) $(CFLAGS) $(SRC)/random_ppio.c -o $(OBJ)/random_ppio.o	
	
$(OBJ)/timer.o : $(SRC)/timer.c 
	$(CXX) $(CFLAGS) $(SRC)/timer.c -o $(OBJ)/timer.o
	
$(OBJ)/qap.o : $(SRC)/qap.c $(INC)/qap.h
	$(CXX) $(CFLAGS) $(SRC)/qap.c -o $(OBJ)/qap.o
	
$(OBJ)/quicksort.o : $(SRC)/quicksort.c $(INC)/quicksort.h
	$(CXX) $(CFLAGS) $(SRC)/quicksort.c -o $(OBJ)/quicksort.o
	
$(OBJ)/main.o : $(SRC)/main.c $(INC)/qap.h $(OBJ)/qap.o
	$(CXX) $(CFLAGS) $(SRC)/main.c -o $(OBJ)/main.o
	
$(OBJ)/obtener_resultados.o : $(SRC)/obtener_resultados.c $(INC)/qap.h $(OBJ)/qap.o
	$(CXX) $(CFLAGS) $(SRC)/obtener_resultados.c -o $(OBJ)/obtener_resultados.o
	
$(OBJ)/obtener_resultados2.o : $(SRC)/obtener_resultados2.c $(INC)/qap.h $(OBJ)/qap.o
	$(CXX) $(CFLAGS) $(SRC)/obtener_resultados2.c -o $(OBJ)/obtener_resultados2.o
	
# ************ Biblioteca ************
	
$(LIB)/libmain.a : $(OBJ)/qap.o $(OBJ)/quicksort.o $(OBJ)/random_ppio.o $(OBJ)/timer.o 
	ar rvs $(LIB)/libmain.a $(OBJ)/qap.o $(OBJ)/quicksort.o $(OBJ)/random_ppio.o $(OBJ)/timer.o

# ************ Generación de ejecutables ************
	
$(BIN)/main : $(OBJ)/main.o $(LIB)/libmain.a
	$(CXX) -o $(BIN)/main $(OBJ)/main.o $(LDFLAGS)

$(BIN)/obtener_resultados : $(OBJ)/obtener_resultados.o $(LIB)/libmain.a
	$(CXX) -o $(BIN)/obtener_resultados $(OBJ)/obtener_resultados.o $(LDFLAGS)
	
$(BIN)/obtener_resultados2 : $(OBJ)/obtener_resultados2.o $(LIB)/libmain.a
	$(CXX) -o $(BIN)/obtener_resultados2 $(OBJ)/obtener_resultados2.o $(LDFLAGS)
	
# ************ Limpieza ************
clean :
	-rm $(OBJ)/* $(SRC)/*~ $(INC)/*~ ./*~ 

clean_all : clean
	-rm $(BIN)/* $(LIB)/*
