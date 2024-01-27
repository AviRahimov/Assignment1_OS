# Assignment 1: Operating Systems

## Task 1: [Task 1 Title]
### Overview
[Short description of Task 1]

### Instructions
1. [Step 1 for Task 1]
2. [Step 2 for Task 1]
3. [Further steps as needed]

## Task 2: [Task 2 Title]
### Overview
[Short description of Task 2]

### Instructions
1. [Step 1 for Task 2]
2. [Step 2 for Task 2]
3. [Further steps as needed]

## Task 3: [Task 3 Title]
### Overview
[Short description of Task 3]

### Instructions
1. [Step 1 for Task 3]
2. [Step 2 for Task 3]
3. [Further steps as needed]


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

