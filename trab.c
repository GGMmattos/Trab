#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> 
#define COMP_REG 64

//char tam_camp ;
char tam_camp [COMP_REG - 4];

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

short leia_reg(char buffer[], int tam, FILE *entrada) {

    short comp_reg;

    if (fread(&comp_reg, sizeof(short), 1, entrada) == 0) {
        return 0;
    }

    if (comp_reg < tam) {
        fread(buffer, sizeof(char), comp_reg, entrada);
        buffer[comp_reg] = '\0';
        return comp_reg;
    } else {
        return 0;
    }
}


struct{
    int cont_reg;
}cab; // cabeçalho do arquivo ( 4 bytes)

int passa_arq(char campo[], int tam, FILE *file){
    int i = 0;
    char k = fgetc(file);

    while (k != '|' && k != EOF)
    {
        if (i < tam-1){
            campo[i++] = k;
        }
        k = fgetc(file);
    }

    campo[i] = 0;
    return i;

}
void importacao(char *argv){
    FILE * candidatos;
    FILE * dados;

    char buffer[COMP_REG +1], campo[COMP_REG - 4];
    int cont = 0;

    candidatos = fopen(argv,"rb");
    dados = fopen("dados.dat","wb");

    // if((candidatos) && (dados)){
        printf("Arquivos abertos com sucesso!");
        cab.cont_reg = -1;
        fwrite(&cab,sizeof(cab),1,dados);//prepara o arquivo vazio

        while (passa_arq (campo, tam_camp, candidatos) > -1)
        {
            strcat(buffer, campo);
            strcat(buffer, "|");

            if (cont++ == 3){
                fwrite(buffer, sizeof(char), COMP_REG, dados);
                buffer[0] = 0;
                cont = 0;
            }
        }

        //fread(&cab,sizeof(cab),1,candidatos);
        //fwrite(&candidatos,sizeof(char),COMP_REG,dados);

    // }else
    //     fprintf(stderr, "Erro na abertura dos arquivos");
}


int main(int argc, char *argv[]) {


    char nome_arq[20];
    char tam_camp[COMP_REG - 4];
   // char buffer[COMP_REG + 1];
    //char *campo;


    if (argc == 3 && strcmp(argv[1], "-i") == 0) {
        printf("Informe o nome do arquivo de registros: ");
        input(nome_arq, 20);
        importacao(argv[2]);

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