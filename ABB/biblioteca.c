/*
 * biblioteca.c
 *
 * Implementação das funções da biblioteca usando Árvore Binária de Busca.
 * Este arquivo contém todas as funções declaradas em biblioteca.h.
 */

#include "biblioteca.h"

/*
 * Cria uma nova biblioteca vazia.
 * Aloca memória para a estrutura e inicializa a raiz como NULL.
 */
Biblioteca *criarBiblioteca()
{
  Biblioteca *bib = (Biblioteca *)malloc(sizeof(Biblioteca));
  if (bib != NULL)
  {
    bib->raiz = NULL;
  }
  return bib;
}

/*
 * Função auxiliar para destruir a árvore recursivamente.
 * Percorre a árvore em pós-ordem, liberando a memória de cada nó.
 */
void destruirArvore(Livro *raiz)
{
  if (raiz != NULL)
  {
    destruirArvore(raiz->esq);
    destruirArvore(raiz->dir);
    free(raiz);
  }
}

/*
 * Libera toda a memória alocada para a biblioteca.
 * Primeiro destrói a árvore de livros e depois libera a estrutura da biblioteca.
 */
void destruirBiblioteca(Biblioteca *bib)
{
  if (bib != NULL)
  {
    destruirArvore(bib->raiz);
    free(bib);
  }
}

/*
 * Cria um novo nó de livro.
 * Aloca memória e inicializa todos os campos do livro.
 */
Livro *criarLivro(int id, const char *titulo, const char *autor)
{
  Livro *novo = (Livro *)malloc(sizeof(Livro));
  if (novo != NULL)
  {
    novo->id = id;
    strcpy(novo->titulo, titulo);
    strcpy(novo->autor, autor);
    novo->disponivel = 1;
    novo->esq = novo->dir = NULL;
  }
  return novo;
}

/*
 * Função auxiliar para inserir um livro na árvore.
 * Insere o livro mantendo a propriedade da ABB: IDs menores à esquerda,
 * maiores à direita.
 */
void inserirLivroRecursivo(Livro **raiz, int id, const char *titulo, const char *autor)
{
  if (*raiz == NULL)
  {
    *raiz = criarLivro(id, titulo, autor);
  }
  else if (id < (*raiz)->id)
  {
    inserirLivroRecursivo(&((*raiz)->esq), id, titulo, autor);
  }
  else if (id > (*raiz)->id)
  {
    inserirLivroRecursivo(&((*raiz)->dir), id, titulo, autor);
  }
}

/*
 * Insere um novo livro na biblioteca.
 * Chama a função recursiva para inserir na posição correta.
 */
void inserirLivro(Biblioteca *bib, int id, const char *titulo, const char *autor)
{
  inserirLivroRecursivo(&(bib->raiz), id, titulo, autor);
}

/*
 * Função auxiliar para buscar um livro na árvore.
 * Busca o livro pelo ID, aproveitando a propriedade da ABB.
 */
Livro *buscarLivroRecursivo(Livro *raiz, int id)
{
  if (raiz == NULL || raiz->id == id)
  {
    return raiz;
  }
  if (id < raiz->id)
  {
    return buscarLivroRecursivo(raiz->esq, id);
  }
  return buscarLivroRecursivo(raiz->dir, id);
}

/*
 * Busca um livro na biblioteca pelo ID.
 * Chama a função recursiva para buscar na árvore.
 */
Livro *buscarLivro(Biblioteca *bib, int id)
{
  return buscarLivroRecursivo(bib->raiz, id);
}

/*
 * Encontra o nó com o menor ID em uma subárvore.
 * Usado na remoção de nós com dois filhos.
 */
Livro *encontrarMenor(Livro *raiz)
{
  Livro *atual = raiz;
  while (atual && atual->esq != NULL)
  {
    atual = atual->esq;
  }
  return atual;
}

/*
 * Função auxiliar para remover um livro da árvore.
 * Remove o livro mantendo a propriedade da ABB.
 * Trata três casos: nó sem filhos, com um filho e com dois filhos.
 */
Livro *removerLivroRecursivo(Livro *raiz, int id)
{
  if (raiz == NULL)
    return raiz;

  if (id < raiz->id)
  {
    raiz->esq = removerLivroRecursivo(raiz->esq, id);
  }
  else if (id > raiz->id)
  {
    raiz->dir = removerLivroRecursivo(raiz->dir, id);
  }
  else
  {
    if (raiz->esq == NULL)
    {
      Livro *temp = raiz->dir;
      free(raiz);
      return temp;
    }
    else if (raiz->dir == NULL)
    {
      Livro *temp = raiz->esq;
      free(raiz);
      return temp;
    }

    Livro *temp = encontrarMenor(raiz->dir);
    raiz->id = temp->id;
    strcpy(raiz->titulo, temp->titulo);
    strcpy(raiz->autor, temp->autor);
    raiz->disponivel = temp->disponivel;
    raiz->dir = removerLivroRecursivo(raiz->dir, temp->id);
  }
  return raiz;
}

