#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "block.h"
#include "blockchain.h"
#include "network.h"

/**
 * Affiche un menu pour l'utilisateur connecté.
 */
void menu(Network* network, Miner* ourMiner);

/**
 * Vérifie qu'un string est sous forma hexadecimal.
 */
bool isHexa(char string[256]);

bool isHexa(char string[256]) {
  for (int i = 0; i < 256 && string[i] != '\0'; i++) {
    if (!isxdigit(string[i])) {
      return false;
    }
  }
  return true;
}

int main(void) {
  Network* network = createNetwork();

  // 1. Demander la difficulté de minage
  printf("%s\n", "Génération du blockchain...");
  int difficulty = 64;
  while (difficulty > 63) {
    printf("%s\n",
           "Difficulté du minage ?\nIl doit être inférieure à 64 (exemple: 2)");
    scanf("%i", &difficulty);
  }
  Blockchain* firstBlockchain = createBlockchain(difficulty);

  // 2. Demander l'utilisateur de se connecter
  while (true) {
    Miner* ourMiner;
    long nodeAddress;
    char buffer[256] = "NOT A HEXA";
    while (!isHexa(buffer)) {
      printf("%s\n", "Veuillez-vous connecter:\nIdentifiant en hexadecimal : ");
      scanf("%256s", buffer);
    }
    nodeAddress = strtol(buffer, NULL, 16);

    ourMiner = findMiner(network, nodeAddress);
    if (ourMiner == NULL) {
      // Add to network
      if (network->all[0] == NULL) {
        // No existing blockchain
        ourMiner = addToNetwork(network, nodeAddress, firstBlockchain);
        deleteBlockchain(firstBlockchain);
      } else {
        // At least one existing
        ourMiner =
            addToNetwork(network, nodeAddress, network->all[0]->blockchain);
      }

      printf("%s\n", "Compte créé !");
      printf("Identifiant: %lx\n", nodeAddress);
    }

    menu(network, ourMiner);
  }

  return 0;
}

void menu(Network* network, Miner* ourMiner) {
  int choix;
  bool quitter = false;
  while (!quitter) {
    // On affiche le menu
    printf("-- Menu --\n");
    printf(
        "[0] Miner\n"
        "[1] Afficher votre Blockchain \n"
        "[2] Afficher un bloc selon l'index \n"
        "[3] Afficher un bloc selon le Hash\n"
        "[4] Afficher le dernier bloc\n"
        "[5] Afficher le network\n"
        "[6] Changer de compte\n");
    scanf("%d", &choix);
    switch (choix) {
      case 0:
        printMineForNetwork(network, ourMiner);
        break;
      case 1:
        printBlockchainOfMiner(ourMiner);
        break;
      case 2:
        printBlockFromIndexOfMiner(ourMiner);
        break;
      case 3:;
        printBlockFromHashOfMiner(ourMiner);
        break;
      case 4:
        printLastBlockOfMiner(ourMiner);
        break;
      case 5:
        printNetwork(network);
        break;
      case 6:
        quitter = true;
        break;
    }
  }
}
