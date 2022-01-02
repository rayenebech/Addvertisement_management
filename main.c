#include <stdio.h>
#include <stdlib.h>
#define SIZEOF(a) sizeof(a)/sizeof(*a)
#define SIZE 20

/*******************************************************************************
 A struct to keep information of an addvertisement
*******************************************************************************/
typedef struct{
	int index;
	int start;
	int duration;
	int value;
}addv;

/*******************************************************************************
The linked list will be used to save the addvertisements chosen at each moment
*******************************************************************************/
typedef struct Node{
	int index;
	struct Node* next;
}Node;

/*******************************************************************************
 A function that takes as an input txt file  (filename.txt) and reads from it information
 of addvertisements and returns them in an array (arr) of struct addv with the total 
 number of addvertisements in the file in the variable size. Returns -1 if the file
 does not exist
*******************************************************************************/
int readFile(char * filename, addv arr[], int* size );

/*******************************************************************************
 A function that computes the finish time of an addvertisement
*******************************************************************************/
int finishTime(addv add1);

/*******************************************************************************
 A function to create an addvertisement with start date s, duration of d and value of v
*******************************************************************************/
addv createAdd(int s, int d, int v);


/*******************************************************************************
 A function to print an array of addvertisements 
*******************************************************************************/
void printArr(addv arr[], int size);


/*******************************************************************************
 Functions to merge sort addvertisements according to their finish time
*******************************************************************************/
void sort(addv arr[], int start, int end );
void merge(addv arr[],int start, int mid, int end);


/*******************************************************************************
 A function to compute the maximum profit from an array of addvertisments and prints
 the addvertisements chosen
*******************************************************************************/
int F(addv adds[],int n);


/*******************************************************************************
 A function to find the last element that does not overlapp with i from the array arr
*******************************************************************************/
int lastavailable(int i,addv arr[]);

/*******************************************************************************
 A function to create a Node struct with the value i
*******************************************************************************/
Node* createNode(int i);

/*******************************************************************************
 A function to dispaly  addvertisement
*******************************************************************************/
void printNode(Node* p);

int main(int argc, char *argv[]) {
	int size;
	addv* adds= malloc(SIZE * sizeof(addv));

	/*Read the addvertisements from the file*/
	printf("******** Finding Maximum Profit using Dynamic Programming ********\n");
	printf("Reading information from the file: \n");
	readFile("Sample.txt", adds, &size );
	printArr(adds,size);
	/*Sort by finish time*/
	sort(adds,0,size-1);
	printf("\nAfter sortings: \n");
	printArr(adds,size);
	/*Compute Maximum Profit*/
	F(adds,size);
	free(adds);
	return 0;
}

int F(addv arr[],int n){
	if(n==0) return 0; /*No addvertisement? Profit 0*/
	if(n==1) return arr[0].value; /*Only one addvertisement? Profit = the value of that addvertisement*/
	int F[n]; /*Accumulate the profit chosen for each addvertisement*/
	Node* S[n]; /*Keeps the addvertisments chosen at the moment i*/
	int i,last; 
	F[0]=0; F[1]= arr[0].value; 
	S[0]= NULL; S[1]= createNode(arr[0].index);
	for(i=2;i<=n;i++){
		last= lastavailable(i-1,arr); /*gets the index of the last addvertisement that its 
										time does not overlapp with the i.th addvertisement*/
		if(F[i-1]>(arr[i-1].value+F[last+1])){ /*if the previous profit is higher than the new*/
			F[i]= F[i-1];
			S[i]=S[i-1];
		}
		else{
			F[i]= arr[i-1].value+F[last+1];
			S[i]=createNode(arr[i-1].index);
			S[i]->next= S[last+1];	
		}
	}
	printf("\nThe process of computing the profit: \n");
	for(i=0;i<=n;i++){
		printf("%d|",F[i]);
	}
	printf("\n\nMaximum Profit is: %d\n",F[i-1]);
	printf("\nThe addvertisements selected are: ");
	printNode(S[i-1]);
	return F[i-1];
}
Node* createNode(int i){
	Node* p = (Node*) malloc(sizeof(Node));
	p->index=i;
	p->next = NULL;
	return p;
}
void printNode(Node* p){
	while(p != NULL){
		printf("%d, ",p->index);
		p= p->next;
	}
}
int lastavailable(int i,addv arr[]){
	int j=i-1;
	while(j>=0){
		if(finishTime(arr[j])<= arr[i].start) return j;
		j--;
	}
	return j;
	
}


addv createAdd(int s, int d, int v){
	addv new_add;
	new_add.start = s;
	new_add.duration = d;
	new_add.value = v;
	return new_add;
}

int checkOverlapp(addv addv1, addv addv2){
	return addv2.start > finishTime(addv1)? 1 : 0;
}

void printArr(addv arr[], int size){
	int i;
	printf("\ni | s | d | v \n");
	for(i=0;i<size;i++){
		printf("%d | %d | %d | %d\n",arr[i].index,arr[i].start, arr[i].duration, arr[i].value);
	}	
}

void sort(addv arr[], int start, int end ){
	if (start<end){
		int mid= start + (end -start)/2;
		sort(arr, start, mid); /*split left array*/
		sort(arr, mid+1, end); /*split right array*/
		merge(arr,start, mid, end); /*merge the two arrays*/
	}
	
}

void merge(addv arr[],int start, int mid, int end){
	int i,j,k;
	int size1= mid -start +1; 
	int size2= end - mid;
	addv leftArr[size1], rightArr[size2];
	/*Copy both arrays*/
	for(i=0;i<size1;i++){
		leftArr[i]= arr[start+i];
	}
	for(j=0; j<size2; j++){
		rightArr[j]= arr[mid+j+1];
	}
	
	
	i=0;j=0;k=start;
	while(i<size1 && j< size2){
		if(finishTime(leftArr[i])< finishTime(rightArr[j])){
			arr[k]= leftArr[i];
			i++;
		}
		else{
			arr[k]= rightArr[j];
			j++;
		}
		k++;
	}
	while(i<size1){
		arr[k]=leftArr[i];
		k++;
		i++;
	}
	while(j<size2){
		arr[k]= rightArr[j];
		k++;
		j++;
	}
	return;
}
int finishTime(addv add1){
	return (add1.start+ add1.duration);
}

int readFile(char * filename, addv arr[], int* size ){
	FILE *fp;
	int i=0;
	char line[256];
	fp = fopen(filename, "r");
	if (fp == NULL){
		printf("The file does not exist");
		return -1;
	}
	while(fgets(line, sizeof(line),fp)){
		sscanf(line, "%d %d %d\n", &arr[i].start, &arr[i].duration,&arr[i].value);
		arr[i].index=i+1;
		i++;
	}
	*size=i;
	fclose(fp);
	return 0;
}

