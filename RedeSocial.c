#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RedeSocial.h"

Lista* Criar_Lista (void) {
    Lista* L = (Lista*) malloc(sizeof(Lista)); //reserva um espaço na memória para a lista
    if (L != NULL) L->inicio = NULL; //se o espaço for reservado, o ponteiro inicio recebe NULL
    return(L);
}

Fila* Criar_Fila (void) {
    Fila* F = (Fila*) malloc(sizeof(Fila)); //reserva um espaço na memória para a fila
    if (F != NULL) { //se o espaço for reservado, os ponteiros inicio e fim recebem NULL
        F->inicio = NULL;
        F->fim = NULL;
    }
    return(F);
}

Pilha* Criar_Pilha (void) {
    Pilha* P = (Pilha*) malloc(sizeof(Pilha)); //reserva um espaço na memória para a pilha
    if (P != NULL) P->topo = NULL; //se o espaço for reservado, o ponteiro topo recebe NULL
    return(P);
}

Cadastro* Registro (char* usuario, char* apelido_usuario) {
    Cadastro* novo = (Cadastro*) malloc(sizeof(Cadastro)); //reserva um espaço na memória para um novo usuário
    if (novo != NULL) { //se o espaço for reservado, as informações do novo usuário são guardadas
        strcpy(novo->nome, usuario);
        strcpy(novo->apelido, apelido_usuario);
        novo->prox = NULL;
        novo->parceiros = NULL;
        novo->mensagens = NULL;
        novo->pedidos = NULL;
    }
    return(novo);
}

void Listar (Lista* L) {
    Cadastro* aux = L->inicio; //ponteiro auxiliar que percorre a lista de usuários
    Bloco* aux_parceiro; //ponteiro auxiliar que percorre a lista de parceiros
    if (aux == NULL) printf("\tAinda não há usuários cadastrados no sistema!\n");
    while (aux != NULL) {
        //aux_parceiro aponta para o primeiro parceiro do usuário em questão
        aux_parceiro = aux->parceiros;
        printf("\t%s com os seguintes parceiros:", aux->apelido);
        //se o usuário não tem parceiros, avisa isso na saída
        if (aux_parceiro == NULL) printf(" sem parceiros\n");
        else {
            //percorre até o penúltimo parceiro, para sair com a formatação correta
            while(aux_parceiro->prox != NULL) {
                printf(" %s,", aux_parceiro->apelido_parceiro);
                aux_parceiro = aux_parceiro->prox;
            }
            //
            printf(" %s\n", aux_parceiro->apelido_parceiro);
        }
        aux = aux->prox;
    }
}

void Parceria (Lista* L, char* origem, char* destino) {
    Cadastro* aux = L->inicio; //aux varre a lista de usuários
    Bloco* B = (Bloco*) malloc(sizeof(Bloco)); //reserva um espaço na memória para um pedido de parceria
    strcpy(B->apelido_parceiro, origem);
    B->prox = NULL;
    while(strcmp(aux->apelido, destino) != 0) aux = aux->prox; //procura a quem foi feito o pedido de parceria
    if (aux->pedidos == NULL) { //se o usuário não recebeu nenhum pedido de parceria ainda, é necessário criar a fila que guarda esses pedidos
        Fila* F;
        F = Criar_Fila();
        aux->pedidos = F;
        aux->pedidos->inicio = B;
    }
    else aux->pedidos->fim->prox = B; //adiciona o pedido à fila do destinatário
    aux->pedidos->fim = B; //altera o final da fila de pedidos
}

