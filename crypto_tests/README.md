# TF - Cryptography Tests
This is the final project for the class "Teste e Confiabilidade de Software" for the Computer Engineering course of PUCRS.

## Running the program and its tests
We've created a *makefile* on the root directory that runs the program and all types of tests. But firstly, to correctly run all the programs you'll need to manually install some packages.

```bash
sudo apt-get install lcov
sudo apt-get install valgrind
sudo apt-get install cppcheck
```
All the commands should be made on the project root directory, not in the crypto directory.

### Running the code
We've changed the crypto main program a little bit, so it could run the Unit tests without the need to change the type progamatically, but passing as an argument. This way if you want to run the program by itself run this command listed below with the type of cryptography of your choosing.

Cryptography  | Value
------------- | -------------
XTEA          | 0
AES-128       | 1
AES-192       | 2
AES-256       | 3
BLOWFISH-128  | 4 
BLOWFISH-192  | 5 
BLOWFISH-256  | 6 

```bash
make run_code <typeCryptography>
```

### Running Unity tests
To run the Unit tests just run the command as below:

```bash
make run_unity
```

### Running GCOV
To run the GCOV tests just run the command as below:

```bash
make run_gcov
```

### Running valgrind
To run the valgrind tests just run the command as below:

```bash
make run_valgrind
```

### Running sanitizer
To run the sanitizer just run the command as below:

```bash
make run_sanitizer
```

### Running all
To run all tests just run the command as below:

```bash
make run_all <typeCryptography>
```
