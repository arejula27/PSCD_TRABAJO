#*****************************************************************
# File:   Makefile_p5_e1
# Author: PSCD-Unizar
# Date:   octubre 2018
# Coms:   Ejemplo de Makefile para la práctica 5 de PSCD
#         "make -f Makefile_p5_e1" construye todos los ejecutables
#         "make -f Makefile_p5_e1 Cliente" construye "Cliente"
#         ....
#         Es necesario descomentar la línea 28 para poder compilar en hendrix
#*****************************************************************

#---------------------------------------------------------
CC=g++
#---------------------------------------------------------


SRC = src
LIB = ${SRC}/lib

SOCKET_DIR= ${LIB}/Socket
SOCKET=${SOCKET_DIR}/Socket

LIB_CAM = ${LIB}/caminante
CAM =${LIB_CAM}/caminante

LIB_SERV =${SRC}/server
SERV = ${LIB_SERV}/Servidor
POBAPROC = ${LIB_SERV}/PoblacionAProcesar

LIB_CLI =${SRC}/client
CLI = ${LIB_CLI}/Cliente

POBA=${LIB_CLI}/PoblacionActual

BUILD = ./build

TEST = test
POBTEST = ${LIB_CAM}/mainPbPob

#----------------------------------------------------------------------------
CPPFLAGS=-I. -I${SOCKET_DIR} -I${LIB} -I${LIB_CAM} -O2 -std=c++11 -lsockets # Flags compilacion
LDFLAGS=-pthread   # Flags linkado threads
TESTPOBFLG= -I.  -I${LIB_CAM} -O2 -std=c++11   # Flags compilacion


all: cliente server
#----------------------------------------------------------------------------
#Para gestionar opciones de compilación según la máquina: hendrix tiene sus manías
#Descomentar la siguiente línea para compilar en hendrix
#SOCKETSFLAGS=-lsocket -lnsl
#-----------------------------------------------------------
#Servidor
server: ${SERV}.o  ${POBAPROC}.o ${CAM}.o ${SOCKET}.o
	${CC}  ${LDFLAGS} ${SERV}.o  ${CAM}.o ${SOCKET}.o ${POBAPROC}.o -o ${SERV}

${SERV}.o: ${SERV}.cpp 
	${CC} -c ${CPPFLAGS} ${SERV}.cpp  -o ${SERV}.o

#-----------------------------------------------------------
# PoblacionActual
# Compilacion monitor de PoblacionActual
${POBAPROC}.o: ${POBAPROC}.cpp 
	${CC} -c ${CPPFLAGS} ${POBAPROC}.cpp  -o ${POBAPROC}.o
#-----------------------------------------------------------
# SOCKETS
# Compilacion libreria de Sockets
${SOCKET}.o: ${SOCKET}.hpp ${SOCKET}.cpp
	${CC} -c ${CPPFLAGS} ${SOCKET}.cpp -o ${SOCKET}.o
#-----------------------------------------------------------	

# Caminate
# Compilacion libreria de Caminante
${CAM}.o: ${CAM}.hpp ${CAM}.cpp
	${CC} -c ${CPPFLAGS} ${CAM}.cpp -o ${CAM}.o
#-----------------------------------------------------------
#CLIENTE
cliente: ${CLI}.o  ${POBA}.o ${CAM}.o ${SOCKET}.o
	${CC}  ${LDFLAGS} ${CLI}.o  ${CAM}.o ${SOCKET}.o ${POBA}.o -o ${CLI}

${CLI}.o: ${CLI}.cpp 
	${CC} -c ${CPPFLAGS} ${CLI}.cpp  -o ${CLI}.o

#-----------------------------------------------------------
# PoblacionActual
# Compilacion monitor de PoblacionActual
${POBA}.o: ${POBA}.cpp 
	${CC} -c ${CPPFLAGS} ${POBA}.cpp  -o ${POBA}.o
#-----------------------------------------------------------
#CLIENTE Estadístico
clienteB: ${CLI}B.o ${SOCKET}.o
	${CC}  ${LDFLAGS} ${CLI}B.o ${SOCKET}.o -o ${CLI}B

${CLI}B.o: ${CLI}B.cpp 
	${CC} -c ${CPPFLAGS} ${CLI}B.cpp  -o ${CLI}B.o

#-----------------------------------------------------------
pobtest: ${POBTEST}.cpp ${CAM}.o 

	${CC}  ${POBTEST}.cpp ${CAM}.o ${TESTPOBFLG}  -o pobTest

#-----------------------------------------------------------	
#---------------------------------------------------------
# LIMPIEZA
clean:
	$(RM) ${SOCKET}.o
	$(RM) ${CAM}.o
	$(RM) ${SERV}.o
	$(RM) $(POBA).o
	$(RM) $(POBAPROC).o
