#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#define COMP_REG 64
#define COMP_CAMPO 60

int proximoRegistro(char registro[64], FILE *file);
//char COMP_CAMPO ;
//char COMP_CAMPO [COMP_REG - 4];

struct{
    int cont_reg;
}cab; // cabeçalho do arquivo ( 4 bytes)

int byteoffset(int rrn)
{
    return rrn * COMP_REG + sizeof(cab);
}

int leia_campo(char campo[], int tam, FILE *file)
{
    int i = 0;
    char c = fgetc(file);

    //Leitura dos caracteres ate atingir o delimitador('|')
    while (c != '|')
    {
        if (feof(file)) // Caso o fim do arquivo for alcancado antes do fim do campo
            return -1;

        if (i < tam - 1)
            campo[i++] = c;

        c = fgetc(file);
    }

    campo[i] = 0;

    return i;
}

int ler_linha(char campo[], int tam, FILE *file){
    int i = 0;
    char k = fgetc(file);

    
    while (k != '\n')
    {
        if (feof(file)) 
            return -1;

        if (i < tam - 1)
            campo[i++] = k;

        k = fgetc(file);
    }

    campo[i] = 0;

    return i;

}
// int input(char str[], int size)
// {
//     int i = 0;
//     char c = getchar();
//     while (c != '\n')
//     {
//         if (i < size - 1)
//         {
//             str[i] = c;
//             i++;
//         }
//         c = getchar();
//     }
//     str[i] = '\0';
//     return i;
// }

// short leia_reg(char buffer[], int tam, FILE *entrada)
// {

//     short comp_reg;

//     if (fread(&comp_reg, sizeof(short), 1, entrada) == 0)
//     {
//         return 0;
//     }

//     if (comp_reg < tam)
//     {
//         fread(buffer, sizeof(char), comp_reg, entrada);
//         buffer[comp_reg] = '\0';
//         return comp_reg;
//     }
//     else
//     {
//         return 0;
//     }
// }


void importacao(char nomeArquivoImportacao[])
{
    FILE *arquivoOriginal;
    FILE *novoArquivo;
    int cabecalho = -1;

    arquivoOriginal = fopen(nomeArquivoImportacao, "r");
    novoArquivo = fopen("dados.dat", "w");

    if (arquivoOriginal == NULL || novoArquivo == NULL)
    {
        fprintf(stderr, "nao foi possivel abrir os arquivos. ABORTANDO....\n");
        exit(EXIT_FAILURE);
    }

    fwrite(&cabecalho, sizeof(int), 1, novoArquivo);

    char buffer[64];

    while (!feof(arquivoOriginal))
    {
        int sucesso = proximoRegistro(buffer, arquivoOriginal);

        if (sucesso == 1)
        {
            fwrite(buffer, sizeof(char), 64, novoArquivo);
        }
    }
    fclose(arquivoOriginal);
    fclose(novoArquivo);
}

int proximoRegistro(char registro[64], FILE *file)
{
    int contPipe = 0, tam = 0;
    char c;

    memset(registro, '\0', 64); //colocar o \0 até dar 64 

    while (contPipe < 4 && tam < 64)
    {
        c = fgetc(file);
        if (c == EOF)
        {
            return -1;
        }
        
        registro[tam] = c;

        if (c == '|')
        {
            contPipe += 1;
        }
        tam += 1;
    }

    return 1;
}

int buscaRegistro(char *chave, FILE *file){


    
    int rrn;
    char campo[COMP_CAMPO];

    fseek(file, sizeof(cab), SEEK_SET);

    while (leia_campo(campo,7,file) > -1)
    {
        if(strcmp(campo, chave) == 0){
            fseek(file, -7, SEEK_CUR);
            return rrn;
        }

        fseek(file, COMP_REG -7, SEEK_CUR);
        rrn++;
    }

    return -1;
    
}

int PED(){

}

