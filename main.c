#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "structs.h"

void print_elem (void *arr, int n);
int length (void *arr, int len);
data_structure* stock(char *p);

/* adauga un element la finalul "vectorului generic" arr */
int add_last (void **arr, int *len, data_structure *data)
{
	int poz = length(*arr, *len); // lungimea totala a vectorului generic

	*arr = realloc(*arr, poz + sizeof(head) + data->header->len);
	if(*arr == NULL) return 1;   // nu s-a putut aloca memorie

	/* adauga la vector header-ul si datele elementului de adaugat */
	memcpy(*arr+poz, data->header, sizeof(head));
	memcpy(*arr+poz+sizeof(head), data->data, data->header->len);
	
	(*len)++; // lungimea "vectorului generic" creste cu un element
	return 0;
}

/* adauga un element la indexul 'index' in "vectorului generic" arr */
int add_at (void **arr, int *len, data_structure *data, int index)
{
	*arr = realloc(*arr, length(*arr,*len) + 8 + data->header->len);
	if(*arr==NULL || index < 0) return 1;

	/* daca indexul e mai mare decat lung. vectorului, se va adauga la final */
	if(index > *len) index = *len;
	
	int i, poz=0; // poz va retine pozitia in care se va adauga noul element
	/* gasire pozitia in care va fi adaugat elementul */
	for(i=0; i<index; i++) {
		head header = *((head *)(*arr + poz));
		poz = poz + sizeof(head) + header.len;
	}

	/* deplasarea datelor pentru inserarea elementului */
	int l_move = length(*arr,*len) - poz;      // lungimea datelor de deplasare 
	int l_elem = sizeof(head) + data->header->len;      // lungimea elementului
	if(l_move != 0) {
		void *temp = malloc(l_move);
		memcpy(temp, *arr+poz, l_move);
		memcpy(*arr+poz+l_elem, temp, l_move);
		free(temp);
	}

	/* adauga la vector header-ul si datele elementului de adaugat */
	memcpy(*arr+poz, data->header, sizeof(head));
	memcpy(*arr+poz+sizeof(head), data->data, data->header->len);
	
	(*len)++;  // lungimea "vectorului generic" creste cu un element
	return 0;
}

/* printeaza elementul de la indexul respectiv */
void find (void *data_block, int len, int index)
{
	if(index<0 && index>len-1) return;
	
	int i, poz=0;  // poz va retine pozitia in arr a elementului cerut
	/* parcurge vectorul generic pana la elementul de la indexul respectiv */
	for(i=0; i<index; i++) {
		head header = *(head *)((unsigned char *)(data_block+poz));
		poz = poz + sizeof(head) + header.len;
	}

	print_elem(data_block, poz);	 /// printare element
}

/* sterge un element de la indexul 'index' din "vectorul generic" arr */
int delete_at (void **arr, int *len, int index)
{
	int i, poz=0;              // poz va retine pozitia elementului de sters
	int l_elem;                // lungimea elementului de sters
	int l = length(*arr,*len); // lungimea vectorului generic  
	for(i=0; i<=index; i++) {
		head header = *((head *)(*arr+poz));
		poz = poz + sizeof(head) + header.len;
		l_elem = sizeof(head) + header.len;
	}
	int l_move = l - poz; // lungimea datelor de deplasare 
	poz = poz - l_elem;
	
	/* deplasarea datelor pentru stergerea elementului */
	if(l_move!=0) {
		void *temp = malloc(l_move);
		memcpy(temp, *arr+poz+l_elem, l_move);
		memcpy(*arr+poz, temp, l_move);
		free(temp);
	}

	l = l - l_elem;
	*arr = realloc(*arr, l);

	(*len)--;
	return 0;
}

/* printeaza datele din interiorul vectorului generic dupa format */
void print(void *arr, int len)
{
	int i, poz=0; // poz are valoarea pozitiei de unde este memorat un element
	for(i=0; i<len; i++) {	
		head header = *((head *)((unsigned char *)arr + poz));
		print_elem(arr, poz);  /// printare element
		poz = poz + sizeof(head) + header.len;
	}
}

int main() {

	void *arr = NULL;
	int len = 0;
	while(1) {
		/* citirea unei comenzi si eliminarea '\n' */
		char *command = (char *) malloc(258);
		fgets(command,258,stdin);
		command[strlen(command)-1]='\0';

		/* executarea comenzilor */
		/* exit */
		if(strcmp(command, "exit") == 0) {
			free(command);
			if(arr != NULL) free(arr);
			return 0;
		}
		
		char *p = strtok(command," ");

		/* insert = adaugare element la final */
		if(strcmp(p, "insert") == 0) {
			data_structure *data = stock(p);
			add_last(&arr,&len,data);
			free(data->header);
			free(data->data);
			free(data);
		}
		
		/* insert_at = adaugare element la un anumit index */
		if(strcmp(p, "insert_at") == 0) {
			p=strtok(NULL," ");
			int index=atoi(p);
			
			data_structure *data = stock(p);
			add_at(&arr,&len,data,index);
			free(data->header);
			free(data->data);
			free(data);
		}

		/* delete_at = stergere un element de la un anumit index */
		if(strcmp(p, "delete_at") == 0) {
			p = strtok(NULL," ");
			int index = atoi(p);
			delete_at(&arr,&len,index);
		}
		
		/* find_at = printare element de la un anumit index*/
		if(strcmp(p, "find") == 0) {
			p = strtok(NULL," ");         
			int index = atoi(p);
			find(arr,len,index);
		}

		/* print = printarea tuturor elementelor */
		if(strcmp(p, "print") == 0)
			print(arr,len);		
		
		free(command);
	}

	return 0;
}

