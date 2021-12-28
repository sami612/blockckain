# Blockchain

## Configuration

### Installation des dépendances

Pour utiliser SHA-256, on utilise openssl.

```sh
sudo apt install libssl-dev  # Debian ou Ubuntu
```

### Compilation

```sh
mkdir -p build && cd build
cmake ..
make -j$(nproc)
```

### Execution

```sh
./bin/executable
```

## Descriptions de la structure des fichiers

Le dossier [bin](./bin) contient les fichiers executables.

Le dossier [obj](./obj) contient les fichiers compilés.

Le dossier [src](./src) contient les fichiers .c et .h.

[Makefile](./Makefile) est le fichier pour la création automatique.