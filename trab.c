#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int input(char str[], int size) {
    int i = 0;
    char c = getchar();
    while (c != '\n') {
        if (i < size-1) {
            str[i] = c;
            i++;
        }
        c = getchar();
    }
    str[i] = '\0';
    return i;
}

void importação(FILE * dados){
    
}

int main(int argc, char *argv[]) {

    FILE * arq;
    FILE * data;
    char nome_arq[20];

    
    if (argc == 3 && strcmp(argv[1], "-i") == 0) {
            printf("Informe o nome do arquivo de registros: ");
            input(nome_arq, 20);

        if ((arq = fopen(nome_arq, "rb")) == NULL){
            fprintf(stderr, "Erro, nao foi possivel abrir o arquivo '%s'", nome_arq);
            exit(EXIT_FAILURE);
        }   
    
        printf("Modo de importacao ativado ... nome do arquivo = %s\n", argv[2]);

        if ((data = fopen("dados.dat", "w+b"))==NULL)
            perror("O arquivo não pode ser aberto");

        
        // importacao(argv[2]);

































    } else if (argc == 3 && strcmp(argv[1], "-e") == 0) {

        printf("Modo de execucao de operacoes ativado ... nome do arquivo = %s\n", argv[2]);
        // executa_operacoes(argv[2]);

    } else if (argc == 2 && strcmp(argv[1], "-p") == 0) {

        printf("Modo de impressao da PED ativado ...\n");
        // imprime_ped();

    } else {

        fprintf(stderr, "Argumentos incorretos!\n");
        fprintf(stderr, "Modo de uso:\n");
        fprintf(stderr, "$ %s (-i|-e) nome_arquivo\n", argv[0]);
        fprintf(stderr, "$ %s -p\n", argv[0]);
        exit(EXIT_FAILURE);

    }

    return 0;
}