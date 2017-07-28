/*

	Aluno: Vitor Palma Aderaldo

	Algoritmo Genetico para distribuir 8 rainhas em um tabuleiro 8x8
	de forma que essas rainhas nao consigam se atacar.

*/

#include <stdio.h>
#include <stdlib.h>

int *criarIndividuo(int n){

	//Aloca espaco o individuo
	int *individuo = (int *)malloc(n*sizeof(int));

	int aleatorio,aux;

	//Inicia-se o indiviuo com os valores {0,1,2,3,4,5,6,7}
	//Isso garante que as rainhas fiquem em linhas e coluna diferentes
	//O indice representa a linha e o valor representa a coluna.
	for(int i=0;i<n;i++){
		individuo[i] = i;
	}

	//Realizo uma permutacao para gerar uma sequencia diferente
	//Essa permutacao mantem a propriedade de que rainhas nao estejam na mesma linha ou coluna.
	for(int i=0;i<n;i++){

		for(int j=0;j<n;j++){

			aleatorio = rand() % 10;

			if(aleatorio>=5){
				aux = individuo[j];
				individuo[j] = individuo[i];
				individuo[i] = aux;
			}
		}
	}

	return individuo;
}

//Dado uma rainha, a funcao retorna quantas outras rainhas ela pode atacar
//Só é necessário percorrer as diagonais devido a propriedade de criacao dos individuos
int acharVizinho(int matriz[8][8], int i, int j){

	int a,b,c;

	// s = numero de vizinhos
	int s = 0;

	//Percorrer diagonal da Direita para baixo
	a = i + 1;
	b = j + 1;
	while(a<8 && b<8){
		s = s + matriz[a][b];
		a++;
		b++;
	}

	//Percorrer diagonal da Direita para cima
	a = i - 1;
	b = j + 1;
	while(a>=0 && b<8){
		s = s + matriz[a][b];
		a--;
		b++;
	}

	//Percorrer diagonal da Esquerda para cima
	a = i - 1;
	b = j - 1;
	while(a>=0 && b>=0){
		s = s + matriz[a][b];
		a--;
		b--;
	}

	//Percorrer diagonal da Esquerda para baixo
	a = i + 1;
	b = j - 1;
	while(a<8 && b>=0){
		s = s + matriz[a][b];
		a++;
		b--;
	}

	//printf("\nTem %d vizinhos",s);

	return s;
}

//Cria um conjunto de individuos
int **criarPopulacao(int n){

	int **populacao = (int **)malloc(n*sizeof(int *));

	for(int i=0;i<n;i++){
		populacao[i] = criarIndividuo(8);
	} 

	return populacao;
}

void exibirIndividuo(int *ind, int n){

	printf("\n");
	for(int i=0;i<n;i++){
		printf("%d ",ind[i]);
	}
}

int fitness(int *individuo){

	//A partir de um individuo é criado uma matriz para representar o tabuleiro
	int matriz[8][8];

	//Variaveis para contar o numero de ataques que podem ocorrer nesse individuo
	int soma = 0;
	int aux = 0;

	//Matriz
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			matriz[i][j]=0;
		}
	}

	for(int i=0;i<8;i++){
		matriz[i][individuo[i]] = 1;
	}

	//Depois de criar a matriz, iremos percorre-la
	//Para cada rainha encontrada iremos analisar seus ataques(vizinhos)
	//Obs: O vizinho nao esta necessariamente ao lado.
	int i,j;

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){

			if(matriz[i][j]==1){
				//Ao encontrar uma rainha iremos ver quantas peças podemos atacar
				soma = soma + acharVizinho(matriz,i,j);
			}
		}
	}

	//Retorna o numero de ataques possiveis de se fazer nesse tabuleiro
	return soma;	
}

void exibirPopulacao(int ** populacao, int n, int j){

	for(int i=0;i<n;i++){
		exibirIndividuo(populacao[i],j);
		printf("(%d)",fitness(populacao[i]));
		
	}
	printf("\n");
}

