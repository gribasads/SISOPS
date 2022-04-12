#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define a estrutura Task(tarefa)
typedef struct {
	char name[2];
	int start;
	int duration;
	int execution;
} Task;

void showTask( Task task ) {
	printf("Tarefa \n");
	printf("nome: %s", task.name);
	printf("| inicio: %d", task.start);
	printf("| duracao: %d", task.duration);
	printf("\n");
}

Task fillsTask( Task task, char name[2], int start, int duration ){
	strcpy( task.name, name );
	task.start = start;
	task.duration = duration;
	task.execution = duration;
	
	return task;
}

double roundRobin(Task task[], int numberOfTasks, int quantum){
	int i = 0;
	int j = 0;
	int tasksDuration = 0;
	
	printf("\nRound Robin:\n");
	
	// calculo do tempo
	clock_t time;
	time = clock();
	
	// percorre todas as tarefas em busca da sua duração total
	for( i = 0 ; i < numberOfTasks ; i++ ) {
		tasksDuration += task[i].duration;
		task[i].execution = task[i].duration;
	}
	
	do {
		for( i = 0 ; i < numberOfTasks ; i++ ) {			//	percorre as tarefas
			if( task[i].execution > 0 ) {					//	se a tarefa ainda precisar ser executada
				for( j = 0 ; j < quantum ; j ++ ) {			// 	executa parte da tarefa no tempo q (quantum)
					printf("%s ", task[i].name);			
					task[i].execution--;
					tasksDuration--;
					
					if( task[i].execution == 0 )  break; 	//	se não precisa mais ser executada pula para a próxima
				}
				printf(" -> \n");								//  simula o overhead
			}
		}
	} while(tasksDuration != 0);							// enquanto há tarefas para serem executadas
	
	clock_t final = clock();
	double processingTime = ((double)(final - time)) / (double)(CLOCKS_PER_SEC);
	
	printf("Tempo de processamento: %.4lf \n\n", processingTime);
	return processingTime;
}

// First-Come, First-Served
double FCFS(Task task[], int numberOfTasks) {
	int i = 0;
	
	
	printf("FCFS:\n");
	
	// calculo do tempo
	clock_t time;
	time = clock();
	
	for( i = 0 ; i < numberOfTasks ; i++ ) {				// percorre todas as tarefas
		task[i].execution = task[i].duration;
		
		do {
			printf("%s ", task[i].name);
			task[i].execution--;
		} while(task[i].execution > 0);						// enquanto há tarefas para serem executadas
		printf("-> \n");										// simula o overhead
	}
	
	clock_t final = clock();
	double processingTime = ((double)(final - time)) / (double)(CLOCKS_PER_SEC);
	
	printf("Tempo de processamento: %.4lf \n\n", processingTime);
	return processingTime;
}

int main() {
	FILE *arq;
	char info[100];
	int cont = 0;
	int i = 0;
	int numberOfTasks = 0;
	
	// atributos de uma tarefa
	char name[2];
	int start = 0;
	int duration = 0;
	
	// testes
	int repetitions = 1;
	double totalTime = 0;
	FILE *reg = fopen("registro.txt","a");
	
	// Abre um arquivo de texto para leitura
	arq = fopen("tarefas.txt", "rt");
	if( arq == NULL ) printf("Problemas na leitura do arquivo\n");
	
	// define o número de tarefas
	while(fscanf(arq, " %s ,", &info) != EOF){
		numberOfTasks = atoi(info);
		cont++;
		
		break;	
	}
	
	Task task[numberOfTasks];
	
	while(fscanf(arq, " %s ,", &info) != EOF) {
		switch(cont) {
			case 1:
				// define o nome da tarefa
				strcpy( name, info );
				strcpy( task[i].name, name );
				
				break;
			case 2:
				// define o início da tarefa
				start = atoi(info);
				task[i].start = start;

				break;
			case 3:
				// define a duração da tarafa
				duration = atoi(info);
				task[i].duration = duration;

				// preenche a tarefa
				task[i] = fillsTask(task[i], name, start, duration);
				
				cont = 0;
				i++;
				
				break;
			default:
				break;
		}
		
		cont++;
	}
	
	// Testes:
	
	for(i = 0 ; i < repetitions ; i++) {
		totalTime += FCFS(task, numberOfTasks);
	}
	printf("Tempo total FCFS: %.4lf", totalTime);
	fprintf(reg,"Tempo total FCFS: %.4lf\n", totalTime);
	
	totalTime = 0;
	
	for(i = 0 ; i < repetitions ; i++) {
		totalTime += roundRobin(task, numberOfTasks, 4);
	}
	printf("Tempo total Round Robin: %.4lf", totalTime);
	fprintf(reg,"Tempo total Round Robin: %.4lf\n\n", totalTime);
	
		
	fclose(arq);
	
	return 0;
}
