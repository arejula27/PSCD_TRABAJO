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

SOCKET_DIR= ${LIB}Socket
SOCKET=${SOCKET_DIR}/Socket

LIB_CAM = ${LIB}/caminante
CAM =${LIB_CAM}/caminante


LIB_CLI =${SRC}/client
CLI = ${LIB_CLI}/Cliente

MON =${LIB_CLI}/Monitorizacion
POBA=${LIB_CLI}/PoblacionActual

BUILD = ./build

TEST = test
POBTEST = ${TEST}/pobMain

#----------------------------------------------------------------------------
CPPFLAGS=-I. -I${SOCKET_DIR} -I${LIB} -O2 -std=c++11 -lsockets # Flags compilacion
LDFLAGS=-pthread # Flags linkado threads
TESTPOBFLG= -I.  -I${LIB_CAM} -O2 -std=c++11  -lgtest  -lgtest_main -pthread # Flags compilacion


all: cliente
#----------------------------------------------------------------------------
#Para gestionar opciones de compilación según la máquina: hendrix tiene sus manías
#Descomentar la siguiente línea para compilar en hendrix
#SOCKETSFLAGS=-lsocket -lnsl
#-----------------------------------------------------------
#CLIENTE
cliente: ${CLI}.o ${MON}.o ${POBA}.o ${CAM}.o
	${CC} -c ${CPPFLAGS} ${CLI}.o ${MON}.o ${POBA}.o -o ${CLI}

${CLI}.o: ${CLI}.cpp 
	${CC} -c ${CPPFLAGS} ${CLI}.cpp  -o ${CLI}.o

#-----------------------------------------------------------
# Monitorización
# Compilacion monitor de Monitorizacón
${MON}.o: ${MON}.cpp 
	${CC} -c ${CPPFLAGS} ${MON}.cpp  -o ${MON}.o

#-----------------------------------------------------------
# PoblacionActual
# Compilacion monitor de PoblacionActual
${POBA}.o: ${POBA}.cpp 
	${CC} -c ${CPPFLAGS} ${POBA}.cpp  -o ${POBA}.o

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
pobtest: ${POBTEST}.cpp ${CAM}.o
	${CC}  ${POBTEST}.cpp ${CAM}.o ${TESTPOBFLG}  -o ${BUILD}/pobTest

#-----------------------------------------------------------	
#---------------------------------------------------------
# LIMPIEZA
clean:
	$(RM) ${SOCKET}.o
	$(RM) ${CAM}.o
