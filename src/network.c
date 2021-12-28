#include "network.h"

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "blockchain.h"

/**
 * Vérifie que la blockchain du mineur est la plus grande.
 *
 * Si non, alors, sa blockchain est remplacée.
 */
bool _consensus(Network* network, Miner* ourMiner);
/**
 * Envoie un nouveau bloc à tous les éléments du réseau.
 */
void _sendNewBlock(Network* network, long sourceNodeAddress, Block* newBlock);
/**
 * Vérifie si le hash est en format hexadécimal.
 */
bool _isNotHashHexa(char hash[65]);

bool _isNotHashHexa(char hash[65]) {
  bool result = true;
  for (int i = 0; i < 65 && hash[i] != '\0'; i++) {
    if (!isxdigit(hash[i])) {
      result = false;
    }
  }
  return !result;
}

Network* createNetwork(void) {
  Network* network = (Network*)malloc(sizeof(Network));
  if (network == NULL) {
    printf("Memory allocation failed : createNetwork -> network");
    exit(1);
  }
  Miner** all = (Miner**)malloc(sizeof(Miner*) * DICTIONARY_SIZE);
  if (all == NULL) {
    printf("Memory allocation failed : createNetwork -> all");
    exit(1);
  }
  for (int i = 0; i < DICTIONARY_SIZE; i++) {
    all[i] = NULL;
  }
  network->all = all;
  network->length = 0;
  return network;
}

Miner* addToNetwork(Network* network, long nodeAddress,
                    Blockchain* ourBlockchain) {
  Miner* newMiner = (Miner*)malloc(sizeof(Miner));
  if (newMiner == NULL) {
    printf("Memory allocation failed : addToNetwork -> newMiner");
    exit(1);
  }

  // Fill data
  newMiner->blockchain = duplicateBlockchain(ourBlockchain);
  newMiner->nodeAddress = nodeAddress;

  network->all[network->length] = newMiner;
  network->length++;
  return newMiner;
}

Miner* findMiner(Network* network, long nodeAddress) {
  for (int i = 0; i < DICTIONARY_SIZE; i++) {
    if (network->all[i] != NULL &&
        network->all[i]->nodeAddress == nodeAddress) {
      return network->all[i];
    }
  }
  return NULL;
}

bool _consensus(Network* network, Miner* ourMiner) {
  Blockchain* longestBlockchain = NULL;
  unsigned long maxLength = ourMiner->blockchain->length;

  // Chercher la plus grande chaine.
  for (int i = 0; i < DICTIONARY_SIZE; i++) {
    if (network->all[i] != NULL) {
      if (maxLength < network->all[i]->blockchain->length &&
          checkChainValidity(network->all[i]->blockchain)) {
        // Longer valid chain found.
        maxLength = network->all[i]->blockchain->length;
        longestBlockchain = network->all[i]->blockchain;
        printf("Longest Chain is %li with length %ld\n",
               network->all[i]->nodeAddress, maxLength);
      }
    }
  }

  if (longestBlockchain != NULL) {
    if (ourMiner->blockchain != NULL) {
      deleteBlockchain(ourMiner->blockchain);
    }
    ourMiner->blockchain = duplicateBlockchain(longestBlockchain);

    return true;
  }
  return false;
}
void _sendNewBlock(Network* network, long sourceNodeAddress, Block* newBlock) {
  for (int i = 0; i < DICTIONARY_SIZE; i++) {
    if (network->all[i] != NULL &&
        network->all[i]->nodeAddress != sourceNodeAddress) {
      Block* duplicateBlock = (Block*)malloc(sizeof(*newBlock));
      if (duplicateBlock == NULL) {
        printf("Memory allocation failed : _sendNewBlock -> duplicateBlock");
        exit(1);
      }
      memcpy(duplicateBlock, newBlock, sizeof(*newBlock));
      if (insert(network->all[i]->blockchain, duplicateBlock,
                 duplicateBlock->hash) == false) {
        printf("Cannot insert block to blockchain %li.\n",
               network->all[i]->nodeAddress);
      };
    }
  }
}

void mineForNetwork(Network* network, Miner* ourMiner, char* auteur,
                    char* message) {
  // Miner
  unsigned long result = mine(ourMiner->blockchain, auteur, message);
  // Ancienne taille
  unsigned long ourLength = ourMiner->blockchain->length;

  // Prend la plus grande blockchain
  _consensus(network, ourMiner);

  // Check si notre blockchain est bien la plus grande
  if (ourMiner->blockchain->length == ourLength) {
    // Envoie le dernier bloc
    _sendNewBlock(network, ourMiner->nodeAddress,
                  ourMiner->blockchain->head->block);
  } else {
    printf("%s\n", "Blockchain length invalid.");
    printf("New %ld\nOld %ld\n", ourMiner->blockchain->length, ourLength);
  }
  char hashString[65];
  hashToString(ourMiner->blockchain->head->block->hash, hashString);
  printf("Block %ld is mined.\nHash: %s\n", result, hashString);
}

void printBlockchainOfMiner(Miner* miner) {
  printBlockchain(miner->blockchain);
}

void printLastBlockOfMiner(Miner* miner) { printLastBlock(miner->blockchain); }

void printBlockFromHashOfMiner(Miner* miner) {
  char hashString[65] = "NOT HEXA";
  while (_isNotHashHexa(hashString)) {
    printf("%s\n", "Hash (en hexa) ?");
    scanf("%64s", hashString);
  }

  printBlockFromHash(miner->blockchain, hashString);
}

void printBlockFromIndexOfMiner(Miner* miner) {
  unsigned long index = ULONG_MAX;
  while (index == ULONG_MAX) {
    printf("%s\n", "Index (en entier) ?");
    scanf("%lu", &index);
  }

  printBlockFromIndex(miner->blockchain, index);
}

void printMineForNetwork(Network* network, Miner* ourMiner) {
  char* auteur = (char*)malloc(sizeof(char) * 200);
  char* message = (char*)malloc(sizeof(char) * 200);
  printf("%s\n", "Auteur du bloc (200 caratères max) ?");
  scanf("%199s", auteur);
  printf("%s\n", "Message (200 caratères max) ?");
  scanf("%199s", message);

  mineForNetwork(network, ourMiner, auteur, message);
}

void printNetwork(Network* network) {
  for (int i = 0; i < DICTIONARY_SIZE; i++) {
    if (network->all[i] != NULL) {
      printf("-- Utilisateur %i --\n", i);
      printf("NodeAddress : %lx\n", network->all[i]->nodeAddress);
      printf("Blockchain Length : %lx\n", network->all[i]->blockchain->length);
    }
  }
}
