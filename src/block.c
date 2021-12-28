#include "block.h"

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Génère un hash sous format SHA-256 à partir d'un string.
 */
unsigned char *_generateSHA256(char *string);

unsigned char *generateHash(Block block)
{
  char buffer[1000];
  char prevHashString[65];
  hashToString(block.previousHash, prevHashString);
  snprintf(buffer, 1000, "%lu%200s%200s%64s%lu%i", block.index, block.auteur,
           block.message, prevHashString, block.timestamp, block.nonce);

  unsigned char *hash = _generateSHA256(buffer);
  return hash;
}

Block *createBlock(char *auteur, time_t timestamp, char *message,
                   Block *previousBlock)
{
  Block *newBlock = malloc(sizeof(Block));
  if (newBlock == NULL)
  {
    printf("Memory allocation failed : createBlock -> newBlock");
    exit(1);
  }
  snprintf(newBlock->auteur, sizeof(newBlock->auteur), "%s", auteur);
  newBlock->timestamp = timestamp;
  snprintf(newBlock->message, sizeof(newBlock->message), "%s", message);
  newBlock->index = previousBlock->index + 1;
  memcpy(newBlock->previousHash, previousBlock->hash, SHA256_DIGEST_LENGTH);
  newBlock->nonce = 0;
  return newBlock;
}

unsigned char *_generateSHA256(char *string)
{
  unsigned char *hash =
      (unsigned char *)malloc(SHA256_DIGEST_LENGTH * sizeof(unsigned char));

  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, string, strlen(string));
  SHA256_Final(hash, &sha256);

  return hash;
}

void hashToString(unsigned char hash[SHA256_DIGEST_LENGTH], char dest[65])
{
  for (int i = 0, j = 0; i < 16; i++, j += 2)
  {
    sprintf(dest + j, "%02x", hash[i]);
  }
  dest[64] = 0;
}

void printBlock(Block block)
{
  char hashString[65];
  char prevHashString[65];
  hashToString(block.hash, hashString);
  hashToString(block.previousHash, prevHashString);
  printf("-- Hash : %s --\n", hashString);
  printf("Index : %lu\n", block.index);
  printf("Auteur : %s\n", block.auteur);
  printf("DateTime : %li\n", block.timestamp);
  printf("Message : %s\n", block.message);
  printf("Previous Hash : %s\n", prevHashString);
  printf("Nonce : %i\n", block.nonce);
}

Block *duplicateBlock(Block *source)
{
  Block *destination = (Block *)malloc(sizeof(Block));
  if (destination == NULL)
  {
    printf("Memory allocation failed : duplicateBlock -> destination");
    exit(1);
  }
  destination->index = source->index;
  strncpy(destination->auteur, source->auteur, 200);
  destination->timestamp = source->timestamp;
  strncpy(destination->message, source->message, 200);
  memcpy(destination->hash, source->hash, SHA256_DIGEST_LENGTH);
  memcpy(destination->previousHash, source->previousHash, SHA256_DIGEST_LENGTH);
  destination->nonce = source->nonce;
  return destination;
}