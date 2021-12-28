#include "blockchain.h"

#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Vérifie la validité d'un nouveau bloc.
 */
bool _isVerified(Blockchain* blockchain, Block* newBlock, unsigned char* proof);

/**
 * Calcule le PoW du nouveau block en utilisant l'algorithme du nonce.
 */
unsigned char* _computeProofOfWork(Blockchain* blockchain, Block* newBlock);

/**
 * Vérifie que le PoW calculé l'algorithme du nonce paramétré dans blockchain.
 */
bool _checkPoWValidity(Blockchain* blockchain, unsigned char computedHash[65]);

/**
 * Créé le Genesis Bloc.
 */
Block* _createGenesisBlock(void);

/**
 * Vérifie la validité d'un bloc du blockchain.
 */
bool _isNotChainValid(Blockchain* blockchain, Block* currentBlock,
                      unsigned char* previousHash);

Blockchain* createBlockchain(int difficulty) {
  Blockchain* newBlockchain = (Blockchain*)malloc(sizeof(Blockchain));
  if (newBlockchain == NULL) {
    printf("Memory allocation failed : createBlockchain -> newBlockchain");
    exit(1);
  }
  Chain* newChain = (Chain*)malloc(sizeof(Chain));
  if (newChain == NULL) {
    printf("Memory allocation failed : createBlockchain -> newChain");
    exit(1);
  }

  newChain->block = _createGenesisBlock();
  newChain->prev = NULL;
  newChain->next = NULL;
  newBlockchain->head = newChain;
  newBlockchain->difficulty = difficulty;
  newBlockchain->length = 1;
  return newBlockchain;
}

Block* _createGenesisBlock(void) {
  Block* newBlock = (Block*)malloc(sizeof(Block));
  if (newBlock == NULL) {
    printf("Memory allocation failed : _createGenesisBlock -> newBlock");
    exit(1);
  }

  memset(newBlock->auteur, 0, sizeof(newBlock->auteur));

  memset(newBlock->hash, 0, sizeof(newBlock->hash));

  newBlock->index = 0;

  memset(newBlock->message, 0, sizeof(newBlock->message));

  newBlock->timestamp = time(NULL);

  return newBlock;
}

bool _isVerified(Blockchain* blockchain, Block* newBlock,
                 unsigned char* proof) {
  char headHashString[65];
  char previousHashString[65];
  char proofString[65];
  char generatedHashString[65];
  hashToString(blockchain->head->block->hash, headHashString);
  hashToString(newBlock->previousHash, previousHashString);
  hashToString(proof, proofString);
  hashToString(generateHash(*newBlock), generatedHashString);

  bool isLinked = strncmp(headHashString, previousHashString, 65) == 0;
  bool isPowVerified = _checkPoWValidity(blockchain, proof);
  bool isCorrectProof = strncmp(proofString, generatedHashString, 65) == 0;

  if (!isLinked) {
    printf("%s\n", "[ERROR] Linking error");
    printf("Prev        %s\nCurr->Prev  %s\n", headHashString,
           previousHashString);
  }
  if (!isPowVerified) {
    printf("%s\n", "[ERROR] PoW error");
    printf("Proof %s\n", proofString);
  }
  if (!isCorrectProof) {
    printf("%s\n", "[ERROR] Proof calculation error");
    printf("Proof %s\nHash  %s\n", proofString, generatedHashString);
  }
  return isLinked && isPowVerified && isCorrectProof;
}

bool insert(Blockchain* blockchain, Block* newBlock, unsigned char* proof) {
  if (_isVerified(blockchain, newBlock, proof)) {
    Chain* newChain = (Chain*)malloc(sizeof(Chain));
    if (newChain == NULL) {
      printf("Memory allocation failed : insert -> newChain");
      exit(1);
    }

    // Remplir les données.
    newChain->block = newBlock;
    memcpy(newChain->block->hash, proof, SHA256_DIGEST_LENGTH);

    // Lier chaine
    newChain->prev = blockchain->head;
    newChain->next = NULL;
    newChain->prev->next = newChain;
    blockchain->head = newChain;
    blockchain->length++;
    return true;
  } else {
    return false;
  }
}

unsigned char* _computeProofOfWork(Blockchain* blockchain, Block* newBlock) {
  unsigned char* computedHash = generateHash(*newBlock);

  while (_checkPoWValidity(blockchain, computedHash) == false) {
    if (computedHash != NULL) {
      free(computedHash);
    }
    newBlock->nonce++;
    computedHash = generateHash(*newBlock);
  }
  return computedHash;
}

