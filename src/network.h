#ifndef NETWORK_H
#define NETWORK_H

#include "blockchain.h"

#define DICTIONARY_SIZE 200  // Nombre d'utilisateurs possible

typedef struct miner_t {
  /**
   * Identifiant unique d'un utilisateur.
   */
  long nodeAddress;
  /**
   * Blockchain d'un utilisateur.
   */
  Blockchain* blockchain;
} Miner;

typedef struct network_t {
  /**
   * Nombre d'utilisateurs.
   */
  int length;
  /**
   * Récupère la liste de tout les mineurs.
   */
  Miner** all;
} Network;

/**
 * Alloue de la mémoire et créer un network.
 */
Network* createNetwork(void);
/**
 * Ajoute au réseau un nouveau membre.
 */
Miner* addToNetwork(Network* network, long nodeAddress,
                    Blockchain* ourBlockchain);
/**
 * Trouve le miner.
 */
Miner* findMiner(Network* network, long nodeAddress);

/**
 * Effectue le mining.
 */
void mineForNetwork(Network* network, Miner* ourMiner, char* auteur,
                    char* message);
/**
 * Afficher la blockchain du miner indiqué.
 */
void printBlockchainOfMiner(Miner* miner);
/**
 * Afficher le dernier bloc du miner.
 */
void printLastBlockOfMiner(Miner* miner);
/**
 * Afficher le bloc selon le Hash indiqué.
 */
void printBlockFromHashOfMiner(Miner* miner);
/**
 * Afficher le bloc selon l'index indiqué.
 */
void printBlockFromIndexOfMiner(Miner* miner);
/**
 * Afficher un mining.
 */
void printMineForNetwork(Network* network, Miner* ourMiner);
/**
 * Affiche le réseau.
 */
void printNetwork(Network* network);

#endif