/* returneaza lungimea reprezentata in bytes a "vectorului generic" arr */
int length(void *arr, int len)
{
	int i, l=0;  /// lungimea vectorului
	for(i=0; i<len; i++) {
		head header = *((head *)((unsigned char *)arr+l));
		l = l + sizeof(head) + header.len;
	}

	return l;
}

/* creeaza un element de tip data_structure */
data_structure* stock(char *p)
{
	p=strtok(NULL," ");  // tipul de date stocate
	data_structure *data = malloc(sizeof(data_structure));
	data->header = calloc(1,sizeof(head));
	data->header->type = (unsigned int) *p;

	p=strtok(NULL," ");  // dedicator
	char *give_money = malloc(strlen(p)+1);
	strcpy(give_money, p);
	data->header->len = strlen(give_money) + 1;
	
	p=strtok(NULL," ");  // prima bancnota
	int sum1=atoi(p);

	p=strtok(NULL," ");  // a doua bancnota
	int sum2=atoi(p);
	if(data->header->type=='1') data->header->len += 2;
	if(data->header->type=='2') data->header->len += 6;
	if(data->header->type=='3') data->header->len += 8;

	p=strtok(NULL," ");  // dedicatul
	char *get_money = malloc(strlen(p)+1);
	strcpy(get_money, p);
	data->header->len += strlen(get_money) + 1;
	
	/* copierea datelor */
	data->data= calloc(data->header->len, 1);
	memcpy(data->data, give_money, strlen(give_money));
	int poz = strlen(give_money)+1;
	
	if(data->header->type=='1'){
		*(int8_t *)((unsigned char *)data->data+poz) = sum1;
		*(int8_t *)((unsigned char *)data->data+poz+1) = sum2;
		poz += 2;
	}
	if(data->header->type=='2'){
		*(int16_t *)((unsigned char *)data->data+poz) = sum1;
		*(int32_t *)((unsigned char *)data->data+poz+2) = sum2;
		poz += 6;
	}
	if(data->header->type=='3'){
		*(int32_t *)((unsigned char *)data->data+poz) = sum1;
		*(int32_t *)((unsigned char *)data->data+poz+4) = sum2;
		poz += 8;
	}

	memcpy(data->data + poz, get_money, strlen(get_money));

	free(give_money);
	free(get_money);

	return data;
}

/* printeaza sumele in functie de tipul elementului */
void print_sums(unsigned char type, void *sum1, void *sum2)
{
	if(type == '1'){
		printf("%"PRId8"\n", (int8_t)*(int8_t *)sum1);
		printf("%"PRId8"\n", (int8_t)*(int8_t *)sum2);  
	} 
	if(type == '2'){
		printf("%"PRId16"\n", (int16_t)*(int16_t *)sum1);
		printf("%"PRId32"\n", (int32_t)*(int32_t *)sum2);  
	} 
	if(type == '3'){
		printf("%"PRId32"\n", (int32_t)*(int32_t *)sum1);
		printf("%"PRId32"\n", (int32_t)*(int32_t *)sum2);  
	}
	printf("\n");
	free(sum1);
	free(sum2);
}

/* printeaza un element aflat la pozitia n in vectorul generic */
void print_elem(void *arr, int n)
{
	int poz=0;  /// pozitia curenta in element
	head header = *(head *)((unsigned char *)arr+n);
	poz = sizeof(head);
	
	char *give_money = strtok(arr+n+poz,""); // dedicator
	poz += strlen(give_money)+1;
	
	char *get_money; // dedicatul
	if(header.type == '1') get_money = strtok(arr+n+poz+2,"");  
	if(header.type == '2') get_money = strtok(arr+n+poz+6,"");  
	if(header.type == '3') get_money = strtok(arr+n+poz+8,"");  

	printf("Tipul %c\n", header.type);
	printf("%s pentru %s\n", give_money, get_money);

	// bancnotele
	if(header.type == '1') {
		void *sum1 = malloc(1);
		void *sum2 = malloc(1);
		memcpy(sum1, (unsigned char *)arr+n+poz, 1);
		memcpy(sum2, (unsigned char *)arr+n+poz+1, 1);
		print_sums(header.type, sum1, sum2);
	}
	if(header.type == '2') {
		void *sum1 = malloc(2);
		void *sum2 = malloc(4);
		memcpy(sum1, (unsigned char *)arr+n+poz, 2);
		memcpy(sum2, (unsigned char *)arr+n+poz+2, 4);
		print_sums(header.type, sum1, sum2);
	}
	if(header.type == '3') {
		void *sum1 = malloc(4);
		void *sum2 = malloc(4);
		memcpy(sum1, (unsigned char *)arr+n+poz, 4);
		memcpy(sum2, (unsigned char *)arr+n+poz+4, 4);
		print_sums(header.type, sum1, sum2);
	}
}