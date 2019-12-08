# RLZ
Relative Lempel Ziv on Integers

### Download
````
git clone https://github.com/adriangbrandon/rlz.git
````
### Compile
````
bash compile.sh
````

### Instructions
Inside *build*, there are two executables:

- *random_integers* builds a file with *size* integers which can adquire a random value between 1 and *alphabet_size*.
````
./random_integers <file_name> <size> <alphabet_size>
````

- *rlz* builds the RLZ on integers of a given file. The reference takes *reference_size* MB and it is built by using an uniform sampling of blocks of size *block_size*.
````
./rlz <file_name> <reference_size> <block_size>
````


