/*----------------------------------------------
 * Author: Nora Steil
 * Date: 9/16/24
 * Description: Read in a csv file and create a list of songs from it
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

void printSongs(struct song songs[], int numSongs)
{
  printf("Welcome to Nora's dynamic song list.\n\n");
  for (int i = 0; i < numSongs; i++)
  {
    int minutes = songs[i].duration / 60000;
    int seconds = (songs[i].duration % 60000) / 1000;

    char duration[16];
    // format duration using sprintf(char *str, const char *format)
    sprintf(duration, "%02d:%02d", minutes, seconds);

    printf("%d) %-20s artist: %-20s duration: %-20s D: %-20g E: %-20g T: %-20g V: %-20g\n", i, songs[i].title, songs[i].artist, duration, songs[i].danceability, songs[i].energy, songs[i].tempo, songs[i].valence);
  }
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

  fgets(line, sizeof(line), file);
  int numSongs = atoi(strtok(line, ","));

  fgets(line, sizeof(line), file);

  struct song *songs = malloc(sizeof(struct song) * (numSongs));

  for (int i = 0; i < numSongs; i++)
  {
    fgets(line, sizeof(line), file);

    char *token = strtok(line, ",");
    strcpy(songs[i].title, token);

    token = strtok(NULL, ",");
    strcpy(songs[i].artist, token);

    token = strtok(NULL, ",");
    songs[i].duration = atoi(token);

    token = strtok(NULL, ",");
    songs[i].danceability = atof(token);

    token = strtok(NULL, ",");
    songs[i].energy = atof(token);

    token = strtok(NULL, ",");
    songs[i].tempo = atof(token);

    token = strtok(NULL, ",");
    songs[i].valence = atof(token);
  }
  printSongs(songs, numSongs);

  free(songs);
  fclose(file);
  return 0;
}
