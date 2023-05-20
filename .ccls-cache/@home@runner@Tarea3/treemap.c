#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) 
{
  TreeMap * map = (TreeMap*) malloc (sizeof(TreeMap));
  if(map == NULL) return NULL;
  map->root = NULL;
  map->current = NULL;
  map ->lower_than = lower_than; 
  return map;
}

void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
  TreeNode* actual = tree->root;
  TreeNode* nuevoHijo = NULL;
  while (actual != NULL) 
  {
    nuevoHijo = actual;
    if (is_equal(tree, key, actual->pair->key))
    {
      return;
    } else if (tree->lower_than(key, actual->pair->key)) 
    {
      actual = actual->left;
    } else {
        actual = actual->right;
    }
  }

  TreeNode* nodito = createTreeNode(key, value);

  nodito->parent = nuevoHijo;
  if (nuevoHijo == NULL) {
        tree->root = nodito;
  } else if (tree->lower_than(key, nuevoHijo->pair->key)) 
    {
        nuevoHijo->left = nodito;
    } else 
    {
      nuevoHijo->right = nodito;
    }
  
  tree->current = nodito;
}

TreeNode * minimum(TreeNode * x){
if (x == NULL) return NULL; 
  while (x->left != NULL) 
  { 
       x = x->left;
  }
  return x;
}


void removeNode(TreeMap * tree, TreeNode* node) 
{
  //caso 1 nodo sin hijos
  if(node->left == NULL && node->right == NULL)
  {
    if(node->parent != NULL)
    {
      if(node->parent->left == node)
      {
        node->parent->left = NULL;
      }
      else{
        node->parent->right = NULL;
      }
    }else{
      tree->root = NULL;
    }
    free(node);
    return;
  }

  //caso 2 con un hijoooo
  if(node->left == NULL || node->right == NULL)
  {
    TreeNode* aux = node->left != NULL ? node->left : node->right;
    if(node->parent != NULL)
    {
      if (node->parent->left == node) {
        node->parent->left = aux;
      } else {
        node->parent->right = aux;
      }
      aux->parent = node->parent;
    } else {
      tree->root = aux;
      aux->parent = NULL;
    }
    free(node);
    return;
  }
  
  // caso 3 con dos hijoo
  TreeNode* ultimoNodo = minimum(node->right);
  node->pair->key = ultimoNodo->pair->key;
  node->pair->value = ultimoNodo->pair->value;
  removeNode(tree, ultimoNodo);
    
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) 
{
  //lowerthan para saber si ir por izquierda o derecha 
  //is equal para saber si es igual o no 

  if(tree == NULL ) return NULL;
  TreeNode *actual = tree->root;
  
  while (actual != NULL) 
  {
    if (is_equal(tree, actual->pair->key, key)) 
    {
      tree->current = actual;
      return actual->pair;
    } else if (tree->lower_than(key, actual->pair->key) == 1) 
      {
        actual = actual->left;
      } else 
        {
          actual = actual->right;
        }
  }
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
  TreeNode* current = tree->root;
  
  TreeNode* ub_node = NULL;
  
  while (current != NULL) 
  {
    if (is_equal(tree, current->pair->key, key)) 
    {
      return current->pair;
    } else if (tree->lower_than(key, current->pair->key)) {
      ub_node = current;
      current = current->left;
    } else {
      current = current->right;
    }
  }
  
  if (ub_node == NULL) 
  {      
    return NULL;
  } else {
      return ub_node->pair;
  }
  
}

Pair * firstTreeMap(TreeMap * tree) 
{
  if(tree == NULL || tree-> root == NULL) return NULL;

  TreeNode* actual = tree->root;

  while (actual->left != NULL)
  {
    actual = actual->left;
  }
  
  tree->current = actual;
  
  return actual->pair;
}

Pair * nextTreeMap(TreeMap * tree)
{
  if(tree == NULL || tree-> current == NULL) return NULL;
  
  TreeNode* actual = tree->current;
  
  if(actual ->right != NULL)
  {
    actual = actual->right;
    
    while(actual->left !=NULL)
    {
       actual = actual->left;   
    }    
  }else{
    TreeNode* tata = actual->parent;
    
    while (tata != NULL && actual == tata->right)
    {
      actual = tata;
      tata = tata->parent;
    }

    actual = tata;
  }
  
  tree->current = actual;
  
  return (actual != NULL) ? actual->pair : NULL;
}
