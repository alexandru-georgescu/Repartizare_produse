#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AVLTree.h"

/* Lungimea maxima a unui buffer */
#define BUFLEN 1024
#define MAXI 100
/* Lungimea unui element din arbore */
#define ELEMENT_TREE_LENGTH 3

/* Range of models*/
typedef struct Range{
	int *index; // Array of models in the range
	int size; // Number of array elements
	int capacity; // Array capacity
}Range;

void* createStrElement(void* str)
{
	char *c = malloc(3*sizeof(char));
	strncpy(c, str , 3);
		
	return c;
}

void destroyStrElement(void* elem)
{
	free((char*)elem);
}


void* createPriceElement(void* value)
{
   long *l = malloc(sizeof(long));
   *l = *((long*)(value));
   return value;
}

void destroyPriceElement(void* price)
{
	free((long*)price);
}

void* createIndexInfo(void* index)
{
	int *i = malloc(sizeof(int));
	*i = *((int*)(index));
	return i;

}

void destroyIndexInfo(void* index)
{
	free((int*)index);
}

int compareStr(void* str1, void* str2)
{
   if(*((char*)str1) < *((char*)str2))
   return -1;

   if(*((char*)str1) > *((char*)str2))
   return 1;
	
return 0;	
}

int comparePrice(void* price1, void* price2)
{
   if(*((long*)price1) < *((long*)price2))
   return -1;

   if(*((long*)price1) > *((long*)price2))
   return 1;
	
return 0;
}


void buildTreesFromFile(char* fileName, TTree* modelTree, TTree* priceTree)
{
   
   FILE* f = fopen(fileName , "rt");
   char v[MAXI] , p[MAXI] ;
   int n = 0 , i ;
   char str[MAXI]; 
   char *str2 ,*str3 , *price;
 
   long fprice;
 
   int d = 0;
 
   while(  feof(f) == 0 )
   {  

    n = (int)ftell(f);
    fgets(str , MAXI , f);
    
    str2 = strtok(str, ",");
    strncpy(str3 , str2 , 3); 

    price = strtok(NULL, ",");
    
    fprice = atol(price);

  //  printf("%s %ld\n" ,str3,fprice ); 
    insert(modelTree , str3 , &n );
    insert(priceTree , &fprice , &n );
  
   }
}

Range* modelGroupQuery(TTree* tree, char* q)
{
	// TODO: 3
}
Range* modelRangeQuery(TTree* tree, char* q, char* p)
{
	// TODO: 4b
	return NULL;
}
Range* priceRangeQuery(TTree* tree, long q, long p)
{
	// TODO: 4a
	return NULL;
}
Range* modelPriceRangeQuery(char* fileName, TTree* tree, char* m1, char* m2, long p1, long p2)
{
	// TODO: 5
	return NULL;
}


void printFile(char* fileName);
void inorderModelTreePrint(TTree* tree, TreeNode* node);
void inorderPriceTreePrint(TTree* tree, TreeNode* node);
void printRange(Range* range, char* fileName);


int main(void) 
{
	
	TTree* modelTree = createTree(createStrElement, destroyStrElement, createIndexInfo, destroyIndexInfo, compareStr);
	TTree* priceTree = createTree(createPriceElement, destroyPriceElement, createIndexInfo, destroyIndexInfo, comparePrice);

	buildTreesFromFile("input.csv", modelTree, priceTree);

	printf("Model Tree In Order:\n");
	inorderModelTreePrint(modelTree, modelTree->root);
	printf("\n\n");
        
	printf("Price Tree In Order:\n");
	inorderPriceTreePrint(priceTree, priceTree->root);
	printf("\n\n");
        printf("daaaaaa\n");
	printf("Group Model Search:\n");
	Range *range = modelGroupQuery(modelTree,"MG3");
	printRange(range,"input.csv");
	printf("\n\n");

	printf("Price Range Search:\n");
	Range *range2 = priceRangeQuery(priceTree,100,400);
	printRange(range2,"input.csv");
	printf("\n\n");

	printf("Model Range Search:\n");
	Range *range3 = modelRangeQuery(modelTree,"L2","M");
	printRange(range3,"input.csv");
	printf("\n\n");

	printf("Model Price Range Search:\n");
	Range *range4 = modelPriceRangeQuery("input.csv",modelTree,"L2","M", 300, 600);
	printRange(range4,"input.csv");

	free(range->index);
	free(range);
	free(range2->index);
	free(range2);
	free(range3->index);
	free(range3);
	free(range4->index);
	free(range4);
	destroyTree(priceTree);
	destroyTree(modelTree);
	return 0;
}



void inorderModelTreePrint(TTree* tree, TreeNode* node){
	if ( tree == NULL ) return;
	if(node != tree->nil){
		inorderModelTreePrint(tree, node->lt);
		TreeNode* begin = node;
		TreeNode* end = node->end->next;
		while(begin != end){
			printf("%d:%s  ",*((int*)begin->info),(char*)begin->elem);
			begin = begin->next;
		}
		inorderModelTreePrint(tree, node->rt);
	}
}

void inorderPriceTreePrint(TTree* tree, TreeNode* node){
	if ( tree == NULL ) return;
	if(node != tree->nil){
		inorderPriceTreePrint(tree, node->lt);
		TreeNode* begin = node;
		TreeNode* end = node->end->next;
		while(begin != end){
			printf("%d:%ld  ",*((int*)begin->info),*((long*)begin->elem));
			begin = begin->next;
		}
		inorderPriceTreePrint(tree, node->rt);
	}
}

void printRange(Range* range, char* fileName)
{
	if(fileName == NULL) return;
	FILE * file = fopen(fileName,"r");
	if (file == NULL) return;

	char *buf = (char*) malloc(BUFLEN+1);

	for(int i = 0; i < range->size;i++){
		fseek(file,range->index[i],SEEK_SET);
		if(fgets(buf,BUFLEN,file) != NULL){
			char* model = strtok(buf,",");
			long price = atol(strtok(NULL,","));
			printf("%s:%ld  ", model, price);
		}
	}
	printf("\n");
	free(buf);
	fclose(file);
}

void printFile(char* fileName)
{
	printf("---------\n");
	if(fileName == NULL) return;
	FILE * file = fopen(fileName,"r");
	if (file == NULL)
		return;

	char *buf = (char*) malloc(BUFLEN+1);
	while(fgets(buf,BUFLEN,file) != NULL){
		buf[strlen(buf) - 1] = '\0';
		printf("%s",buf);
	}
	printf("\n");
	printf("---------\n");
	free(buf);
	fclose(file);
}