//Retorna o indice do melhor individuo da populacao
int getMelhor(int *qualidade, int n){

	int menor = qualidade[0];
	int indice = 0;

	for(int k=1; k<n; k++){

		if(menor > qualidade[k]){
			indice = k;
			menor = qualidade[k];
		}

	}

	//printf("\nO melhor é: %d com indice %d",menor,indice);
	return indice;
}


//O processo de mutação é uma permutacao dos alelos do individuo
int mutacao(int *individuo, int j){

	//Variaveis aleatoria geradas de 0 a 7
	int r1 = rand() % j;
	int r2 = rand() % j;

	int k;

	//Antes de realizar a mutacao iremos criar um individuo para ver se a mutacao seria boa
	int teste[8];

	//Cria o individuo teste
	for(int k=0;k<8;k++){
		teste[k] = individuo[k];
	}

	teste[r1] = individuo[r2];
	teste[r2] = individuo[r1];

	//Caso o individuo teste (que recebeu a permutacao) seja melhor do que o original:
	if(fitness(teste) < fitness(individuo)){
		
		//Realiza a permutacao no individuo
		k = individuo[r2];
		individuo[r2] = individuo[r1];
		individuo[r1] = k;

		//Retorna 1 pois ocorreu a mutacao
		return 1;

		//Como nao ocorreu a mutacao, retorna 0, pois n compensa fazer mutacao
	}else return 0;
}

int main(){

	/*	j = Tamanho do tabuleiro (se j=8 => tabuleiro: 8x8)
		n = Quantidade de individuos na populacao

		populacao: Uma matriz que cada linha representa um individuo
		qualidade: Um vetor que contem o fitness de cada individuo

		erro: variavel para guardar o featness do melhor individuo a cada geracao
	*/

	int opcao;
	printf("\nO que deseja exibir ?\n 1 - Somente a resposta\n 2 - Geracoes e resposta\n");
	printf(" -> ");
	scanf("%d",&opcao);


	int j = 8;
	int n = 10;

	int k;

	//Criar populacao com n elementos
	int **populacao = criarPopulacao(n);

	//Cria vetor auxiliar para armazenar os fitness
	int *qualidade = (int *) malloc(n*sizeof(int));

	//Atribui os respectivos valores de fitness para cada individuo
	for(k=0;k<n;k++){
		qualidade[k] = fitness(populacao[k]);
	}

	//Guarda o fitness do melhor individuo
	int erro = fitness(populacao[getMelhor(qualidade,n)]);

	//Contar geracoes
	int geracao = 1;

	k = 0;

	while(erro > 0){

		//Mostrar polucao
		if(opcao==2){
			printf("\nGeracao (%d) ",geracao);
			exibirPopulacao(populacao,n,j);
		}

		//Selecao: Todos individuos são utilizados

		//CrossOver: Nao ha necessidade, perderia a caracterista de uma rainha por linha e por coluna.


		//Mutacao
		if(mutacao(populacao[k],j)==1){

			//Se a mutacao fornecer um valor melhor entao ela substituira o individuo original
			//A funcao de mutacao nao alterou o vetor qualidade, entao precisamos mudar
			qualidade[k] = fitness(populacao[k]);

			//Calcula o novo erro em cima da mutacao
			erro = qualidade[k];
		}

		geracao++;			
		k++;
		if(k==8) k=0;		
	}

	//Exibe populacao que contem a resposta.
	if(opcao==2){
		printf("\nGeracao (%d) ",geracao);
		exibirPopulacao(populacao,n,j);
	}

	//Exibir o tabuleiro da solucao:
	int *melhor = populacao[getMelhor(qualidade,n)];

	int matriz[8][8];

	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			matriz[i][j]=0;
		}
	}

	for(int i=0;i<8;i++){
		matriz[i][melhor[i]] = 1;
	}

	printf("\nSolucao:\n\n");
	printf("     0   1   2   3   4   5   6   7");
	printf("\n    --- --- --- --- --- --- --- --- \n");
	for(int i=0;i<8;i++){
		printf(" %d | ",i);
		for(int j=0;j<8;j++){
			if(matriz[i][j]==1) printf("X | ");
			else printf("  | ");
		}

		printf("\n    --- --- --- --- --- --- --- --- \n");
	}

	return 1;
}