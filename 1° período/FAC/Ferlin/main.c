#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct livro {
  char autor[51];
  char titulo[31];
  int qtdVendida;
};

typedef struct noFila {
  struct livro *livro;
  struct noFila *prox;
} NoFila;

typedef struct filaDeLivros {
  NoFila *inicio;
  NoFila *fim;
} FilaDeLivros;

FilaDeLivros *fiCria(void) {
  FilaDeLivros *fi = (FilaDeLivros *)malloc(sizeof(FilaDeLivros));
  if (fi == NULL) {
    printf("Erro ao criar a fila de livros.\n");
    exit(1);
  }
  fi->inicio = NULL;
  fi->fim = NULL;
  return fi;
}

void fiInsere(FilaDeLivros *fi, struct livro *liv) {
  NoFila *novo = (NoFila *)malloc(sizeof(NoFila));
  if (novo == NULL) {
    printf("Erro ao alocar memória para o nó da fila.\n");
    exit(1);
  }
  novo->livro = liv;
  novo->prox = NULL;
  if (fi->inicio == NULL) {
    fi->inicio = novo;
  } else {
    fi->fim->prox = novo;
  }
  fi->fim = novo;
}

struct livro *fiRetira(FilaDeLivros *fi) {
  if (fi->inicio == NULL) {
    printf("Erro: a fila está vazia.\n");
    return NULL;
  }
  struct livro *liv = fi->inicio->livro;
  NoFila *temp = fi->inicio;
  fi->inicio = fi->inicio->prox;
  free(temp);
  if (fi->inicio == NULL) {
    fi->fim = NULL;
  }
  return liv;
}

int fiVazia(FilaDeLivros *fi) { return (fi->inicio == NULL); }

void fiLibera(FilaDeLivros *fi) {
  NoFila *aux;
  while (fi->inicio != NULL) {
    aux = fi->inicio;
    fi->inicio = fi->inicio->prox;
    free(aux);
  }
  free(fi);
}

void exibeOsXLivrosMaisVendidosDeUmAutor(FilaDeLivros *fila, char *autor,
                                         int X) {
  NoFila *inicio = fila->inicio;

  FilaDeLivros *filaAuxiliar = fiCria();

  while (inicio != NULL) {
    if (strcmp(inicio->livro->autor, autor) == 0) {
      fiInsere(filaAuxiliar, inicio->livro);
    }
    inicio = inicio->prox;
  }

  // Ordena a fila de livros auxiliar em ordem decrescente de quantidade vendida
  // Implementação do algoritmo de ordenação Bubble Sort
  int trocado;
  do {
    trocado = 0;
    NoFila *atual = filaAuxiliar->inicio;
    NoFila *anterior = NULL;
    while (atual != NULL && atual->prox != NULL) {
      if (atual->livro->qtdVendida < atual->prox->livro->qtdVendida) {
        struct livro *temp = atual->livro;
        atual->livro = atual->prox->livro;
        atual->prox->livro = temp;
        trocado = 1;
      }
      anterior = atual;
      atual = atual->prox;
    }
  } while (trocado);

  printf("Os %d livros mais vendidos do autor %s:\n", X, autor);
  for (int i = 0; i < X && !fiVazia(filaAuxiliar); i++) {
    struct livro *liv = fiRetira(filaAuxiliar);
    printf("%s (%d exemplares vendidos)\n", liv->titulo, liv->qtdVendida);
  }

  fiLibera(filaAuxiliar);
}

int main() {
  FilaDeLivros *fila = fiCria();

  struct livro livro1 = {"Autor1", "Livro1", 100};
  struct livro livro2 = {"Autor2", "Livro2", 200};
  struct livro livro3 = {"Autor1", "Livro3", 150};
  struct livro livro4 = {"Autor2", "Livro4", 250};
  struct livro livro5 = {"Autor1", "Livro5", 300};

  fiInsere(fila, &livro1);
  fiInsere(fila, &livro2);
  fiInsere(fila, &livro3);
  fiInsere(fila, &livro4);
  fiInsere(fila, &livro5);

  exibeOsXLivrosMaisVendidosDeUmAutor(fila, "Autor1", 3);

  fiLibera(fila);

  return 0;
}
