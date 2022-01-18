typedef struct bloco {
    char apelido_parceiro[10];
    struct bloco* prox;
} Bloco;

typedef struct conjunto {
    char remetente[10];
    char mensagem[100];
    struct conjunto* prox;
} Conjunto;

typedef struct {
    Bloco *inicio, *fim;
} Fila;

typedef struct {
    Conjunto* topo;
} Pilha;

typedef struct cadastro {
    char nome[50];
    char apelido[10];
    struct cadastro* prox;
    Fila* pedidos;
    Bloco* parceiros;
    Pilha* mensagens;
} Cadastro;

typedef struct {
    Cadastro* inicio;
} Lista;

/*typedef struct {
    char[10] apelido;
    int ponto = 0;
}*/

Lista* Criar_Lista(void);
Fila* Criar_Fila(void);
Pilha* Criar_Pilha(void);
Cadastro* Registro (char*, char*);
void Listar(Lista*);
void Parceria(Lista*, char*, char*);
void Avaliar(Lista*, char*);
void Enviar(Lista*, char*, char*, char*);
void Ler(Lista*, char*);
void Sugerir(Lista*, char*);
void Reiniciar(Lista*);
