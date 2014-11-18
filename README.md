ftcs
====

Cálculo numérico de um problema unidimensional de condução de calor por diferenças finitas através do método FTCS.

Código para cálculo Serial e distribuido (Utilizando MPI).

Instalando MPI
==========================

sudo apt-get install libcr-dev mpich2 mpich2-doc.

Compilando MPI
==========================

mpicc -o ftcs ftcs.c -lm.

