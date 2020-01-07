#*****************************************************************
# File:   Makefile
# Author: Hector Garcia
# Date:   December 2019
# Coms:  Es necesario descomentar la línea 28 para poder compilar en hendrix
#*****************************************************************

#---------------------------------------------------------
CC=g++

CLIENTE=Cliente
MONITOR=PoblacionActual

SOCKET_DIR=Socket
SOCKET=${SOCKET_DIR}/Socket

CAM_DIR=../lib/caminante
CAM=${CAM_DIR}/caminante

CPPFLAGS=-I. -I${SOCKET_DIR} -O2 -std=c++11 -lsockets # Flags compilacion
LDFLAGS=-pthread # Flags linkado threads


all: ${CLIENTE}
#----------------------------------------------------------------------------
#Para gestionar opciones de compilación según la máquina: hendrix tiene sus manías
#Descomentar la siguiente línea para compilar en hendrix
#SOCKETSFLAGS=-lsocket -lnsl

# SERVIDOR
# Compilacion
${CLIENTE}.o: ${CLIENTE}.cpp 
	${CC} -c ${CPPFLAGS} ${CLIENTE}.cpp

# Linkado
${CLIENTE}: ${SOCKET}.o ${CLIENTE}.o ${MONITOR}.o ${CAM}.o
	${CC} ${LDFLAGS} ${SOCKET}.o ${CLIENTE}.o ${MONITOR}.o ${CAM}.o -o ${CLIENTE} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# SOCKETS
# Compilacion libreria de Sockets
${SOCKET}.o: ${SOCKET}.hpp ${SOCKET}.cpp
	${CC} -c ${CPPFLAGS} ${SOCKET}.cpp -o ${SOCKET}.o
#-----------------------------------------------------------	
# LIMPIEZA
clean:
	$(RM) ${SOCKET}.o
	$(RM) ${CLIENTE}.o
	$(RM) ${MONITOR}.o
	$(RM) ${CAM}.o
	$(RM) ${CLIENTE}