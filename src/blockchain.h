#if !defined(BLOCKCHAIN_H)
#define BLOCKCHAIN_H

#include <stdbool.h>
#include "block.h"

typedef struct blockchain_t {
  /**
   * Tête de liste chaînée. Dernier block ajouté.
   */
  Chain* head;
  /**
   * Difficulté de minage selon l'algorithme du nonce.
   */
  int difficulty;
  /**
   * Taille de la blockchain.
   */
  unsigned long length;
} Blockchain;

/**
 * Génére un blockchain avec une difficulté de minage.
 */
Blockchain* createBlockchain(int difficulty);

/**
 * Génére un blockchain avec une difficulté de minage.
 */
bool insert(Blockchain* blockchain, Block* newBlock, unsigned char* proof);

/**
 * Mine et ajoute un block.
 */
unsigned long mine(Blockchain* blockchain, char* auteur, char* message);

/**
 * Vérifie la validité de toute la chaîne.
 */
bool checkChainValidity(Blockchain*);

/**
 * Supprime toute la chaîne et libère de la mémoire.
 */
void deleteBlockchain(Blockchain*);

/**
 * Alloue de la mémoire et duplique toute la chaîne.
 */
Blockchain* duplicateBlockchain(Blockchain* source);

/**
 * Affiche toute la chaîne.
 */
void printBlockchain(Blockchain* blockchain);

/**
 * Affiche le dernier bloc.
 */
void printLastBlock(Blockchain* blockchain);

/**
 * Affiche un bloc particulier en fonction du hash.
 */
void printBlockFromHash(Blockchain* blockchain, char hashString[65]);

/**
 * Affiche un bloc particulier en fonction de l'index.
 */
void printBlockFromIndex(Blockchain* blockchain, unsigned long index);
#endif