/*
 * Remove um livro da biblioteca pelo ID.
 * Chama a função recursiva para remover da árvore.
 */
void removerLivro(Biblioteca *bib, int id)
{
  bib->raiz = removerLivroRecursivo(bib->raiz, id);
}

/*
 * Função auxiliar para listar os livros em ordem.
 * Percorre a árvore em ordem (esquerda, raiz, direita).
 */
void listarLivrosRecursivo(Livro *raiz)
{
  if (raiz != NULL)
  {
    listarLivrosRecursivo(raiz->esq);
    printf("ID: %d\n", raiz->id);
    printf("Título: %s\n", raiz->titulo);
    printf("Autor: %s\n", raiz->autor);
    printf("Disponível: %s\n", raiz->disponivel ? "Sim" : "Não");
    printf("------------------------\n");
    listarLivrosRecursivo(raiz->dir);
  }
}

/*
 * Lista todos os livros da biblioteca em ordem.
 * Verifica se a biblioteca está vazia e chama a função recursiva.
 */
void listarLivros(Livro *raiz)
{
  if (raiz == NULL)
  {
    printf("Biblioteca vazia!\n");
    return;
  }
  listarLivrosRecursivo(raiz);
}

/*
 * Marca um livro como emprestado.
 * Busca o livro e verifica se está disponível antes de emprestar.
 */
void emprestarLivro(Biblioteca *bib, int id)
{
  Livro *livro = buscarLivro(bib, id);
  if (livro != NULL)
  {
    if (livro->disponivel)
    {
      livro->disponivel = 0;
      printf("Livro emprestado com sucesso!\n");
    }
    else
    {
      printf("Livro não está disponível para empréstimo.\n");
    }
  }
  else
  {
    printf("Livro não encontrado.\n");
  }
}

/*
 * Marca um livro como devolvido.
 * Busca o livro e verifica se está emprestado antes de devolver.
 */
void devolverLivro(Biblioteca *bib, int id)
{
  Livro *livro = buscarLivro(bib, id);
  if (livro != NULL)
  {
    if (!livro->disponivel)
    {
      livro->disponivel = 1;
      printf("Livro devolvido com sucesso!\n");
    }
    else
    {
      printf("Livro já está disponível.\n");
    }
  }
  else
  {
    printf("Livro não encontrado.\n");
  }
}

/*
 * Função auxiliar para armazenar os livros em um vetor em ordem.
 * Usada para rebalancear a árvore antes de salvar.
 *
 * Parâmetros:
 * - raiz: ponteiro para a raiz da árvore
 * - vetor: vetor que vai armazenar os livros em ordem
 * - pos: ponteiro para a posição atual no vetor
 *
 * Como funciona:
 * 1. Percorre a árvore em ordem (esquerda -> raiz -> direita)
 * 2. Coloca cada livro no vetor na ordem correta
 * 3. Incrementa a posição no vetor
 */
void armazenarLivrosEmOrdem(Livro *raiz, Livro **vetor, int *pos)
{
  if (raiz != NULL)
  {
    // Primeiro percorre a subárvore esquerda
    armazenarLivrosEmOrdem(raiz->esq, vetor, pos);

    // Depois coloca o nó atual no vetor
    vetor[(*pos)++] = raiz;

    // Por fim percorre a subárvore direita
    armazenarLivrosEmOrdem(raiz->dir, vetor, pos);
  }
}

/*
 * Função auxiliar para salvar os livros de forma balanceada.
 * Usa divisão e conquista para garantir árvore balanceada.
 *
 * Parâmetros:
 * - vetor: vetor com os livros em ordem
 * - inicio: índice inicial do intervalo
 * - fim: índice final do intervalo
 * - arquivo: arquivo onde os livros serão salvos
 *
 * Como funciona:
 * 1. Pega o elemento do meio do intervalo
 * 2. Salva esse elemento (será a raiz da subárvore)
 * 3. Recursivamente salva a subárvore esquerda
 * 4. Recursivamente salva a subárvore direita
 */
