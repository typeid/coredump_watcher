# coredump_watcher

## Build

```bash
mkdir build
cd build 
conan install .. --build=missing
cmake ..
make
```

## Tests

### Unit

```bash
cd build
make test
```

### Integration 

```bash
cd ./tests/integration
robot ./
```