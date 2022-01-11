/**
  Este programa tem por finalidade gerenciar um arquivo hospitalar.
  Tem por objetivo cadastrar pacientes diagnosticados com COVID-19.
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <locale.h>
#include <direct.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <iso646.h>
#include "stdgen.h"


/// ESTRUTURA PACIENTE

struct REG_PACIENTE
  {
    char NOME[51], CPF[18], EMAIL[51], TELEFONE[20], COMORBIDADES[40], NASCIMENTO[15];
    char RUA[51], NUMERO[30], BAIRRO[51], CIDADE[51], ESTADO[8], CEP[14];
    int DIA, MES, ANO, IDADE;
  } PACIENTE;

// DATAS EM STRING
char DIAGNOSTICO[15];
char NASCIMENTO[15];

// ARQUIVOS DO SISTEMA
FILE *PTRARQGERAL;
FILE *PTRARQRISCO;
FILE *PTRARQBACKUP;

// VARIÁVEIS DO SISTEMA
char TITULO[49] = "CADASTRO DE PACIENTES DIAGNOSTICADOS COM COVID-19";
int  FIM = 1;
fpos_t POS;

/// DATA LOCAL
// APRESENTA A DATA DE HOJE NO CANTO SUPERIOR ESQUERDO DA TELA

void printData(void)
{
    struct tm *DATA;
    time_t segundos;
    time(&segundos);
    DATA = localtime(&segundos);
    printf("Data: %d/", DATA->tm_mday);
    printf("%d/",DATA->tm_mon+1);
    printf("%d\n\n",DATA->tm_year+1900);
}

/// PROGRAMA PRINCIPAL - UNIDADE MAIN

int main(void)
{
  //DEFINE A LINGUAGEM PADRÃO DO PROGRAMA
  setlocale(LC_ALL, "Portuguese");

  //CHAMA PÁGINA DE LOGIN E SENHA
  login();

  return 0;
}

/// TELA DE LOGIN E SENHA

void login(void)
{
    char USUARIO[15] = "UNIP";
    char SENHA[15] = "UNIP";
    char GETLOGIN[15], GETPASS[15];
    bool APROVADO = false;

    do
    {
      clear();
      position(5, 2); printData();
      position(1, 1); line("=");
      center(2, TITULO);
      position(3, 1); line("=");
      center(5, "- Digite seu usuário e senha -\n");
      position(28, 1); clearline();
      position(29, 1); clearline();

        position( 8,27); printf("Usuário ..............: ");
        position(10,27); printf("Senha ................: ");
        position(29, 2); printf("Digite [0] para encerrar o programa. ");
        position( 8,51); fgets(GETLOGIN, sizeof(GETLOGIN), stdin);
        if (GETLOGIN[0] == '0') return 0;
        position(10,51); fgets(GETPASS, sizeof(GETPASS), stdin);
        if (GETPASS[0] == '0') return 0;
        position(29, 2); clearline();

        while ((strncmp(GETLOGIN, USUARIO, 4) != 0)
               || (strncmp(GETPASS, SENHA, 4) != 0)
               || strlen(GETLOGIN) > 5
               || strlen(GETPASS) > 5)
        {
            position(14, 27); printf("Usuário ou senha inválidos! ");
            position(29, 2); pause("Tecle <Enter> para digitar novamente. ");
            position(29, 2); clearline();
            position(29, 2); printf("Digite [0] para encerrar o programa. ");
            position( 8,51); clearline();
            position(10,51); clearline();
            position(14,27); clearline();
            position(26, 2); clearline();
            position( 8,51); fgets(GETLOGIN, sizeof(GETLOGIN), stdin);
            if (GETLOGIN[0] == '0') return 0;
            position(10,51); fgets(GETPASS, sizeof(GETPASS), stdin);
            if (GETPASS[0] == '0') return 0;
        }

        APROVADO = true;
        position(14,27); printf("Login confirmado!");
        position(29, 2); clearline();
        position(29, 2); pause("Tecle <Enter> para continuar. ");
        position(22, 1); clearline();
        position(14,27); clearline();
        position(29, 2); clearline();
        if (APROVADO == true); break;

    }
    while (validarConteudo(GETLOGIN) != 1 || validarConteudo(GETPASS));

    //CHAMA MENU PRINCIPAL
    menu();
}

/// TELA DO MENU PRINCIPAL

void menu(void)
{
  char OPCAO[10];

  do
    {
      clear();
      position( 5, 2); printData();
      position( 1, 1); line("=");
      center( 2, TITULO);
      position( 3, 1); line("=");
      center( 5, "- Menu Principal -\n");
      position( 28, 1); clearline();
      position( 29, 1); clearline();
      position( 8,22); printf("Novo Cadastro ................: [1]");
      position(10,22); printf("Pesquisar Paciente ...........: [2]");
      position(12,22); printf("Listar Grupo de Risco ........: [3]");
      position(14,22); printf("Remover Cadastros ............: [4]");
      position(16,22); printf("Encerrar Sessão...............: [5]");
      position(22,22); printf("Digite a opcao desejada: --> ");
      fgets(OPCAO, sizeof(OPCAO), stdin);

      while (strlen(OPCAO) != 2)
                {
                    position(22,51); clearline();
                    position(22,51); fgets(OPCAO, sizeof(OPCAO), stdin);
                }

        switch(OPCAO[0])
        {
          case '1' : cadastrar(); break;
          case '2' : pesquisar(); break;
          case '3' : risco(); break;
          case '4' : remover(); break;
          case '5' : login(); break;
        }
      }
  while (validarConteudo(OPCAO) != 1 || OPCAO[0] != '5');
}

/// - MÓDULO DE CADASTRO DE PACIENTES -
// PEGA O INPUT DO USUÁRIO E VALIDA OS DADOS

void cadastrar(void)
{
  clear();
  position( 1, 1); line("-");
  center( 2, TITULO);
  position( 3, 1); line("-");
  center( 5, " - Novo Cadastro - \n");
  position( 7, 2); printf("Digite os dados do paciente: ");
  do
    {
            // CHAMA A TELA DA FICHA CADASTRAL
            telaCadastro();
            position(29, 2); printf("Digite [0] para voltar ao menu principal. ");
            position(28, 1); clearline();

            // NOME DO PACIENTE
            position(10,26); fgets(PACIENTE.NOME, sizeof(PACIENTE.NOME), stdin);
            if (strncmp(PACIENTE.NOME, "0", 1) == 0) break;
            while (validarLetras(PACIENTE.NOME) != 1 || strlen(PACIENTE.NOME) < 6)
            {
                position(10,26); clearline();
                position(29, 2); clearline();
                position(10,36); printf("Dados inválidos! Digite o nome completo. ");
                position(29, 2); pause("Tecle <Enter> para digitar novamente. ");
                position(10,26); clearline();
                position(29, 2); clearline();
                position(29, 2); printf("Digite [0] para voltar ao menu principal. ");
                position(10,26); fgets(PACIENTE.NOME, sizeof(PACIENTE.NOME), stdin);
                if (strncmp(PACIENTE.NOME, "0", 1) == 0) return 0;
            }

            // CPF DO PACIENTE
            position(11,40); printf(" *Digite somente números* ");
            position(11,26); fgets(PACIENTE.CPF, sizeof(PACIENTE.CPF), stdin);
            position(11,40); clearline();
            if (strlen(PACIENTE.CPF) == 2)
                {
                    if(strncmp(PACIENTE.CPF, "0", 1) == 0) break;
                }
            while (validarNumeros(PACIENTE.CPF) != 1 || strlen(PACIENTE.CPF) != 12)
            {
                position(11,26); clearline();
                position(29, 2); clearline();
                position(11,36); printf("Dados inválidos! Digite apenas números. ");
                position(29, 2); pause("Tecle <Enter> para digitar novamente. ");
                position(11,26); clearline();
                position(11,40); printf(" *Digite somente números* ");
                position(29, 2); clearline();
                position(29, 2); printf("Digite [0] para voltar ao menu principal. ");
                position(11,26); fgets(PACIENTE.CPF, sizeof(PACIENTE.CPF), stdin);
                position(11,40); clearline();
                if (strlen(PACIENTE.CPF) == 2)
                {
                    if(strncmp(PACIENTE.CPF, "0", 1) == 0) return 0;
                }
            }

            // DATA DE NASCIMENTO DO PACIENTE
            position(12,40); printf(" *Data no formato dd/mm/aaaa* ");
            position(12,26); fgets(PACIENTE.NASCIMENTO, sizeof(PACIENTE.NASCIMENTO), stdin);
            position(12,40); clearline();
            if (strlen(PACIENTE.NASCIMENTO) == 2)
                {
                    if(strncmp(PACIENTE.NASCIMENTO, "0", 1) == 0) break;
                }
            while (validarData(PACIENTE.NASCIMENTO) != 1 || strlen(PACIENTE.NASCIMENTO) != 11)
            {
                position(12,26); clearline();
                position(29, 2); clearline();
                position(12,36); printf("Formato incorreto! ");
                position(29, 2); pause("Tecle <Enter> para digitar novamente. ");
                position(12,36); clearline();
                position(12,40); printf(" *Data no formato dd/mm/aaaa* ");
                position(29, 2); clearline();
                position(29, 2); printf("Digite [0] para voltar ao menu principal. ");
                position(12,26); fgets(PACIENTE.NASCIMENTO, sizeof(PACIENTE.NASCIMENTO), stdin);
                position(12,40); clearline();
                if (strlen(PACIENTE.NASCIMENTO) == 2)
                {
                    if(strncmp(PACIENTE.NASCIMENTO, "0", 1) == 0) return 0;
                }
            }
            strcpy(NASCIMENTO, PACIENTE.NASCIMENTO); //SALVA NOVA STRING ANTES DE DIVIDÍ-LA EM 3

            /// CONVERSÃO DE DADOS DA DATA DE NASCIMENTO STRING PARA INTEIROS
            int i, vetor[3], DIA, MES, ANO;
            char *token = strtok (PACIENTE.NASCIMENTO,"/");
            int pos = 0;
            while (token != NULL && pos < 3)
            {
                vetor[pos++] = atoi(token);
                token = strtok (NULL, "/");
            }
            for (i = 0; i < 3; ++i)
            {
                PACIENTE.DIA = vetor[0];
                PACIENTE.MES = vetor[1];
                PACIENTE.ANO = vetor[2];
            }

            // E-MAIL DO PACIENTE
            position(13,53); printf(" *Digite um e-mail válido* ");
            position(13,26); fgets(PACIENTE.EMAIL, sizeof(PACIENTE.EMAIL), stdin);
            position(13,53); clearline();
            if (strncmp(PACIENTE.EMAIL, "0", 1) == 0) break;
            while (validarEmail(PACIENTE.EMAIL) != 1)
            {
                position(13,26); clearline();
                position(29, 2); clearline();
                position(13,36); printf("Formato incorreto! ");
                position(29, 2); pause("Tecle <Enter> para digitar novamente. ");
                position(13,26); clearline();
                position(13,53); printf(" *Digite um e-mail válido* ");
                position(29, 2); clearline();
                position(29, 2); printf("Digite [0] para voltar ao menu principal. ");
                position(13,26); fgets(PACIENTE.EMAIL, sizeof(PACIENTE.EMAIL), stdin);
                position(13,53); clearline();
                if (strncmp(PACIENTE.EMAIL, "0", 1) == 0) return 0;
            }

            // TELEFONE DO PACIENTE
            position(14,42); printf(" *Somente os números com DDD* ");
            position(14,26); fgets(PACIENTE.TELEFONE, sizeof(PACIENTE.TELEFONE), stdin);
            position(14,42); clearline();
            if (strlen(PACIENTE.TELEFONE) == 2)
                {
                    if(strncmp(PACIENTE.TELEFONE, "0", 1) == 0) break;
                }
            while (validarNumeros(PACIENTE.TELEFONE) != 1 || strlen(PACIENTE.TELEFONE) < 11)
            {
                position(14,26); clearline();
                position(29, 2); clearline();
                position(14,36); printf("Número inválido!");
                position(29, 2); pause("Tecle <Enter> para digitar novamente. ");
                position(14,26); clearline();
                position(14,42); printf(" *Somente os números com DDD* ");
                position(29, 2); clearline();
                position(29, 2); printf("Digite [0] para voltar ao menu principal. ");
                position(14,26); fgets(PACIENTE.TELEFONE, sizeof(PACIENTE.TELEFONE), stdin);
                position(14,42); clearline();
                if (strlen(PACIENTE.TELEFONE) == 2)
                {
                    if(strncmp(PACIENTE.TELEFONE, "0", 1) == 0) return 0;
                }
            }

            // COMORBIDADES DO PACIENTE
            position(15,44); printf(" *Deixe vazio se não aplicável* ");
            position(15,26); fgets(PACIENTE.COMORBIDADES, sizeof(PACIENTE.COMORBIDADES), stdin);
            position(15,44); clearline();
            if (strncmp(PACIENTE.COMORBIDADES, "0", 1) == 0) break;

            // DATA DO DIAGNÓSTICO
            position(16,40); printf(" *Data no formato dd/mm/aaaa* ");
            position(16,26); fgets(DIAGNOSTICO, sizeof(DIAGNOSTICO), stdin);
            position(16,40); clearline();
            if (strlen(DIAGNOSTICO) == 2)
                {
                    if(strncmp(DIAGNOSTICO, "0", 1) == 0) break;
                }
            while (validarData(DIAGNOSTICO) != 1 || strlen(DIAGNOSTICO) != 11)
            {
                position(16,26); clearline();
                position(29, 2); clearline();
                position(16,36); printf("Formato incorreto!");
                position(29, 2); pause("Tecle <Enter> para digitar novamente. ");
                position(16,26); clearline();
                position(16,40); printf(" *Data no formato dd/mm/aaaa* ");
                position(29, 2); clearline();
                position(29, 2); printf("Digite [0] para voltar ao menu principal. ");
                position(16,26); fgets(DIAGNOSTICO, sizeof(DIAGNOSTICO), stdin);
                position(16,40); clearline();
                if (strlen(DIAGNOSTICO) == 2)
                {
                    if(strncmp(DIAGNOSTICO, "0", 1) == 0) return 0;
                }
            }

            /// - ENDEREÇO COMPLETO DO PACIENTE -
            // RUA DO PACIENTE
            position(19,26); fgets(PACIENTE.RUA, sizeof(PACIENTE.RUA), stdin);
            if (strlen(PACIENTE.RUA) == 2)
                {
                    if(strncmp(PACIENTE.RUA, "0", 1) == 0) break;
                }
            while (validarConteudo(PACIENTE.RUA) != 1)
            {
                position(19,26); clearline();
                position(29, 2); clearline();
                position(19,36); printf("Não deixe o campo em branco!");
                position(29, 2); pause("Tecle <Enter> para digitar novamente. ");
                position(19,26); clearline();
                position(19,30); clearline();
                position(29, 2); clearline();
                position(29, 2); printf("Digite [0] para voltar ao menu principal. ");
                position(19,26); fgets(PACIENTE.RUA, sizeof(PACIENTE.RUA), stdin);
                if (strlen(PACIENTE.RUA) == 2)
                {
                    if(strncmp(PACIENTE.RUA, "0", 1) == 0) return 0;
                }
            }

            // NÚMERO DA CASA DO PACIENTE
            position(20,26); fgets(PACIENTE.NUMERO, sizeof(PACIENTE.NUMERO), stdin);
            if (strlen(PACIENTE.NUMERO) == 2)
                {
                    if(strncmp(PACIENTE.NUMERO, "0", 1) == 0) break;
                }
            while (validarConteudo(PACIENTE.NUMERO) != 1)
            {
                position(20,26); clearline();
                position(29, 2); clearline();
                position(20,36); printf("Não deixe o campo em branco!");
                position(29, 2); pause("Tecle <Enter> para digitar novamente. ");
                position(20,26); clearline();
                position(20,30); clearline();
                position(29, 2); clearline();
                position(29, 2); printf("Digite [0] para voltar ao menu principal. ");
                position(20,26); fgets(PACIENTE.NUMERO, sizeof(PACIENTE.NUMERO), stdin);
                if (strlen(PACIENTE.NUMERO) == 2)
                {
                    if(strncmp(PACIENTE.NUMERO, "0", 1) == 0) return 0;
                }
            }

            // BAIRRO DO PACIENTE
            position(21,26); fgets(PACIENTE.BAIRRO, sizeof(PACIENTE.BAIRRO), stdin);
            if (strlen(PACIENTE.BAIRRO) == 2)
                {
                    if(strncmp(PACIENTE.BAIRRO, "0", 1) == 0) break;
                }
            while (validarConteudo(PACIENTE.BAIRRO) != 1)
            {
                position(21,26); clearline();
                position(29, 2); clearline();
                position(21,36); printf("Não deixe o campo em branco!");
                position(29, 2); pause("Tecle <Enter> para digitar novamente. ");
                position(21,26); clearline();
                position(21,30); clearline();
                position(29, 2); clearline();
                position(29, 2); printf("Digite [0] para voltar ao menu principal. ");
                position(21,26); fgets(PACIENTE.BAIRRO, sizeof(PACIENTE.BAIRRO), stdin);
                if (strlen(PACIENTE.BAIRRO) == 2)
                {
                    if(strncmp(PACIENTE.BAIRRO, "0", 1) == 0) return 0;
                }
            }

            // CIDADE DO PACIENTE
            position(22,26); fgets(PACIENTE.CIDADE, sizeof(PACIENTE.CIDADE), stdin);
            if (strncmp(PACIENTE.CIDADE, "0", 1) == 0) break;
            while (validarLetras(PACIENTE.CIDADE) != 1 || strlen(PACIENTE.CIDADE) < 3)
            {
                position(22,26); clearline();
                position(29, 2); clearline();
                position(22,36); printf("Dados inválidos! Digite apenas letras. ");
                position(29, 2); pause("Tecle <Enter> para digitar novamente. ");
                position(22,26); clearline();
                position(22,30); clearline();
                position(29, 2); clearline();
                position(29, 2); printf("Digite [0] para voltar ao menu principal. ");
                position(22,26); fgets(PACIENTE.CIDADE, sizeof(PACIENTE.CIDADE), stdin);
                if (strncmp(PACIENTE.CIDADE, "0", 1) == 0) return 0;
            }

            // ESTADO DO PACIENTE
            position(23,40); printf(" *Digite a sigla com 2 letras*  ");
            position(23,26); fgets(PACIENTE.ESTADO, sizeof(PACIENTE.ESTADO), stdin);
            position(23,40); clearline();
            if (strncmp(PACIENTE.ESTADO, "0", 1) == 0) break;
            while (validarLetras(PACIENTE.ESTADO) != 1 || strlen(PACIENTE.ESTADO) != 3)
            {
                position(23,26); clearline();
                position(29, 2); clearline();
                position(23,36); printf("Formato incorreto!");
                position(29, 2); pause("Tecle <Enter> para digitar novamente. ");
                position(23,26); clearline();
                position(23,40); clearline();
                position(23,40); printf(" *Digite a sigla com 2 letras*  ");
                position(29, 2); clearline();
                position(29, 2); printf("Digite [0] para voltar ao menu principal. ");
                position(23,26); fgets(PACIENTE.ESTADO, sizeof(PACIENTE.ESTADO), stdin);
                position(23,40); clearline();
                if (strncmp(PACIENTE.ESTADO, "0", 1) == 0) return 0;
            }

            // CEP DO PACIENTE
            position(24,40); printf(" *Digite somente números* ");
            position(24,26); fgets(PACIENTE.CEP, sizeof(PACIENTE.CEP), stdin);
            position(24,40); clearline();
            if (strlen(PACIENTE.CEP) == 2)
                {
                    if(strncmp(PACIENTE.CEP, "0", 1) == 0) break;
                }
            while (validarNumeros(PACIENTE.CEP) != 1 || strlen(PACIENTE.CEP) != 9)
            {
                position(24,26); clearline();
                position(29, 2); clearline();
                position(24,36); printf("Dados inválidos! Digite apenas números. ");
                position(29, 2); pause("Tecle <Enter> para digitar novamente. ");
                position(24,26); clearline();
                position(24,40); printf(" *Digite somente números* ");
                position(29, 2); clearline();
                position(29, 2); printf("Digite [0] para voltar ao menu principal. ");
                position(24,26); fgets(PACIENTE.CEP, sizeof(PACIENTE.CEP), stdin);
                position(24,40); clearline();
                position(29, 2); clearline();
                if (strlen(PACIENTE.CEP) == 2)
                {
                    if(strncmp(PACIENTE.CEP, "0", 1) == 0) return 0;
                }
            }

              /// - CRIAÇÃO DOS ARQUIVOS DE TEXTOS -
              // GRAVAÇÃO DOS DADOS NO ARQUIVO GERAL DE PACIENTES
              PTRARQGERAL = fopen("PACIENTES.TXT", "a");
              fprintf(PTRARQGERAL,"\nNome: %s",PACIENTE.NOME);
              fprintf(PTRARQGERAL,"CPF: %s", PACIENTE.CPF);
              fprintf(PTRARQGERAL,"Data de Nascimento: %s", NASCIMENTO);
              fprintf(PTRARQGERAL,"E-mail: %s",PACIENTE.EMAIL);
              fprintf(PTRARQGERAL,"Telefone: %s", PACIENTE.TELEFONE);
              fprintf(PTRARQGERAL,"Comorbidades: %s",PACIENTE.COMORBIDADES);
              fprintf(PTRARQGERAL,"Data do Diagnóstico: %s", DIAGNOSTICO);
              fprintf(PTRARQGERAL,"Rua: %s",PACIENTE.RUA);
              fprintf(PTRARQGERAL,"Número: %s",PACIENTE.NUMERO);
              fprintf(PTRARQGERAL,"Bairro: %s",PACIENTE.BAIRRO);
              fprintf(PTRARQGERAL,"Cidade: %s",PACIENTE.CIDADE);
              fprintf(PTRARQGERAL,"Estado: %s",PACIENTE.ESTADO);
              fprintf(PTRARQGERAL,"CEP: %s",PACIENTE.CEP);
              fflush(stdout);
              fclose(PTRARQGERAL);

              /// CALCULO DA IDADE DO PACIENTE
              PACIENTE.IDADE = calculaIdade(PACIENTE.DIA, PACIENTE.MES, PACIENTE.ANO);
              position(26, 2); printf("O paciente tem %d anos de idade.", PACIENTE.IDADE);

              // VERIFICA IDADE E COMORBIDADES PARA GRAVAÇÃO EM ARQUIVO DE GRUPO DE RISCO
              if (PACIENTE.IDADE >= 65 || strlen(PACIENTE.COMORBIDADES) > 3)
              {
              PTRARQRISCO = fopen("EM_RISCO.TXT", "a");
              fprintf(PTRARQRISCO,"\nIdade: %d\n",PACIENTE.IDADE);
              fprintf(PTRARQRISCO,"CEP: %s",PACIENTE.CEP);
              fflush(stdout);
              fclose(PTRARQRISCO);
              }

              // FINALIZAÇÃO DO CADASTRO
              position(27, 1); clearline();
              position(29, 2); clearline();
              position(28, 2); printf("Cadastro realizado com sucesso! ");
              position(29, 2);
              pause("Tecle <Enter> para novo cadastro. ");
              position( 26, 1); clearline();
              position( 28, 1); clearline();
              position( 29, 1); clearline();

    }
  while (FIM != 0);
}

/// FUNÇÃO CALCULADORA DE IDADE
// RETORNA A IDADE DOS PACIENTES

int calculaIdade(int DIA, int MES, int ANO)
{
     int IDADE;
     struct tm *DATA;
     time_t segundos;
     time(&segundos);
     DATA = localtime(&segundos);

     if ((MES > DATA->tm_mon+1) || ((MES = DATA->tm_mon+1) && (DIA > DATA->tm_mday)))
        {
        IDADE = DATA->tm_year+1900 - ANO;
        }
        else
        {
        IDADE = DATA->tm_year+1900 - ANO;
        }

     return IDADE;
}

/// MÓDULO DE PESQUISA DE PACIENTES
// INFORMA SE UM PACIENTE JÁ ENCONTRA-SE CADASTRADO NO SISTEMA

void pesquisar(void)
{
  char INPUT[20], GERAL[2500];

  do
  {
      clear();
      position(5, 2);
      position(1, 1); line("-");
      center(2, TITULO);
      position(3, 1); line("-");
      center(5, "- Pesquisar Paciente -\n");
      position( 8, 2); printf("Digite o CPF do paciente: ");
      position(28, 1); clearline();
      position(29, 2); printf("Digite [0] para voltar ao menu principal. ");

      PTRARQGERAL = fopen("PACIENTES.TXT", "r");

      if (PTRARQGERAL == NULL)
      {
        position(29, 1); clearline();
        position(8, 2); clearline();
        position(10, 25); printf("Não há pacientes cadastrados! ");
        position(29, 2); pause("Tecle <Enter> para voltar ao menu principal. ");
        position(10, 25); clearline();
        position(29, 1); clearline();
        return 0;
      }

      position(8,28); fgets(INPUT, sizeof(INPUT), stdin);
      if (strlen(INPUT) == 2)
                {
                    if(strncmp(INPUT, "0", 1) == 0) break;
                }
      while (validarNumeros(INPUT) != 1 || strlen(INPUT) != 12)
      {
        position(10,26); clearline();
        position(29, 2); clearline();
        position(8, 2); clearline();
        position(10,20); printf("Dados inválidos! Digite apenas números. ");
        position(29, 2); pause("Tecle <Enter> para digitar novamente. ");
        position(10,20); clearline();
        position( 8, 2); printf("Digite o CPF do paciente: ");
        position(29, 2); clearline();
        position(29, 2); printf("Digite [0] para voltar ao menu principal. ");
        position(8,28); fgets(INPUT, sizeof(INPUT), stdin);
        if (strlen(INPUT) == 2)
                {
                    if(strncmp(INPUT, "0", 1) == 0) return 0;
                }
      }

      char BUSCA[61] = "CPF: ";
      strcat(BUSCA, INPUT);

      rewind(PTRARQGERAL);

      while (fread(&GERAL, sizeof(char), 2500, PTRARQGERAL))
      {
        if(strstr(GERAL, BUSCA) != NULL)
        {
            position(29, 2); clearline();
            position(12, 2); printf("Paciente já cadastrado!\n");
            position(14, 2); printf("%s\n", BUSCA);
        }
        else
        {
            position(29, 2); clearline();
            position(12, 2); printf("Paciente não cadastrado!\n");
        }

        position(29, 2); pause("Tecle <Enter> para nova pesquisa. ");
        fflush(stdout);
        fclose(PTRARQGERAL);
      }
    }
  while (FIM != '0');
}

/// MÓDULO DE LISTAGEM DOS PACIENTES NO GRUPO DE RISCO
// APRESENTA NA TELA A IDADE E O CEP DOS PACIENTES CADASTRADOS

void risco(void)
{
  char RISCO[400];

      clear();
      position(5, 2);
      position(1, 1); line("-");
      center(2, TITULO);
      position(3, 1); line("-");
      center(5, "- Lista de Pacientes no Grupo de Risco -\n");
      position(28, 1); clearline();

      PTRARQRISCO = fopen("EM_RISCO.TXT", "r");

      if (PTRARQRISCO == NULL)
      {
        position(29, 1); clearline();
        position(12, 22); printf("Não há pacientes no Grupo de Risco! ");
        position(29, 2); pause("Tecle <Enter> para voltar ao menu principal. ");
        position(12, 22); clearline();
        position(29, 1); clearline();
        return 0;
      }

      while (fread(&RISCO, sizeof(char), 400, PTRARQRISCO))
      {
        position(8, 1); printf("%s", RISCO); printf("\n Fim dos resultados!\n\n ");
        pause("Tecle <Enter> para voltar ao menu principal. ");
        fflush(stdout);
        fclose(PTRARQRISCO);
        return 0;
      }
}

/// MÓDULO QUE REMOVE O ARQUIVO TXT DE PACIENTES CADASTRADOS
// CRIA UM NOVO ARQUIVO "BACKUP" DO PACIENTES.TXT E EXCLUI O ORIGINAL

void remover(void)
{
  char INPUT[20], OPCAO[20], BACKUP[10];
  char SEGURANCA[10] = "RESET!";

  do
  {
      clear();
      position(5, 2);
      position(1, 1); line("-");
      center(2, TITULO);
      position(3, 1); line("-");
      center(5, "- Remover Arquivo de Pacientes Cadastrados -\n");
      position( 8, 7); printf("Tem certeza que deseja apagar a lista de pacientes cadastrados? ");
      position(24, 4); printf("Executar esta função resultará no backup do arquivo PACIENTES.TXT atual. ");
      position(26, 3); printf("Um novo arquivo PACIENTES.TXT será recriado no próximo cadastro realizado. ");
      position(10, 7); printf("Digite 'S' para confirmar e 'N' ou [0] para cancelar: ");
      position(28, 1); clearline();
      position(29, 2); printf("Digite [0] para voltar ao menu principal. ");

      PTRARQGERAL = fopen("PACIENTES.TXT", "r");

      if (PTRARQGERAL == NULL)
      {
        position(10, 7); clearline();
        position(29, 1); clearline();
        position(8, 2); clearline();
        position(10, 25); printf("Não há pacientes cadastrados! ");
        position(29, 2); pause("Tecle <Enter> para voltar ao menu principal. ");
        position(10, 25); clearline();
        position(29, 1); clearline();
        return 0;
      }

        position(10,61); fgets(OPCAO, sizeof(OPCAO), stdin);

      if (strncmp(OPCAO, "0", 1) == 0 || (strncmp(OPCAO, "N", 1) == 0)) break;
      while (validarConteudo(OPCAO) != 1 || (strncmp(OPCAO, "S", 1) != 0) || strlen(OPCAO) > 2)
      {
        position(29, 2); clearline();
        position(10,61); clearline();
        position(14, 31); printf("Opção inválida! ");
        position(29, 2); pause("Tecle <Enter> para digitar novamente. ");
        position(14,20); clearline();
        position( 8, 7); printf("Tem certeza que deseja apagar a lista de pacientes cadastrados? ");
        position(10, 7); printf("Digite 'S' para confirmar e 'N' ou [0] para cancelar: ");
        position(29, 2); clearline();
        position(29, 2); printf("Digite [0] para voltar ao menu principal. ");
        position(10,61); fgets(OPCAO, sizeof(OPCAO), stdin);
        if (strncmp(OPCAO, "0", 1) == 0 || (strncmp(OPCAO, "N", 1) == 0)) return 0;
      }

        position(14,23); printf("Digite o código de segurança:");
        position(14,53); fgets(INPUT, sizeof(INPUT), stdin);

        while (strncmp(SEGURANCA, INPUT, 6) != 0 || strlen(INPUT) > 7)
      {
        position(29, 2); clearline();
        position(10,61); clearline();
        position(16, 29); printf("Código incorreto! ");
        position(29, 2); pause("Tecle <Enter> para voltar ao menu principal. ");
        return 0;
      }

        PTRARQBACKUP = fopen("BACKUP.TXT", "w");

        char c = fgetc(PTRARQGERAL);
        while (c != EOF)
        {
            fputc(c, PTRARQBACKUP);
            c = fgetc(PTRARQGERAL);
        }

        fflush(stdout);
        fclose(PTRARQGERAL);
        fclose(PTRARQBACKUP);

        remove("PACIENTES.TXT");

        position(18,23); printf("Operação realizada com sucesso!");
        position(29, 2); pause("Tecle <Enter> para voltar ao menu principal. ");
        return 0;
        }

  while (FIM != '0');
}

/// TELA DA FICHA DE CADASTRO DOS PACIENTES

void telaCadastro(void)
{
  position(10,17); clearline();
  position(11,17); clearline();
  position(12,17); clearline();
  position(13,17); clearline();
  position(14,17); clearline();
  position(15,17); clearline();
  position(16,17); clearline();
  position(17,17); clearline();
  position(18,17); clearline();
  position(19,17); clearline();
  position(20,17); clearline();
  position(21,17); clearline();
  position(22,17); clearline();
  position(23,17); clearline();
  position(24,17); clearline();
  position(10, 2); printf("Nome Completo . . . . : ");
  position(11, 2); printf("CPF . . . . . . . . . : ");
  position(12, 2); printf("Data de Nascimento. . : ");
  position(13, 2); printf("E-mail. . . . . . . . : ");
  position(14, 2); printf("Telefone. . . . . . . : ");
  position(15, 2); printf("Comorbidades. . . . . : ");
  position(16, 2); printf("Data do Diagnóstico . : ");
  position(17, 2); printf(" ");
  position(18, 2); printf("      - Endereço -      ");
  position(19, 2); printf("Rua . . . . . . . . . : ");
  position(20, 2); printf("Número. . . . . . . . : ");
  position(21, 2); printf("Bairro. . . . . . . . : ");
  position(22, 2); printf("Cidade. . . . . . . . : ");
  position(23, 2); printf("Estado. . . . . . . . : ");
  position(24, 2); printf("CEP . . . . . . . . . : ");
}

/// FUNÇÕES DE VERIFICAÇÃO DE DADOS DIGITADOS
// VERIFICA SE O QUE O USUÁRIO DIGITOU É VÁLIDO

int validarLetras(char *INPUT)
{
    if (strlen(INPUT) == 0)
    return 0;

    if (isspace(INPUT[0]))
    return 0;

    for (int i = 0; INPUT[i] != '\0'; i++)
    {
        char c = INPUT[i];
        if (isdigit(c) || ispunct(c))
        {
        return 0;
        }
    }

    return 1;
}

int validarNumeros(char *INPUT)
{
    if (strlen(INPUT) == 0)
    return 0;

    for (int i = 0; INPUT[i - 1] != '\0'; i++)
    {
        char c = INPUT[i];
        if ((isalpha(c)) || (ispunct(c)) || isspace(c))
        {
        return 0;
        }
    }
    return 1;
}

int validarData(char *INPUT)
{
    if (strlen(INPUT) == 0)
    return 0;

    int barra = 0, i;

    for (i = 0; INPUT[i] != '\0'; i++)
    {
        if (isspace(INPUT[i - 1]) || isalpha(INPUT[i - 1]))
            {
            return 0;
            }

        char c = INPUT[i];
        if(c == '/')
        {
              barra++;
        }
    }
    while (i != 10 && barra != 2)
    {
      return 0;
    }
    return 1;
}

int validarEmail(char *INPUT)
{
    if (strlen(INPUT) == 0)
    return 0;

    int arroba = 0, ponto = 0, antesPonto = 0, depoisPonto = 0, i;
    int tamanho = strlen(INPUT);

    for (i = 0; INPUT[i] != '\0'; i++)
    {
        if (isspace(INPUT[i - 1]))
            {
            return 0;
            }

        char c = INPUT[i];
        if(c == '@')
        {
              if (arroba)
                return 0; // não pode ter uma segunda @
                arroba = 1;
              if (i < 3)
                return 0; // se @ vier antes de 3 caracteres, erro
        }
        else if (arroba)
        { // se já encontrou @
              if (ponto)
              { // se já encontrou . depois de @
                depoisPonto++;
              }
              else if(c == '.')
              {
                ponto = 1;
                if (antesPonto < 3)
                {
                  return 0; // se . depois de @ vier antes de 3 caracteres, erro
                }
              }
              else
              {
                antesPonto++;
              }
        }
    }
    if (i == tamanho && depoisPonto > 1)
        return 1;
    else
        return 0;
}

int validarConteudo(char *INPUT)
{
    int RET = INPUT;
    if (RET == 0)
    return 0;

    if (strlen(INPUT) == 0)
    return 0;

    if (isspace(INPUT[0]))
    return 0;

    if (ispunct(INPUT[0]))
    return 0;

return 1;
}

/// SUB-ROTINAS GERAIS PARA CONTROLE DO PROGRAMA
// TRAÇA LINHA

void line(const char *TIPO)
{
  int POSICAO;
  if (TIPO == "-" or TIPO == "=")
    for (POSICAO = 0; POSICAO <= 79; POSICAO++)
      printf(TIPO);
  else
    for (POSICAO = 0; POSICAO <= 79; POSICAO++)
      printf("-");
}

// CENTRALIZA MENSAGEM NO MONITOR

void center(int LINHA, const char *MENSAGEM)
{
  #include <string.h>
  int COLUNA;
  COLUNA = ((80 - strlen(MENSAGEM)) / 2);
  position(LINHA, COLUNA);
  printf(MENSAGEM);
}


/// AÇÕES EXCLUSIVAS PARA MODO: WINDOWS API

#if defined _WIN32 || defined _WIN64

  #include <windows.h>

  // Limpar tela
  void clear(void)
  {
    HANDLE TELA;
    DWORD ESCRITA = 0;
    COORD POS;
    TELA = GetStdHandle(STD_OUTPUT_HANDLE);
    POS.X = 0;
    POS.Y = 0;
    FillConsoleOutputCharacter(TELA, 32, 80 * 70, POS, &ESCRITA);
  }

  // Posicionar cursor nos limites 80 x 24
  void position(int LINHA, int COLUNA)
  {
    if (COLUNA >= 1 && COLUNA <= 80 && LINHA >= 1 && LINHA <= 35)
    {
      HANDLE TELA;
      COORD POS;
      TELA = GetStdHandle(STD_OUTPUT_HANDLE);
      POS.X = COLUNA - 1;
      POS.Y = LINHA - 1;
      SetConsoleCursorPosition(TELA, POS);
    }
  }

  // Limpar linha a partir da posicao do cursor
  void clearline(void)
  {
    HANDLE TELA;
    COORD POS;
    CONSOLE_SCREEN_BUFFER_INFO VIDEO;
    DWORD ESCRITA = 0;
    TELA = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(TELA, &VIDEO);
    POS.Y = VIDEO.dwCursorPosition.Y;
    POS.X = VIDEO.dwCursorPosition.X;
    FillConsoleOutputCharacter(TELA, 32, 80 - POS.X , POS, &ESCRITA);
  }

/// AÇÕES EXCLUSIVAS PARA MODO: TERMINAL ANSI

#else

  // Limpar tela
  void clear(void)
  {
    printf("\033[2J");
  }

  // Limpar linha a partir da posicao do cursor
  void position(int LINHA, int COLUNA)
  {
    if (COLUNA >= 1 && COLUNA <= 80 && LINHA >= 1 && LINHA <= 35)
      printf("\033[%i;%iH", LINHA, COLUNA);
  }

  // Limpar linha a partir da posicao do cursor
  void clearline(void)
  {
    printf("\033[K");
  }

#endif
