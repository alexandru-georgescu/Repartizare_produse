#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <stdlib.h>

#define MAX(a, b) (((a) >= (b))?(a):(b))

typedef struct node
{
	void* elem;
	void* info;
	struct node *pt;
	struct node *lt;
	struct node *rt;
	struct node* next;
	struct node* prev;
	struct node* end;
	long height;

}TreeNode;

typedef struct TTree
{
	TreeNode *root;
	TreeNode *nil;
	void* (*createElement)(void*);
	void (*destroyElement)(void*);
	void* (*createInfo)(void*);
	void (*destroyInfo)(void*);
	int (*compare)(void*, void*);
	long size;
}TTree;


TTree* createTree(void* (*createElement)(void*), void (*destroyElement)(void*),
		void* (*createInfo)(void*), void (*destroyInfo)(void*),
		int compare(void*, void*))
{

//CREEAZA ARBORELE +  CELULA DUMMY SI SETEAZA TOATE LINKURILE ACESOTRA
  TTree* arbore = NULL;
  TreeNode* nil = NULL;

//CELULA DUMMY
  nil = malloc(sizeof( TreeNode ));
  if( nil == NULL ) return NULL;
  
   nil->elem = NULL;
   nil->info = NULL;
   
   nil->lt = nil;
   nil->rt = nil;
   nil->pt = nil;
   nil->next = nil;
   nil->prev = nil;
   nil->end = nil;
   nil->height = 0;

 //ARBORE
  arbore = malloc(sizeof( TTree ));
  if(arbore == NULL ) return NULL;
  
   arbore->root = nil;
   arbore->nil = nil;
   
   arbore->createElement = createElement;
   arbore->destroyElement = destroyElement;
   arbore->createInfo = createInfo;
   arbore->destroyInfo = destroyInfo;
   arbore->compare = compare;

   arbore->size = 0;

	/* 
	 * TODO: 
	 * 1. Allocate tree
     * 2. Allocate sentinel
     * 3. Set sentinel
     * 4. Set all other fields
	 */

return arbore;
}

TreeNode* createTreeNode(TTree *tree, void* value, void* info)
{   

  //CREEAZA UN NOU NOD SI SETEAZA CAMPURILE ACESTUIA.
  TreeNode* newNode = NULL;  

  newNode = malloc(sizeof(TreeNode));
  if( newNode == NULL ) return NULL;

  newNode->pt = tree->nil;
  newNode->lt = tree->nil;
  newNode->rt = tree->nil;
  newNode->next = tree->nil;
  newNode->prev = tree->nil;
  newNode->end = tree->nil;
  
  newNode->elem = tree->createElement(value);
  newNode->info = tree->createInfo(info);
  newNode->height = 1;

return newNode;

/* 
   	 * 1. Allocate new node
     * 2. Set it's fields using appropriate functions
     * 3. Set all links
     * 4. Set height for the new node
*/
}

void destroyTreeNode(TTree *tree, TreeNode* node)
{
   //DEZALOCA NODUL PRIMIT.
   if( node == tree->nil ) return;

   tree->destroyElement(node->elem);
   tree->destroyInfo(node->info);
  
   free(node);

   node = tree->nil;

/* 
	* 1. Destroy fields
  * 2. De-allocate node
*/

}

int isEmpty(TTree* arbore)
{
	//DACA ESTE NULA SE RETURNEAZA 1 ALTFEL 0.
  if( arbore->root->height == 0 ) return 1;
  return 0; 
}

TreeNode* search(TTree* tree, TreeNode* node, void* elem)
{

  TreeNode* save = node;
  //CAUTA NODUL CU VALOAREA DE ELEMENT EGALA CU ELEM
  while( save != tree->nil &&  tree->compare(elem , save->elem ) != 0 )
  {

    if(tree->compare(elem , save->elem ) == -1 )
      save = save->lt;
    else
      save = save->rt;
  }
  return save;

/*
   * Note:
   * If the tree is empty the function will return NULL
  TODO: Search sub-tree rooted at node x
  Hint: Elements are compared via compare function for this node
*/
}

TreeNode* minimum(TTree*tree, TreeNode* node)
{
  //Minimul de pe arbore reprezinta cea mai mica frunza din stanga.
  while( node->lt != tree->nil)
         node = node->lt;

  return node;
}

TreeNode* maximum(TTree* tree, TreeNode* node)
{

  //Maximul de pe arbore reprezinta cea mai mare frunza din dreapta.
  while( node->rt != tree->nil)
         node = node->rt;

  return node;

}

TreeNode* successor(TTree* tree, TreeNode* node)
{

  //ARE ROL DE A GASI PREDECSORUL NODULUI NODE.
  //CAT TIMP NODUL NU ESTE NULL SI NICI EGAL CU NODUL DREPT 
  //ACESTA SE DUCE LA PARINTELE LUI
  //RETURNEAZA PARINTELE PRIMULUI NOD CARE ARE FIU LA DREAPTA.
  if(node == tree->nil)
    return tree->nil;

  if(node->rt != tree->nil)
    return minimum(tree,node->rt);

  TreeNode* save = node->pt;

  while(save != tree->nil && node == save->rt)
  {
    node = save;
    save = save->pt;
  }

  return save;
}

