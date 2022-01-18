#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RedeSocial.h"

int main (void) {
    int escolha, vef1, vef2;//variaves que detectam a validez da entrada
    char nome[50]; //nome para a criação de uma cadastro
    char apelido[10];//apelido para a criação de um cadastro
    char parceiro[10];//aquele que recebe um pedido de amizade ou que é enviado uma mensagem 
    char mensagem[100];//utilizada para enviar mensagens entre os usuarios
    Lista* L; // lista de cadastro
    Cadastro *usuario, *aux, *aux2; //Utilizados na criação de um cadastro para a lista L
    Bloco* aux3;//Auxilia na busca do parceiro para mandar mensagem 
    printf("Caro usuário, suas opções são:\n");
    printf("\t1)\tcadastrar um usuário\n");
    printf("\t2)\tlistar usuários cadastrados e suas informações\n");
    printf("\t3)\tpedir para ser parceiro de um usuário\n");
    printf("\t4)\tavaliar lista de pedidos de parceria\n");
    printf("\t5)\tenviar mensagem para um parceiro\n");
    printf("\t6)\tver mensagens recebidas\n");
    printf("\t7)\tsugerir novas parcerias\n");
    printf("\t8)\treinicializar sistema\n");
    printf("\t0)\tparar de executar o sistema (reinicializando-a antes de encerrar)\n");
    L = Criar_Lista();
    //cadastro do usuario
    while(1) {
        printf("\nO que deseja fazer?\nResposta: ");
        int teste = scanf("%d", &escolha);
        //No caso da entrada ser uma string
        if(teste!=1){
            char c;
            do {
                // Usamos a função getchar para limpar o buffer de entrada..
                c = getchar();
                } while(c != '\n' && c != EOF);
            printf("Entrada invalida. Digite um numero!\n");
            continue;
        }
        if (escolha == 1) {
            printf("\tEntre com seu nome: ");
            scanf(" %[^\n]", nome);
            printf("\tEntre com seu apelido: ");
            scanf(" %[^\n]", apelido);
            //caso já haja usuario cadastrado
            if (L->inicio != NULL) {
                aux = L->inicio;
                while (aux != NULL) {
                    //verifica se o usuario já foi cadastrado
                    if ((strcmp(aux->nome, nome) == 0) && (strcmp(aux->apelido, apelido) == 0)) {
                        printf("Esse usuário já foi cadastrado!\n");
                        break;
                    }
                    aux2 = aux;
                    aux = aux->prox;
                }
                if (aux2->prox == NULL) {
                    usuario = Registro(nome, apelido);//criação
                    aux2->prox = usuario;//Adiciona-se o usuario ao final da fila
                    printf("Usuário cadastrado com sucesso!\n");
                }
            }
            else {
                //Adicina-se o primeiro usuario a fila
                usuario = Registro(nome, apelido);
                L->inicio = usuario;
                printf("Usuário cadastrado com sucesso!\n");
            }
        }
        else if (escolha == 2) { //lista dos usuarios cadastrados
            printf("\tPessoas e seus parceiros:\n");
            Listar(L);
            printf("Listagem completa!\n");
        }
        else if (escolha == 3) { //pedido de parceria
            printf("\tEntre com seu apelido: ");
            scanf(" %[^\n]", apelido);
            printf("\tEntre com o apelido de quem quer ser parceiro: ");
            scanf(" %[^\n]", parceiro);
            aux = L->inicio;
            vef1 = 0;
            vef2 = 0;
            //verificação de validez da entrada
            while (aux != NULL) {
                if (strcmp(aux->apelido, apelido) == 0) vef1 = 1;//apelido válido
                if (strcmp(aux->apelido, parceiro) == 0) vef2 = 1;//parceiro válido
                aux = aux->prox;//atualiza a busca
            }
            if ((!vef1) && (vef2)) printf("Seu apelido não foi encontrado no sistema!\n");
            else if ((vef1) && (!vef2)) printf("O apelido de quem quer ser parceiro não foi encontrado no sistema!\n");
            else if ((!vef1) && (!vef2)) printf("O seu apelido e de quem quer ser parceiro não foram encontrados no sistema!\n");
            else {
                Parceria(L, apelido, parceiro); // encaminhamento do pedido 
                printf("Pedido encaminhado com sucesso!\n");
            }
        }
        else if (escolha == 4) { //avaliar pedidos de parceria
            printf("\tEntre com seu apelido: ");
            scanf(" %[^\n]", apelido);
            aux = L->inicio;
            vef1 = 0;
            // verifica-se a validez da entrada
            while (aux != NULL) {
                if (strcmp(aux->apelido, apelido) == 0) {
                    vef1 = 1;
                    break;
                }
                aux = aux->prox;
            }
            if (!vef1) printf("Seu apelido não foi encontrado no sistema!\n");
            else {
                Avaliar(L, apelido);//chamada da função de avaliação
                printf("Operação realizada com sucesso.\n");
            }
        }
        else if (escolha == 5) {//enviar mensagem
            printf("\tEntre com seu apelido: ");
            scanf(" %[^\n]", apelido);
            printf("\tEntre com o apelido de quem quer enviar mensagem: ");
            scanf(" %[^\n]", parceiro);
            printf("\tEntre com a mensagem: ");
            scanf(" %[^\n]", mensagem);
            aux = L->inicio;
            vef1 = 0;
            vef2 = 0;
            //verificaçao do apelido
            while (aux != NULL) {
                if (strcmp(aux->apelido, apelido) == 0) {
                    vef1 = 1;
                    break;
                }
                aux = aux->prox;
            }
            //verficação do apelido do parceiro
            if (vef1) { // se o apelido é válido
                aux3 = aux->parceiros;
                while (aux3 != NULL) {
                    if (strcmp(aux3->apelido_parceiro, parceiro) == 0) {
                        vef2 = 1;
                        break;
                    }
                    aux3 = aux3->prox;
                }
            }
            if (!vef1) printf("Seu apelido não foi encontrado no sistema!\n");
            else if (!vef2) printf("O apelido de quem você quer enviar a mensagem não foi encontrado na sua lista de parceiros!\n");
            else {
                Enviar(L, apelido, parceiro, mensagem);//chamada da função de envio
                printf("Mensagem enviada com sucesso.\n");
            }
        }
        else if (escolha == 6) { //ver mensagens recebidas
            printf("\tEntre com seu apelido: ");
            scanf(" %[^\n]", apelido);
            aux = L->inicio;
            vef1 = 0;
            //verificação do apelido
            while (aux != NULL) {
                if (strcmp(aux->apelido, apelido) == 0) {
                    vef1 = 1;
                    break;
                }
                aux = aux->prox;
            }
            if (!vef1) printf("Seu apelido não foi encontrado no sistema!\n");
            else {
                Ler(L, apelido);//chamada da função de leitura
                printf("Mensagens exibidas!\n");
            }
        }
        else if (escolha == 7) {//listar sugestões

            aux = L->inicio;
            //verifica se a lista está vazia
            if(aux==NULL) printf("\tAinda não há usuário cadastrado.\n");
            else{
                while(aux!=NULL){
                    Sugerir(L, aux->apelido);//chamada da função de sugestão
                    aux = aux->prox; //atualiza o usuario
                }
            }
            printf("Operação realizada com sucesso.\n");
           
        }
        else if (escolha == 8) {//renicializar o sistema
            if (L->inicio == NULL) printf("O sistema já foi reinicializado e está pronto para receber novos usuários!\n");
            else {
                Reiniciar(L);// chamada da função de reinicialização
                printf("Operação realizada com sucesso.\n");
            }
        }
        else if (escolha == 0) { //encerra a execução
            Reiniciar(L);//chamada da função de reinicialização
            break;
        }

        else{
            printf("\tEntrada inválida.\n");;      
        }

        
    }
    printf("\nObrigado por usar nosso sistema!\n");
    return(0);
}
