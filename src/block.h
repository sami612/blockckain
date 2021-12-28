#if !defined(BLOCK_H)
#define BLOCK_H

#include <openssl/sha.h>
#include <time.h>

typedef struct block_t {
  /**
   * Position dans la chaine
   */
  unsigned long index;
  /**
   * Auteur du block
   */
  char auteur[200];
  /**
   * Secondes depuis Jan 01 1970 UTC
   */
  time_t timestamp;
  /**
   * Description du message
   */
  char message[200];
  /**
   * Identifiant
   */
  unsigned char hash[SHA256_DIGEST_LENGTH];

  /**
   * Identifiant du block précédent
   *
   * 0 si GenesisBlock.
   */
  unsigned char previousHash[SHA256_DIGEST_LENGTH];

  /**
   * Nonce pour PoW Algorithm
   */
  int nonce;

} Block;

typedef struct chain_t {
  /**
   * Donnée.
   */
  Block* block;

  /**
   * Pointeur vers la chaîne suivante.
   */
  struct chain_t* next;

  /**
   * Pointeur vers la chaîne précédente.
   */
  struct chain_t* prev;
} Chain;

/**
 * Génère un hash SHA-256 pour bloc.
 */
unsigned char* generateHash(Block);

/**
 * Convertie un hash SHA-256 en format hexadecimal.
 */
void hashToString(unsigned char hash[SHA256_DIGEST_LENGTH], char dest[65]);

/**
 * Alloue de la mémoire et créer un block.
 */
Block* createBlock(char* auteur, time_t timestamp, char* message,
                   Block* previousBlock);

/**
 * Affiche toute les données d'un bloc.
 */
void printBlock(Block block);

/**
 * Alloue de la mémoire et duplique un bloc.
 */
Block* duplicateBlock(Block* source);

#endif
