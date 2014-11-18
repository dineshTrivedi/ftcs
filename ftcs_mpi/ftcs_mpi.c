#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define tam 1.0
#define dx 0.00001
#define dt 0.000001
#define T  0.01
#define kappa 0.000045

int main(int argc, char *argv[]) {

    double *tmp, *u, *u_prev, *retorno;
    double x, t, maximo;
    int i, tamLocal, maxloc, iproc;
    int fatorProcesso, ult, tamTotal;
    // retirar depois
    int numProc = 2, esteProc = 0;
    retorno = (double *) malloc((numProc+1)*sizeof(double));

    MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &esteProc);

    /* Dividimos pelo numero de processos */
    /* Claculando quantidade de pontos */
    tamTotal = (tam/dx);
    tamLocal = tamTotal/numProc;
    fatorProcesso = esteProc*tamLocal;

    if (esteProc==numProc-1) {
      ult = tamTotal - fatorProcesso;
    }
    else
      ult = floor((float)(tam/dx)/numProc);

    printf("no %d/%d: fatorProcesso=%d, qtd=%d, ult=%d\n\n",
       esteProc, numProc, fatorProcesso, ult, (fatorProcesso+ult));

    /* Alocando vetores */
    u = (double *) malloc((ult+1)*sizeof(double));
    u_prev = (double *) malloc((ult+1)*sizeof(double));

    printf("no %d/%d: Inicio: qtde=%d, dt=%g, dx=%g, dx²=%g, kappa=%f, const=%f\n\n",
       esteProc, numProc, (ult+1), dt, dx, dx*dx, kappa, kappa*dt/(dx*dx));

    printf("Iteracoes previstas: %g\n\n", T/dt);

    // x = dx*esteProc*fatorProcesso + dx;
    x = dx*fatorProcesso + dx;
    printf("\nno %d/%d: x inicial: %f\n", esteProc, numProc, x);

    for (i=1; i<ult; i++) {
      if (x<=0.5)
          u_prev[i] = 200*x;
      else
          u_prev[i] = 200*(1.-x);
      x += dx;
    }

    printf("\nno %d/%d: x final: %f\n", esteProc, numProc, x);

    t = 0.;
    while (t<T) {
      x = dx*esteProc*fatorProcesso + dx;
      for (i=1; i<ult; i++) {
        u[i] = u_prev[i] + kappa*dt/(dx*dx)*(u_prev[i-1]-2*u_prev[i]+u_prev[i+1]);
        x += dx;
      }
      u[0] = u[ult] = 0.; /* forca condicao de contorno */
      tmp = u_prev; u_prev = u; u = tmp; /* troca entre ponteiros */
      t += dt;
    }

    /* Calculando o maior valor e sua localizacao */
    maxloc = 0;
    for (i=1; i<ult+1; i++) {
      if (u[i] > u[maxloc]) maxloc = i;
    }
    printf("no %d/%d: Maior valor u[%d] = %g\n\n", esteProc, numProc, maxloc, u[maxloc]);

    if (esteProc!=0) { /* Processos escravos */
      MPI_Send(&u[maxloc], 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    else { /* Processo Mestre */
      retorno[0] = u[maxloc];
      for(iproc=1; iproc<numProc; iproc++) {
        MPI_Recv(&retorno[iproc], 1, MPI_DOUBLE, iproc,
             0, MPI_COMM_WORLD, &status);
        // printf("source %d, erro %d\n", status.MPI_SOURCE, status.MPI_ERROR);
      }
    }

    if (esteProc==0) {
        for(iproc=0; iproc<numProc; iproc++) {
          printf("retorno[%d]=%f\n", iproc, retorno[iproc]);
        }

        maximo = retorno[0];
        for(i=1; i<numProc; ++i){
            if(retorno[i] > maximo)
                maximo = retorno[i];
        }

        printf("\nMax de tudo=%f\n", maximo);
    }
    MPI_Finalize();

    free(retorno);
    free(u);
    free(u_prev);
    return 0;
  }

