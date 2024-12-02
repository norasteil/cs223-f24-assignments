#include "tree.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>

struct tree_node *find(const char *name, struct tree_node *root)
{
  if (root == NULL)
  {
    return NULL;
  }

  int compare = strcmp(name, root->data.name);
  if (compare == 0)
  {
    return root;
  }
  else if (compare < 0)
  {
    return find(name, root->left);
  }
  else
  {
    return find(name, root->right);
  }
}

struct tree_node *insert(const char *name, struct tree_node *root)
{
  if (root == NULL)
  {
    struct tree_node *node = (struct tree_node *)malloc(sizeof(struct tree_node));
    if (!node)
    {
      fprintf(stderr, "error: memory allocation failed for tree node\n");
      return NULL;
    }

    strncpy(node->data.name, name, sizeof(node->data.name) - 1);
    node->data.name[sizeof(node->data.name) - 1] = '\0';
    node->left = NULL;
    node->right = NULL;
    return node;
  }

  int compare = strcmp(name, root->data.name);
  if (compare < 0)
  {
    root->left = insert(name, root->left);
  }
  else if (compare > 0)
  {
    root->right = insert(name, root->right);
  }
  return root;
}

void clear(struct tree_node *root)
{
  if (root == NULL)
  {
    return;
  }

  clear(root->left);
  clear(root->right);
  free(root);
}

void print(struct tree_node *root)
{
  static int depth = 0;

  if (root == NULL)
  {
    return;
  }

  if (depth == 0)
  {
    printf("%s\n", root->data.name);
  }

  if (root->left)
  {
    for (int i = 0; i < depth + 1; i++)
    {
      printf(" ");
    }
    printf("l: %s\n", root->left->data.name);
    depth++;
    print(root->left);
    depth--;
  }

  if (root->right)
  {
    for (int i = 0; i < depth + 1; i++)
    {
      printf(" ");
    }
    printf("r: %s\n", root->right->data.name);
    depth++;
    print(root->right);
    depth--;
  }
}

void printSorted(struct tree_node *root)
{
  if (root == NULL)
  {
    return;
  }

  printSorted(root->left);
  printf("%s\n", root->data.name);
  printSorted(root->right);
}
