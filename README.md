# Blockchain

## Configuration

### Dependencies

```sh
sudo apt install libssl-dev  # Debian-based systems
```

### Compilation

```sh
mkdir -p build && cd build
cmake ..
make -j$(nproc)
```

Thr generated binary is stored in `./build/blockchain`

### Execution

```sh
./build/blockchain
```