TreeNode* predecessor(TTree* tree, TreeNode* node)
{
  //ARE ROL DE A GASI PREDECSORUL NODULUI NODE.
  //CAT TIMP NODUL NU ESTE NULL SI NICI EGAL CU NODUL STANG 
  //ACESTA SE DUCE LA PARINTELE LUI
  //RETURNEAZA PARINTELE PRIMULUI NOD CARE ARE FIU LA STANGA. 
  if(node == tree->nil)
    return tree->nil;

  if(node->lt != tree->nil)
    return maximum(tree,node->lt);

  TreeNode* save = node->pt;

  while(save != tree->nil && node == save->lt)
  {
    node = save;
    save = save->pt;
  }

  return save;
}
//nope
void avlRotateLeft(TTree* tree, TreeNode* x)
{
	TreeNode *y = x->rt;
	x->rt = y->lt;

	if(y->lt != tree->nil)
		y->lt->pt = x; // the parent of the y left subtree is x
	y->pt = x->pt; // update parent for y

	if(x->pt == tree->nil)
		tree->root = y;
	else if(x->pt->lt == x)
		x->pt->lt = y;
	else
		x->pt->rt = y;

	y->lt = x; // x is to the left of y
	x->pt = y; // x parent is y

	// Update heights
	y->height = MAX(y->lt->height, y->rt->height) + 1;
	x->height = MAX(x->lt->height, x->rt->height) + 1;

}

//nope
void avlRotateRight(TTree* tree, TreeNode* y)
{
	TreeNode *x = y->lt;

	y->lt = x->rt;
	if(x->rt != tree->nil)
		x->rt->pt = y;

	x->pt = y->pt;

	if(y->pt == tree->nil)
		tree->root = x;
	else if(y->pt->lt == y)
		y->pt->lt = x;
	else
		y->pt->rt = x;

	x->rt = y;
	y->pt = x;

	// Update heights
	y->height = MAX(y->lt->height, y->rt->height) + 1;
	x->height = MAX(x->lt->height, x->rt->height) + 1;
}

int avlGetBalance(TTree* tree, TreeNode* node)
{	

  //ARE CA ROG DE RETURNARE UN NUMAR CARE REPREZINTA DIFERENTA DINTRE
  //INTALTIMEA PE NOD STANGA - INALTIMEA PE NOD DREAPTA
  if (node == tree->nil ) return 0;
  if(node->lt == tree->nil && node->rt == tree->nil ) return 0;


  int stanga = 0;
  int dreapta = 0;
   
  if ( node->lt != tree->nil )   
  { 
    stanga = node->lt->height;
  }
  if ( node->rt != tree->nil ) 
  { 
    dreapta = node->rt->height;
  }

return stanga - dreapta;
}

void avlFixUp(TTree* tree, TreeNode* nod)
{
 TreeNode* node = nod;

 while( node != tree->nil)
 {

  node->height = MAX(node->lt->height, node->rt->height)+1;
  int balance = avlGetBalance(tree , node);
   
   //ROTATIE DREAPTA  
   if( balance > 1 && avlGetBalance(tree , node->lt) >= 0 )
   {
      avlRotateRight(tree,node);
   }
   //ROTATIE STANGA
   if( balance < -1 && avlGetBalance(tree , node->rt) <= 0 )
   {  
      avlRotateLeft(tree,node);

   }
   //ROTATIE ZIGZAG1
   if( balance > 1 && avlGetBalance(tree , node->lt) < 0 )
   {
      avlRotateLeft(tree,node->lt);
      avlRotateRight(tree,node);
   }
   //ROTATIE ZIGZAG2
   if( balance < -1 && avlGetBalance(tree, node->rt) > 0 )
   {
      avlRotateRight(tree,node->rt);
      avlRotateLeft(tree,node);
   }
 
 node = node->pt;		
 }

/* 
	 * TODO: 
	 * 1. Iterate up-wards to nil
     * 2. Update hight and get balance
     * 3. Test for each case
*/
}

