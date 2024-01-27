# Assignment 1: Operating Systems
**By Avi rahimov, Jonathan booritsky and Avichay mazin.**

## Requirements
* Linux machine (Ubuntu 22.04 LTS preferable)
* GNU C Compiler
* gdb debugger
* Make
  
 ## Building
```
# Cloning the repo to local machine.
https://github.com/AviRahimov/Assignment1_OS.git
```

## Task 1: Practice working with a debugger
### Overview
This task focuses on honing debugging skills in the C programming language. We are assigned a specific bug based on the check digit of our ID card. 
The bugs involve intentional program crashes to simulate real-world scenarios:
- Bug A: Creates a program that crashes due to division by zero.
- Bug B: Generates a program that crashes due to illegal access to memory (memory not initialized).
- Bug C: Develops a program that crashes due to infinite recursion.
### We solve only Bugs B and C because our check digit of our ID is 4 and 7.

### Instructions
1. Navigate to task1.c
2.   Use the commands shown in [Download PDF Documentation](https://github.com/AviRahimov/Assignment1_OS/blob/main/Assignment1_OS.pdf) , pages 1-7 is the solution for task 1.
The commands for bug "C" is in pages 1-3, and the commands for bug "B" is in pages 4-7.


## Task 2: Working with a library
### Overview
In this task, we asked for writing a code that the input is three side lengths of a triangle provided by the user.
The program checks whether these side lengths form a Pythagorean triangle, and if so, it calculates and displays the corresponding angles in radians 
- Input:
Run the program and enter three side lengths when prompted.
- Validation:
The program checks if the input side lengths form a Pythagorean triangle.
- Output:
If valid, the program prints the angles in radians.
If invalid, an "Error" message is displayed, and the program exits.

### Instructions
1. Navigate to task2.c
2. in terminal run: 'gcc -o task2 task2.c'
3. run: './task2'


## Task 3: Writing a library
### Overview
In this part of the assignment we asked for writing a library.
This library provides a polyalphabetic encoder with the following functionalities:
1. Create Encoder:
  - Function: void * createCodec (char key[62]);
    -  Description: Generates a polyalphabetic encoder based on a 62-character key. The key specifies the mapping of lowercase letters, uppercase letters, and digits.
  If the key is invalid (e.g., letters are mapped to the same character), the function returns null.
2. Encode Text:
 - Function: int encode(char * textin, char * textout, int len, void * codec);
     - Description: Encrypts the input text using the provided codec. Returns the number of encoded bits in case of success; -1 if there's a failure (e.g., null input).
3. Decode Text:
  - Function: int decode(char * textin, char * textout, int len, void * codec);
    - Description: Decrypts the input text using the provided codec. Returns the number of decoded bits in case of success; -1 if there's a failure (e.g., null input).
4. Free Encoder:
  - Function: void freecodec(void * codec);
    - Description: Frees the memory allocated for the codec. 

### Instructions



## Task 4: File Collection, Compression, and Encryption
### Overview
This task demonstrates file compression and encryption using Unix commands and pipelines. It involves building a program, creating a GPG key, compressing and encrypting files, and then decompressing and decrypting them.

### Instructions
#### 1. Building the Program
- Navigate to the `Q4` directory.
- Execute `make all` to build the program.

#### 2. Create a GPG Key
- Run `gpg --gen-key` to generate a new GPG key.
- Follow the on-screen instructions to complete the key creation.

#### 3. Store the Passphrase
- Enter the passphrase used for the GPG key into `passphrase.txt`.
  ![Store the Passphrase](https://github.com/AviRahimov/Assignment1_OS/assets/73108322/0c7f7e68-5fe8-4edd-97a9-cd51372af07f)

#### 4. Compress and Encrypt Files
- Use the command `./myzip <file1> <file2>` to compress and encrypt the specified files.
- The output file will be named `output`.
  ![Compress and Encrypt Files](https://github.com/AviRahimov/Assignment1_OS/assets/73108322/db10c772-61d7-48db-9177-c5fccac32e65)

#### 5. Decompress and Decrypt File
- Run `./myunzip <compressed file>` to decompress and decrypt the file.
  ![Decompress and Decrypt File](https://github.com/AviRahimov/Assignment1_OS/assets/73108322/7f3c64a8-20e3-44f9-b364-2f6435ccee7e)

### Implementation Notes
- **Pipes and Child Processes**:
  - The implementation uses pipes to enable a pipeline for operations between `gpg`, `tar`, and `gzip` commands.
    ![Pipes and Child Processes](https://github.com/AviRahimov/Assignment1_OS/assets/73108322/848c4cd7-65c7-4835-8a89-e57ddff4ce12)
  - Three child processes are used, one for each command (`tar`, `gzip`, `gpg`).
  - In each child process, the file descriptor for the read-end of the pipe is duplicated (`dup2`) to the standard input (stdin), and the standard output (stdout) to the write-end of the pipe.
    ![image](https://github.com/AviRahimov/Assignment1_OS/assets/73108322/ef7e2bd1-af4f-4577-a436-71443d9d19cb)