bool _checkPoWValidity(Blockchain* blockchain, unsigned char proof[65]) {
  char outputString[65] = {0};
  hashToString(proof, outputString);
  // Vérifie que les premiers caractères du hash sont des 0 selon la difficulté
  for (int i = 0; i < blockchain->difficulty && i < 65; i++) {
    if (outputString[i] != '0') return false;
  }
  return true;
}

unsigned long mine(Blockchain* blockchain, char* auteur, char* message) {
  Block* block =
      createBlock(auteur, time(NULL), message, blockchain->head->block);

  unsigned char* proof = _computeProofOfWork(blockchain, block);

  if (insert(blockchain, block, proof) == false) {
    printf("%s\n", "Cannot insert mined block to blockchain.");
  };
  return block->index;
}

bool _isNotChainValid(Blockchain* blockchain, Block* currentBlock,
                      unsigned char* previousHash) {
  char previousHashString[65];
  char previousOfCurrentHashString[65];
  char proofString[65];
  char generatedHashString[65];
  hashToString(previousHash, previousHashString);
  hashToString(currentBlock->previousHash, previousOfCurrentHashString);
  hashToString(currentBlock->hash, proofString);
  hashToString(generateHash(*currentBlock), generatedHashString);

  bool isPowVerified = _checkPoWValidity(blockchain, currentBlock->hash);
  bool isCorrectProof = strncmp(proofString, generatedHashString, 65) == 0;
  bool isLinked =
      strncmp(previousHashString, previousOfCurrentHashString, 65) == 0;

  if (!isLinked) {
    printf("%s\n", "[ERROR] Linking error");
    printf("Prev %s\nCurr->Prev  %s\n", previousHashString,
           previousOfCurrentHashString);
  }
  if (!isPowVerified) {
    printf("%s\n", "[ERROR] PoW error");
    printf("Proof %s\n", proofString);
  }
  if (!isCorrectProof) {
    printf("%s\n", "[ERROR] Proof calculation error");
    printf("Proof %s\nHash  %s\n", proofString, generatedHashString);
  }

  return !(isPowVerified && isCorrectProof && isLinked);
}

bool checkChainValidity(Blockchain* blockchain) {
  Chain* current = blockchain->head;
  if (current->block->index != 0) {
    unsigned char* previousHash = current->prev->block->hash;

    while (current->block->index != 0) {  // Jusqu'au GenesisBlock
      if (_isNotChainValid(blockchain, current->block, previousHash)) {
        printf("%s\n", "[FATAL] Chain not valid.");
        return false;
      }
      previousHash = current->block->hash;
      current = current->prev;
    }
    return true;
  } else {
    return true;
  }
}

void deleteBlockchain(Blockchain* blockchain) {
  Chain* chain = blockchain->head;

  while (chain != NULL) {
    free(chain->block);
    Chain* old = chain;
    chain = chain->prev;
    free(old);
  }
  free(blockchain);
}

Blockchain* duplicateBlockchain(Blockchain* source) {
  Blockchain* destination = createBlockchain(source->difficulty);

  // Fill data
  destination->difficulty = source->difficulty;
  destination->length = source->length;

  // Jusqu'au GenesisBlock
  Chain* current = source->head;
  while (current->block->index != 0) {
    current = current->prev;
  }
  // Copy GenesisBlock timestamp
  destination->head->block->timestamp = current->block->timestamp;
  current = current->next;

  while (current != NULL) {  // Jusqu'au dernier Block
    Block* duplicatedBlock = duplicateBlock(current->block);
    if (insert(destination, duplicatedBlock, duplicatedBlock->hash) == false) {
      printf("%s\n", "Cannot insert source block to destination blockchain.");
    };
    current = current->next;
  }
  return destination;
}

void printBlockchain(Blockchain* blockchain) {
  Chain* chain = blockchain->head;
  printf("%s\n", "--START BLOCKCHAIN--");

  while (chain != NULL) {
    printBlock(*chain->block);
    chain = chain->prev;
  }
  printf("%s\n", "--END BLOCKCHAIN--");
}

void printLastBlock(Blockchain* blockchain) {
  Chain* chain = blockchain->head;
  printBlock(*chain->block);
}

void printBlockFromHash(Blockchain* blockchain, char hashString[65]) {
  Chain* chain = blockchain->head;
  char hashStringSearch[256];
  while (chain != NULL) {
    hashToString(chain->block->hash, hashStringSearch);
    if (strcmp(hashStringSearch, hashString) == 0) {
      printBlock(*chain->block);
      return;
    } else {
      chain = chain->prev;
    }
  }
}

void printBlockFromIndex(Blockchain* blockchain, unsigned long index) {
  Chain* chain = blockchain->head;
  while (chain != NULL) {
    if (chain->block->index == index) {
      printBlock(*chain->block);
      return;
    } else {
      chain = chain->prev;
    }
  }
}
