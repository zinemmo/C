# Warm Up Identifier
The project is formed by to directories, them being *Unity* and *identifier*. *Unity* is the directory responsible for mantaining the Unity test suite. *Identifier* is the directory that contains the identifier program and hist test cases.

## Running the program and its tests
We've created a *makefile* on the root directory that runs the program and all types of tests. But firstly, to correctly run all the programs you'll need to manually install some packages.

```bash
sudo apt-get install lcov
sudo apt-get install valgrind
sudo apt-get install cppcheck
```
All the commands should be made on the project root directory, not in the identifier directory.

### Running the code
We've changed the identifier program a little bit, so it could run the Unit tests without the user input at each one. This way if you want to run the program by itself run this command listed below with the identifier of your choosing.

```bash
make run_code <identifier>
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
make run_all
```

**OBSERVATION** After the run_all command, to run it again is advisable to delete the logs directory.
 
