#*****************************************************************
# File:   ejecuta_CLIENTE.bash
# Author: HÉCTOR GARCÍA PIQUERAS - PSCD-Unizar
# Date:   DICIEMBRE 2019 - ENERO 2020
# Coms:   Ejemplo de Makefile para el trabajo de PSCD
#         chmod u+x ejecuta_CLIENTE.bash
#         ./ejecuta_p5_CLIENTE.bash
#         
#*****************************************************************
#!/bin/bash


if [[ "$#" -ne 2 ]]
then
	echo "Invocar como: ./ejecuta_CLIENT.bash  fichero citIni"
	echo "-----------------------------------------------------"

	exit
fi
make cliente
cd src/client

./Cliente $1 $2   #Argumentos a pasarle