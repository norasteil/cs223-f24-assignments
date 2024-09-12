/***************************************************
 * songs.c
 * Author: Nora Steil
 * Creates an editable array of songs
 */

#include <stdio.h>
#include <string.h>

struct song
{
  char artist[32];
  char title[32];
  int durationMin;
  int durationSec;
  float danceability;
};

void printSongs(struct song songs[], int numSongs)
{
  printf("Welcome to Nora's song list.\n\n");
  for (int i = 0; i < numSongs; i++)
  {
    printf("%d) %-20s artist: %-20s duration: %d:%02d         danceability %g\n", i, songs[i].title, songs[i].artist, songs[i].durationMin, songs[i].durationSec, songs[i].danceability);
  }
  printf("\n");
}

void editAttribute(struct song songs[], int numSongs, char attribute[], int song)
{
  char strValue[32];
  int intValue;
  float floatValue;

  if (strcmp(attribute, "duration") != 0)
  {
    printf("Enter a %s: ", attribute);
  }

  if (strcmp(attribute, "artist") == 0)
  {
    scanf("%[^\n]%*c", strValue);
    strcpy(songs[song].artist, strValue);
  }
  else if (strcmp(attribute, "title") == 0)
  {
    scanf("%[^\n]%*c", strValue);
    strcpy(songs[song].title, strValue);
  }
  else if (strcmp(attribute, "duration") == 0)
  {
    printf("Enter a %s (minutes): ", attribute);
    scanf("%d%*c", &intValue);
    songs[song].durationMin = intValue;

    printf("Enter a %s (seconds): ", attribute);
    scanf("%d%*c", &intValue);
    songs[song].durationSec = intValue;
  }
  else if (strcmp(attribute, "danceability") == 0)
  {
    scanf("%g%*c", &floatValue);
    songs[song].danceability = floatValue;
  }
}

int main()
{
  struct song songs[3];

  struct song s1 = {"MUNA", "Loose Garment", 3, 10, 0.3};
  songs[0] = s1;

  struct song s2 = {"Ethel Cain", "Sun Bleached Flies", 7, 36, 0.1};
  songs[1] = s2;

  struct song s3 = {"Rico Nasty", "OHFR?", 2, 0, 0.7};
  songs[2] = s3;

  printSongs(songs, 3);

  int song;
  char attribute[64];
  printf("Enter a song id to edit [0, 1, 2]: ");
  scanf("%d%*c", &song);

  if (song >= sizeof(songs) / sizeof(songs[0]) || song < 0)
  {
    printf("Invalid song choice.\n");
    return 0;
  }

  printf("Which attribute do you wish to edit? [artist, title, duration, danceability]: ");
  scanf("%[^\n]%*c", attribute);

  editAttribute(songs, 3, attribute, song);

  printSongs(songs, 3);

  return 0;
}
