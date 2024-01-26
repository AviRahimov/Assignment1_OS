
// This function receives a pointer to a tar file, and filenames.
// The function should add the files to the tar file.
int create_tar_file(const char * tarfile, const char **filenames, const int num_files);

int compress (const char * tarfile, const char * gzip);

int encrypt_file (const char * compressed_tarfile, const char * gpg);