void insert(TTree* tree, void* elem, void* info) 
{

  TreeNode* newNode = tree->nil;
  TreeNode* saveNode = tree->nil;
  TreeNode* node = tree->nil;
  TreeNode* dublicate ;
  TreeNode* save = tree->nil;
  
  newNode =  createTreeNode(tree,elem,info);
  newNode->end = newNode;

  //CAZ INSERT DUBLICATA
  if ( (dublicate = search(tree, tree->root, elem ) ) != tree->nil )
  {

    save  = dublicate->end->next;
    //SETARE DE LINK
    dublicate->end->next = newNode;
    dublicate->end = newNode;
 
    newNode->prev = dublicate;
    newNode->next = save;
    newNode->end = newNode;
    
    if ( save != tree->nil )
      save->prev = newNode;
    return;
 
  }
  
  //PARCURGERE PANA GASESTE POZITIA DE INSERT.
  node = tree->root;
  while ( node != tree->nil ) 
  {  
    saveNode = node;
    if ( tree->compare(elem , node->elem ) == -1) 
    {
      node = node->lt;
    }
    else if (tree->compare(elem , node->elem ) == 1) 
    {
      node = node->rt;
    }
    else return;
  }

  //CAZ 1: ARBORE GOL.
  if ( saveNode == tree->nil ) 
  {
    tree->root = newNode;
    newNode->prev = tree->nil;
    newNode->next = tree->nil;
    newNode->pt = tree->nil; 
  }

  //CAZ 2: INSERT LA STANGA
  else if ( tree->compare(newNode->elem , saveNode->elem ) == -1 ) 
  {

    saveNode->next = newNode;
    newNode->prev = saveNode;
    saveNode->lt = newNode;

  }
  //CAZ 3: INSERT LA DREAPTA
  else if ( tree->compare(newNode->elem , saveNode->elem ) == 1 ) 
  {
    saveNode->next = newNode;
    newNode->prev = saveNode;
    saveNode->rt = newNode;   
  }

  newNode->pt = saveNode;     
  tree->size++;

  //ECHILIBRARE
  avlFixUp(tree, saveNode);
   	  
return;
 	/* 
	 * TODO: 
	 * 1. Create new node
     * 2. Iterate to down-wards to nil 
     *    (duplicates are added to the list for the search node)
     * 3. Update tree 
     * 4. Update linked list
     * 5. Update size of tree and call fix-up
	 */
}


void delete(TTree* tree, void* elem)
{

  TreeNode* node_del; 
  TreeNode* spliced; 
  TreeNode* child;
  TreeNode* dublicate;
  TreeNode* save;

   // Find the node in the tree if it exists
  node_del = search(tree , tree->root, elem);
  dublicate = search(tree , tree->root, elem);

  if( node_del  == tree->nil )
  return;
  
  //CAZ STERGERE DUBLICATE
  if( dublicate->next != tree->nil  )
  {
     //CAZ IN CARE ELEMENTUL URMATOR ESTE LAFEL CU CEL DE ACUM
    if( tree->compare(dublicate->elem , dublicate->next->elem) == 0 )
    { 

      dublicate = dublicate->next;
      save = dublicate;
      
      //LINK
      if ( dublicate->prev != tree->nil )
           dublicate->prev->next = dublicate->next;

      if ( dublicate->next != tree->nil )
           dublicate->next->prev = dublicate->prev;

      if( dublicate->next == tree->nil )
      {
        dublicate->prev->end = dublicate->prev;
      }

      if( dublicate->prev != tree->nil && dublicate->next != tree->nil )
      if ( tree->compare( dublicate->prev->elem , dublicate->next->elem ) != 0 )
      {
          dublicate->prev->end = dublicate->prev;
      }

     //FREE
     destroyTreeNode(tree,save); 
     return;
    }   
  } 


 // CAZURI( NU ARE COPII/ ARE UN COPIL / ARE 2 COPII)
 spliced = ( (node_del->lt == tree->nil) || (node_del->rt == tree->nil) ) ? node_del : successor(tree, node_del);

 //SETEAZA COPILL DREAPTA DACA STANGA E NULL
 child = (spliced->lt == tree->nil) ? spliced->rt : spliced->lt;


 if(child != tree->nil)
   child->pt = spliced->pt;

 //ROOT
 if(spliced->pt == tree->nil)
   tree->root = child;

 //COPIL STANGA
 else if( spliced == spliced->pt->lt)
   spliced->pt->lt = child;

 //COPIL DREAPTA
 else
   spliced->pt->rt = child;

   
  if (spliced != node_del)
    node_del->elem = spliced->elem;


  TreeNode* aux = spliced;
 // SETARE NEXT SI PREV
   if ( spliced->prev != tree->nil )
           spliced->prev->next = spliced->next;

   if ( spliced->next != tree->nil )
           spliced->next->prev = spliced->prev;

  spliced = spliced->pt; 

//  destroyTreeNode(tree,aux); 

  free(aux);
  avlFixUp( tree ,spliced );
  tree->size--;

return;
/*
	 * 1. Search for the node
     * 2. If duplicate delete from the list and exit
     * 3. Otherwise perform tree deletion and update linked list at the end
     * 4. Update size and call fix-up
     * 
     * Hints:
     * 1. If you MUST delete a node from the tree than it has no duplicates!
     *
     * 2. Changing the links with the splice out node is the way to GO,
     *    exchaning node fields does not work in this case,
          you might have duplicates for the splice-out node!!!! 
*/

}

void destroyTree(TTree* tree)
{

  //PARCURGE LISTA PENTRU A STERGE FIECARE ELEMENT.
  //STERGE ARBORELE.
  if( isEmpty(tree) ) return;

  TreeNode* node = tree->root;
  TreeNode* save;
  TreeNode* save2;

  while( node->prev != tree->nil )
	{
    save = node;
    node = node->prev;
  }

   
  while( save != tree->nil)
  {
   
    save2 = save;  
    save = save->next;

    destroyTreeNode(tree,save2); 
  }
 
  free(tree->nil);
  free(tree);
  tree = NULL;
 

	// Hint: Can you use the list?

}


#endif /* AVLTREE_H_ */