void Avaliar (Lista* L, char* usuario) {
    Cadastro *aux, *aux_parceiro; //aux varre a lista de usuários
    Bloco *aux_pedido, *aux_montar, *B;//blocos para a análise dos pedidos e mudanças na fila
    char resp[5];
    aux = L->inicio;
    while(strcmp(aux->apelido, usuario) != 0) aux = aux->prox;//procura o usuario que deseja ver os pedidos
    if (aux->pedidos == NULL) printf("\tNão há pedidos de parceria!\n");
    else { //Analisa-se o pedido
        aux_pedido = aux->pedidos->inicio;
        while(aux_pedido != NULL) {
            printf("\t%s deseja ser seu(sua) parceiro(a). Aceita (sim/não)? ", aux_pedido->apelido_parceiro);
            scanf("%s", resp);
            if (strcmp(resp, "sim") == 0) { //Alteração no Bloco de parceiros do usuario
                if (aux->parceiros == NULL) aux->parceiros = aux_pedido;//Caso não tenha parceiro
                else { //Situação que já tinha ao menos um parceiro
                    aux_montar = aux->parceiros;
                    while (aux_montar->prox != NULL) aux_montar = aux_montar->prox;
                    aux_montar->prox = aux_pedido;
                }
                aux_parceiro = L->inicio; // Alteração no Bloco de parceiros daquele que enviou o pedido
                //Localiza quem mandou o pedido na Lista L
                while(strcmp(aux_parceiro->apelido, aux_pedido->apelido_parceiro) != 0) aux_parceiro = aux_parceiro->prox;
                B = (Bloco*) malloc (sizeof(Bloco));
                strcpy(B->apelido_parceiro, usuario);
                B->prox = NULL;
                if (aux_parceiro->parceiros == NULL) aux_parceiro->parceiros = B;
                else {
                    aux_montar = aux_parceiro->parceiros;
                    while (aux_montar->prox != NULL) aux_montar = aux_montar->prox;
                    aux_montar->prox = B;
                }
                aux->pedidos->inicio = aux->pedidos->inicio->prox;
                aux_pedido->prox = NULL;
            }
            else if (strcmp(resp, "não") == 0) {
                aux->pedidos->inicio = aux->pedidos->inicio->prox;
                free(aux_pedido);//pedido negado e retirado da fila
            }
            aux_pedido = aux->pedidos->inicio;//atualiza-se a fila de pedidos
        }
    }
}

void Enviar(Lista* L, char* origem, char* destino, char* msg) {
    Cadastro* aux = L->inicio; //aux varre a lista de usuários
    Conjunto* C = (Conjunto*) malloc(sizeof(Conjunto)); //reserva um espaço na memória para a mensagem
    strcpy(C->remetente, origem);
    strcpy(C->mensagem, msg);
    C->prox = NULL;
    while (strcmp(aux->apelido, destino) != 0) aux = aux->prox; //procura o destinatário da mensagem
    if (aux->mensagens == NULL) { //se o usuário não recebeu mensagens ainda, cria-se a pilha onde elas serão armazenadas
        Pilha* P;
        P = Criar_Pilha();
        aux->mensagens = P;
    }
    else C->prox = aux->mensagens->topo;
    aux->mensagens->topo = C; //adiciona a mensagem à pilha
}

void Ler(Lista* L, char* usuario) {
    Cadastro* aux = L->inicio; //aux varre a lista de usuários
    Conjunto* aux_msg; //aux_msg varre a pilha de mensagens
    printf("\tSuas mensagens são:\n");
    while (strcmp(aux->apelido, usuario) != 0) aux = aux->prox; //procura pelo usuário que pediu para ler suas mensagens
    if (aux->mensagens == NULL) printf("\tNão há mensagens recebidas!\n");
    else {
        aux_msg = aux->mensagens->topo;
        while(aux_msg != NULL) {//se o usuário já recebeu alguma mensagem, elas são mostradas, junto com o remetente
            printf("\t(%s) %s\n", aux_msg->remetente, aux_msg->mensagem);
            aux_msg = aux_msg->prox;
        }
    }
}

