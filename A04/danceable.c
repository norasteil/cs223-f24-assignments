/*----------------------------------------------
 * Author: Nora Steil
 * Date: 9/24/24
 * Description: Read in a list of songs and create a linked list. Print the most danceable song and then remove it from the linked list.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct song
{
  char title[128];
  char artist[128];
  int duration;
  float danceability;
  float energy;
  float tempo;
  float valence;
};

struct node
{
  struct song val;
  struct node *next;
};

struct node *insert_front(struct song val, struct node *head)
{
  struct node *n = malloc(sizeof(struct node));
  if (n == NULL)
  {
    printf("Error: out of space\n");
    exit(1);
  }
  n->val = val;
  n->next = head;
  return n;
}

void printSongs(struct node *head)
{
  printf("Welcome to Nora's dynamic song list.\n\n");
  struct node *n = head;
  int i = 0;
  while (n != NULL)
  {
    struct song *currentSong = &n->val;

    int minutes = currentSong->duration / 60000;
    int seconds = (currentSong->duration % 60000) / 1000;

    char duration[16];
    sprintf(duration, "%02d:%02d", minutes, seconds);

    printf("%d) %-22s artist: %-22s duration: %-22s D: %-22g E: %-22g T: %-22g V: %-22g\n", i, currentSong->title, currentSong->artist, duration, currentSong->danceability, currentSong->energy, currentSong->tempo, currentSong->valence);

    n = n->next;
    i++;
  }
  printf("\nDataset contains %d songs\n", i);
}

void printMax(struct song *s)
{
  printf("Most Danceable:\n");

  int minutes = s->duration / 60000;
  int seconds = (s->duration % 60000) / 1000;

  char duration[16];
  sprintf(duration, "%02d:%02d", minutes, seconds);

  printf("%-22s artist: %-22s duration: %-22s D: %-22g E: %-22g T: %-22g V: %-22g\n\n", s->title, s->artist, duration, s->danceability, s->energy, s->tempo, s->valence);
}

struct node *mostDanceable(struct node **head)
{
  if (*head == NULL)
  {
    printf("Dataset contains 0 songs\n\n");
    return NULL;
  }

  struct node *n = *head;
  struct node *maxSong = *head;
  struct node *prev = NULL;
  struct node *prevMax = NULL;

  while (n != NULL)
  {
    if (n->val.danceability > maxSong->val.danceability)
    {
      maxSong = n;
      prevMax = prev;
    }

    prev = n;
    n = n->next;
  }

  if (prevMax == NULL) // deleting head
  {
    *head = maxSong->next;
  }
  else // deleting from middle or tail
  {
    prevMax->next = maxSong->next;
  }

  printMax(&maxSong->val);
  free(maxSong);
  maxSong = NULL;
  return *head;
}

int main()
{
  char fileName[] = "songlist.csv";

  FILE *file = fopen(fileName, "r");
  if (file == NULL)
  {
    printf("Error: unable to open file %s\n", "songlist.csv");
    exit(1);
  }

  char line[512];
  struct node *head = NULL;

  fgets(line, sizeof(line), file);

  // make a song struct and a linked list node for ea line
  while (fgets(line, sizeof(line), file) != NULL)
  {
    struct song newSong;
    char *token = strtok(line, ",");
    strcpy(newSong.title, token);

    token = strtok(NULL, ",");
    strcpy(newSong.artist, token);

    token = strtok(NULL, ",");
    newSong.duration = atoi(token);

    token = strtok(NULL, ",");
    newSong.danceability = atof(token);

    token = strtok(NULL, ",");
    newSong.energy = atof(token);

    token = strtok(NULL, ",");
    newSong.tempo = atof(token);

    token = strtok(NULL, ",");
    newSong.valence = atof(token);

    head = insert_front(newSong, head);
  }

  printSongs(head);

  char input;
  printf("Press 'd' to show the most danceable song (any other key to quit): ");
  scanf(" %c", &input);

  while (1)
  {
    if (input != 'd')
    {
      break;
    }
    head = mostDanceable(&head);

    if (head != NULL)
    {
      printSongs(head);
    }

    printf("Press 'd' to show the most danceable song (any other key to quit): ");
    scanf(" %c", &input);
  }

  struct node *prev;
  while (head != NULL)
  {
    prev = head;
    head = head->next;
    free(prev);
  }

  fclose(file);
  return 0;
}
