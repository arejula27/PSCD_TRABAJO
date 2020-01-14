#*****************************************************************
# File:   ejecuta_SERVER.bash
# Author: HÉCTOR GARCÍA PIQUERAS - PSCD-Unizar
# Date:   DICIEMBRE 2019 - ENERO 2020
# Coms:   Ejemplo de Makefile para el trabajo de PSCD
#         chmod u+x ejecuta_SERVER.bash
#         ./ejecuta_SERVER.bash puerto
#         
#*****************************************************************
#!/bin/bash

if [[ "$#" -ne 1 ]]
then
	echo "Invocar como: ./ejecuta_SERVER.bash  port"
	echo "-----------------------------------------------------"

	exit
fi

make clean 
make server
cd src/server

./Servidor $1