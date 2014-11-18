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

Configurando QtCreator
==========================
Adicionar no arquivo .pro

QMAKE_CXX = mpicxx

QMAKE_CXX_RELEASE = $$QMAKE_CXX

QMAKE_CXX_DEBUG = $$QMAKE_CXX

QMAKE_LINK = $$QMAKE_CXX

QMAKE_CC = mpicc
 
QMAKE_CFLAGS += $$system(mpicc --showme:compile)

QMAKE_LFLAGS += $$system(mpicxx --showme:link)

QMAKE_CXXFLAGS += $$system(mpicxx --showme:compile) -DMPICH_IGNORE_CXX_SEEK

QMAKE_CXXFLAGS_RELEASE += $$system(mpicxx --showme:compile) -DMPICH_IGNORE_CXX_SEEK


Add a Custom Executable run configuration and set the executable to /usr/bin/mpirun or any other path where mpirun resides on your system (run ‘which mpirun’ in a terminal to find it if you don’t know where it is). Then, set the arguments to

-n 2 yourexecutable

to run yourexecutable with 2 processors. Of course you’ll have to change yourexecutable to whatever your executable is named.

Rodando Máquina Local
===========================

* Um processo:

mpirun -np 1 ftcs

* Dois processos:

mpirun -np 2 ftcs

Configurando chaves ssh para execução em mais máquinas
===========================

* Gerar um par de chaves criptográficas para permitir conexões seguras na maquina local:
```shell
ssh-keygen
```

Rodando Máquina da rede
===========================
Criar um arquivo no formato:

IP-maquina1

IP-maquina2

...

IP-maquinaN


* Especificar quantidade de processos em cada máquina (usar parâmetro slots no arquivo texto):

IP-maquina1 slots=2

IP-maquina2 slots=2

IP-maquina3 slots=2

* Parâmetro -nolocal, --nolocal: Do not run any copies of the launched application on the same node as orterun is running. This option will override listing the localhost with --host or any other host-specifying mechanism.
* Parâmetro -machinefile, --machinefile < machinefile >: Lista de hosts para executarmos

* Quatro processos:

mpirun -np 4 -nolocal -machinefile maquinas.txt ftcs

* Oito processos:

mpirun -np 8 -nolocal -machinefile maquinas.txt ftcs


Referências
===========================

* https://jetcracker.wordpress.com/2012/03/01/how-to-install-mpi-in-ubuntu/
* http://dragly.org/2012/03/14/developing-mpi-applications-in-qt-creator/ 
* https://www.open-mpi.org/doc/v1.8/man1/mpirun.1.php
* http://askubuntu.com/questions/403936/ssh-connect-to-host-localhost-port-22-connection-refused 