void operacoes(char *argv)
{

    FILE *dados;
    FILE *operacoesFile;

    char campo [COMP_CAMPO];       
    char buffer [COMP_REG];
    char def_op;  //definição de qual operação vai ser feita
    char chave[7];
    int posChave; //posChave está como o RRN

    operacoesFile = fopen(argv, "rb");
    dados = fopen("dados.dat", "r+b");

    while ((def_op = fgetc(operacoesFile)) != EOF)
    {
        buffer[0]=0;
        fgetc(operacoesFile);

        switch(def_op){

        case 'b':
        {
           
            ler_linha(chave, 7, operacoesFile);

            posChave = buscaRegistro(chave, dados);  

            printf ("\nREGISTRO DE CHAVE %s\n", chave);

            if(posChave == -1){
                printf("REGISTRO NAO ENCONTRADO\n");
                break;
                
            }

            for (int i = 0; i < 4; i++){
                leia_campo(campo, COMP_CAMPO, dados);
                strcat(buffer, campo);
                strcat(buffer,"|");
            }
            
            printf("%s; RRN = %d, byte-offset %d\n",buffer, posChave, byteoffset(posChave));
            break;        
        }

        case 'i':
            ler_linha(buffer, COMP_REG, operacoesFile);
            strncpy(chave, buffer, 6);
            printf("\n INSERCAO DO REGISTRO DE CHAVE %s\n", chave);

            if (buscaRegistro (chave, dados) != -1){
                printf("ERRO: CHAVE %s JA EXISTE NO REGISTRO", chave);
                break;
            }

            rewind(dados);
            fread(&cab, sizeof(cab), 1, dados);
            posChave = cab.cont_reg;

            if (posChave == -1){

                fseek(dados, 0, SEEK_END);
                fwrite(buffer, sizeof(char), COMP_REG, dados);
                fprintf("FIM DO ARQUIVO");
            }

            else{

                cab.cont_reg = PED(dados, posChave);
                fwrite(buffer, sizeof(char), COMP_REG, dados);
                rewind(dados);
                fwrite(&cab, sizeof(cab), 1, dados);

                printf("LOCAL: RRN = %d (byte-offset %d) [reutilizado]\n", posChave, byteoffset(posChave));
            }
            
            break;
            

        case 'r':
        
            ler_linha(chave, 7, operacoesFile);
            printf("\nREMOCAO DO REGISTRO %s\n", chave);

            rewind(dados);
            fread(&cab, sizeof(cab), 1, dados);

            posChave = buscaRegistro(chave, dados);
            
            if (posChave == -1)
            {
                printf("REGISTRO NAO ENCONTRADO\n");
                break;
            }

            fputc('*', dados);
            fwrite(&cab.cont_reg, sizeof(int), 1, dados);

            cab.cont_reg = posChave;

            rewind(dados);
            fwrite(&cab, sizeof(cab), 1, dados);

            printf("REMOVIDO");
            printf("\nPosicao: RRN = %d (byte-offset %d)\n", posChave, byteoffset(posChave));
            break;

            default:
            printf("\n NAO FOI POSSIVEL REMOVER");
        

            fseek(dados, 0, SEEK_END);
            if (((ftell(dados) - sizeof(cab)) % COMP_REG) != 0) /* ftell = Retorna o valor atual do indicador de posição do fluxo. 
                                                                Esse valor pode ser usado pela função fseek com origem SEEK_SET 
                                                                para retornar o indicador a posição atual.*/
            {
              printf("NAO FOI POSSIVEL REALIZAR A OPERACAO");
              exit(1);
            }
        
        
        }
    }

    fclose(operacoesFile);
    fclose(dados);
    
}



int main(int argc, char *argv[])
{
    

    if (argc == 3 && strcmp(argv[1], "-i") == 0)
    {
        printf("Nome do arquivo em que ocorrera a importacao= %s\n", argv[2]);
        importacao(argv[2]);
    }
    else if (argc == 3 && strcmp(argv[1], "-e") == 0)
    {

        printf("Modo de execucao de operacoes ativado ... nome do arquivo = %s\n", argv[2]);
        operacoes(argv[2]);
    }
    else if (argc == 2 && strcmp(argv[1], "-p") == 0)
    {

        printf("Modo de impressao da PED ativado ...\n");
        // imprime_ped();
    }
    else
    {

        fprintf(stderr, "Argumentos incorretos!\n");
        fprintf(stderr, "Modo de uso:\n");
        fprintf(stderr, "$ %s (-i|-e) nome_arquivo\n", argv[0]);
        fprintf(stderr, "$ %s -p\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    return 0;
}