void Sugerir(Lista *L, char* usuario ){

    Bloco* aux_parceiro; //ponteiro auxiliar que percorre a lista de parceiros
    Bloco* aux_parceiro2;//ponteiro que auxilia a responder a pergunta: o "parceiro do parceiro" é meu parceiro?
    Bloco* aux2_parceiro; //ponteiro auxiliar que percorre a lista de parceiros dos parceiros
    Cadastro* aux = L->inicio; //aux varre a lista de usuários
    Cadastro* aux2 = L->inicio; //aux2 auxilia o encontro do parceiro na lista L

    while (strcmp(aux->apelido, usuario) != 0) aux = aux->prox; // encontra o usuário

    aux_parceiro = aux->parceiros;

    //Cria-se uma fila para salvar as sugestões
    Fila* F1;
    F1 = Criar_Fila();

    //Se o usuario tem parceiros, analisa-se a fila deles
    if (aux_parceiro != NULL){

        //percorrer os parceiros 
        while(1) {

            //Encontrar o parceiro na lista L
            aux2 = L -> inicio;
            while (strcmp(aux2->apelido, aux_parceiro->apelido_parceiro) != 0) aux2 = aux2->prox; 

            //Inicio da análise dos "pareceiros dos parceiros"
            //aux2_parceiro recebe a "fila de parceiros" do parceiro
            aux2_parceiro = aux2 -> parceiros;

            //Se a fila não for vazia, analisa-se se os elementos vão para F1 
            if(aux2_parceiro != NULL){
                while(1){

                    //(aux2_parceiro->apelido_parceiro) é parceiro de aux?
                    aux_parceiro2 = aux->parceiros;
                    //O "parceiro do parceiro" é o usuario?
                    if(!strcmp(aux2_parceiro->apelido_parceiro,usuario)){
                        if(aux2_parceiro->prox!=NULL){
                            aux2_parceiro = aux2_parceiro->prox;
                            //testando
                            continue;
                        }
                        else break;
                    }

                    //O "parceiro do parceiro" é parceiro do usuario?
                    while (strcmp(aux2_parceiro->apelido_parceiro, aux_parceiro2->apelido_parceiro) != 0){
                        if(aux_parceiro2->prox!=NULL) aux_parceiro2 = aux_parceiro2->prox;
                        else break;
                    }
                    //testando
                    if(!strcmp(aux2_parceiro->apelido_parceiro,aux_parceiro2->apelido_parceiro)){
                        if(aux2_parceiro->prox!=NULL){
                            aux2_parceiro = aux2_parceiro->prox;
                            continue;
                        }
                        else break;
                    }

                    //Após as chacagens, adiciona-se o elemento a fila de sugestões       
                    if (F1->inicio == NULL){
                        Bloco* B = (Bloco*) malloc(sizeof(Bloco));
                        strcpy(B->apelido_parceiro, aux2_parceiro->apelido_parceiro);
                        B->prox = NULL;
                        F1->inicio = B;
                        F1->fim = B;
                    } 
                    else {
                        Bloco* B = (Bloco*) malloc(sizeof(Bloco));
                        strcpy(B->apelido_parceiro, aux2_parceiro->apelido_parceiro);
                        B->prox = NULL;

                        F1->fim->prox=B;
                        F1->fim = B;
                    }
                //Atualização dos "parceiros do parceiro"
                if(aux2_parceiro->prox == NULL) break;
                aux2_parceiro = aux2_parceiro->prox;
                }

            }
            //Atualização dos parceiros
            if(aux_parceiro->prox == NULL) break;
            aux_parceiro = aux_parceiro->prox;
        }
    }

    //Mostra-se o rsultado da Fila F1
    Bloco* aux_fila = F1->inicio;
    //Se não houver sugestão
    if(aux_fila==NULL){
        printf("\tSugestão de nova parceria para %s: Não há sugestão.\n", usuario);
    }
    else{  //caso haja alguma sugestão
        printf("\tSugestão de nova parceria para %s: ", usuario);
        while (aux_fila!=NULL){
            //percorrer fila
        
            if(aux_fila->prox!=NULL) printf("%s, ", aux_fila->apelido_parceiro);
            else printf("%s\n", aux_fila->apelido_parceiro);
            aux_fila = aux_fila->prox;
        }
    }   
}

void Reiniciar (Lista *L) {
    Bloco *aux1, *aux2;//ponteiros utilizados para liberar o espaço dos pedidos e dos parceiros
    Cadastro *pessoa1, *pessoa2;//ponteiros usados para percorrer a Lista L e liberar espaço dos cadastros
    Conjunto *msg1, *msg2;//ponteiros utilizados para liberar o espaço das mensagens
    pessoa1 = L->inicio;
    while(pessoa1 != NULL) {
        if (pessoa1->mensagens != NULL) {
            msg1 = pessoa1->mensagens->topo;
            while (msg1 != NULL) {
                msg2 = msg1;
                msg1 = msg1->prox;
                free(msg2); //libera os espaços de memória associados às mensagens
            }
            free(pessoa1->mensagens); //libera a memória associada à pilha
            pessoa1->mensagens = NULL;
        }
        if (pessoa1->pedidos != NULL) {
            aux1 = pessoa1->pedidos->inicio;
            while (aux1 != NULL) {
                aux2 = aux1;
                aux1 = aux1->prox;
                free(aux2); //libera os espaços de memória associados aos pedidos de parceria
            }
            free(pessoa1->pedidos); //libera a memória associada à fila
            pessoa1->pedidos = NULL;
        }
        if (pessoa1->parceiros != NULL) {
            aux1 = pessoa1->parceiros;
            while (aux1 != NULL) {
                aux2 = aux1;
                aux1 = aux1->prox;
                free(aux2); //libera os espaços de memória associados aos parceiros
            }
            pessoa1->parceiros = NULL;
        }
        pessoa2 = pessoa1;
        pessoa1 = pessoa1->prox;
        free(pessoa2); //libera os espaços de memória associados aos usuários
    }
    L->inicio = NULL;
}