void salvarBalanceadoRecursivo(Livro **vetor, int inicio, int fim, FILE *arquivo)
{
  if (inicio <= fim)
  {
    // Calcula o meio do intervalo
    int meio = (inicio + fim) / 2;

    // Pega o livro do meio
    Livro *livro = vetor[meio];

    // Salva o livro no arquivo
    fprintf(arquivo, "%d|%s|%s|%d\n", livro->id, livro->titulo, livro->autor, livro->disponivel);

    // Salva a subárvore esquerda (elementos menores que o meio)
    salvarBalanceadoRecursivo(vetor, inicio, meio - 1, arquivo);

    // Salva a subárvore direita (elementos maiores que o meio)
    salvarBalanceadoRecursivo(vetor, meio + 1, fim, arquivo);
  }
}

/*
 * Função principal para salvar os livros de forma balanceada.
 * Primeiro armazena em um vetor em ordem, depois salva balanceado.
 *
 * Parâmetros:
 * - raiz: ponteiro para a raiz da árvore
 * - arquivo: arquivo onde os livros serão salvos
 *
 * Como funciona:
 * 1. Conta quantos livros existem na árvore
 * 2. Cria um vetor do tamanho exato
 * 3. Coloca todos os livros no vetor em ordem
 * 4. Salva os livros de forma balanceada
 * 5. Libera a memória do vetor
 *
 * Exemplo prático com 7 livros (IDs 1 a 7):
 * 1. Vetor em ordem: [1, 2, 3, 4, 5, 6, 7]
 * 2. Árvore balanceada resultante:
 *          4
 *         / \
 *        2   6
 *       / \ / \
 *      1  3 5  7
 */
void salvarLivrosBalanceado(Livro *raiz, FILE *arquivo)
{
  // Conta quantos livros existem na árvore
  int n = contarLivros(raiz);

  // Aloca memória para o vetor
  Livro **vetor = (Livro **)malloc(n * sizeof(Livro *));
  if (vetor == NULL)
    return;

  // Preenche o vetor com os livros em ordem
  int pos = 0;
  armazenarLivrosEmOrdem(raiz, vetor, &pos);

  // Salva os livros de forma balanceada
  salvarBalanceadoRecursivo(vetor, 0, n - 1, arquivo);

  // Libera a memória do vetor
  free(vetor);
}

/*
 * Função que salva os livros no arquivo.
 * Usa salvamento balanceado para manter a árvore balanceada.
 */
void salvarLivros(Livro *raiz, FILE *arquivo)
{
  salvarLivrosBalanceado(raiz, arquivo);
}

/*
 * Carrega livros de um arquivo para a biblioteca.
 * Lê o arquivo linha por linha, criando um novo livro para cada linha.
 */
void carregarLivros(Biblioteca *bib, const char *nomeArquivo)
{
  FILE *arquivo = fopen(nomeArquivo, "r");
  if (arquivo == NULL)
  {
    printf("Erro ao abrir arquivo para leitura.\n");
    return;
  }

  int id, disponivel;
  char titulo[MAX_TITULO], autor[MAX_AUTOR];
  char linha[256];

  while (fgets(linha, sizeof(linha), arquivo))
  {
    sscanf(linha, "%d|%[^|]|%[^|]|%d", &id, titulo, autor, &disponivel);
    inserirLivro(bib, id, titulo, autor);
    Livro *livro = buscarLivro(bib, id);
    if (livro != NULL)
    {
      livro->disponivel = disponivel;
    }
  }

  fclose(arquivo);
  printf("Livros carregados com sucesso!\n");
}

/*
 * Conta o número total de livros na biblioteca.
 * Percorre a árvore contando todos os nós.
 */
int contarLivros(Livro *raiz)
{
  if (raiz == NULL)
    return 0;
  return 1 + contarLivros(raiz->esq) + contarLivros(raiz->dir);
}

/*
 * Imprime os livros por níveis da árvore.
 * Usa uma fila para imprimir nível por nível, da raiz até as folhas.
 */
void imprimirPorNiveis(Livro *raiz)
{
  if (raiz == NULL)
    return;

  Livro **fila = (Livro **)malloc(1000 * sizeof(Livro *));
  int inicio = 0, fim = 0;

  fila[fim++] = raiz;

  while (inicio < fim)
  {
    Livro *atual = fila[inicio++];
    printf("ID: %d, Título: %s\n", atual->id, atual->titulo);

    if (atual->esq != NULL)
      fila[fim++] = atual->esq;
    if (atual->dir != NULL)
      fila[fim++] = atual->dir;
  }

  free(fila);
}