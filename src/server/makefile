#*****************************************************************
# File:   Makefile
# Author: Héctor Garcia
# Date:   December 2019
# Coms:  Es necesario descomentar la línea 28 para poder compilar en hendrix
#*****************************************************************

#---------------------------------------------------------
CC=g++

SERVER=Servidor
MONITOR=PoblacionAProcesar

SOCKET_DIR=../lib/Socket
SOCKET=${SOCKET_DIR}/Socket

CAM_DIR=../lib/caminante
CAM=${CAM_DIR}/caminante

CPPFLAGS=-I. -I${SOCKET_DIR} -O2 -std=c++11 -lsockets # Flags compilacion
LDFLAGS=-pthread # Flags linkado threads


all: ${SERVER}
#----------------------------------------------------------------------------
#Para gestionar opciones de compilación según la máquina: hendrix tiene sus manías
#Descomentar la siguiente línea para compilar en hendrix
#SOCKETSFLAGS=-lsocket -lnsl

# SERVIDOR
# Compilacion
${SERVER}.o: ${SERVER}.cpp 
	${CC} -c ${CPPFLAGS} ${SERVER}.cpp

# Linkado
${SERVER}: ${SOCKET}.o ${SERVER}.o ${MONITOR}.o ${CAM}.o
	${CC} ${LDFLAGS} ${SOCKET}.o ${SERVER}.o ${MONITOR}.o ${CAM}.o -o ${SERVER} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# SOCKETS
# Compilacion libreria de Sockets
${SOCKET}.o: ${SOCKET}.hpp ${SOCKET}.cpp
	${CC} -c ${CPPFLAGS} ${SOCKET}.cpp -o ${SOCKET}.o
#-----------------------------------------------------------	
# LIMPIEZA
clean:
	$(RM) ${SOCKET}.o
	$(RM) ${SERVER}.o
	$(RM) ${MONITOR}.o
	$(RM) ${CAM}.o
	$(RM) ${SERVER}