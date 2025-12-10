#include "lib_tar.h"

/**
 * Checks whether the archive is valid.
 *
 * Each non-null header of a valid archive has:
 *  - a magic value of "ustar" and a null,
 *  - a version value of "00" and no null,
 *  - a correct checksum
 *
 * @param tar_fd A file descriptor pointing to the start of a file supposed to contain a tar archive.
 *
 * @return a zero or positive value if the archive is valid, representing the number of non-null headers in the archive,
 *         -1 if the archive contains a header with an invalid magic value,
 *         -2 if the archive contains a header with an invalid version value,
 *         -3 if the archive contains a header with an invalid checksum value
 */
int check_archive(int tar_fd) {
    // TODO
    int numberHeader = 0;
    for (;;){
        unsigned char buffer[512];
        int len_block = read(tar_fd, buffer, 512);
        tar_header_t *header = (tar_header_t *)buffer;
        bool blockIsEmpty = true;
        if (len_block == 512){
            for (int i = 0; i < 512; i++){ // on boucle sur les 512 octets du bloc
                if (buffer[i] != 0){
                    blockIsEmpty = false;
                }
            }
            if (blockIsEmpty){ // Si le bloc est vide, alors on est à la fin
                return numberHeader;
            } else { // Si c'est pas vide alors on regarde si c'est un header complet ou non 
                // Vérification de magic
                for(int i=0; i < TMAGLEN; i++){
                    if (header->magic[i] != TMAGIC[i]){
                        return(-1);
                    }
                }
                // Vérification de version
                for(int i=0; i < TVERSLEN; i++){
                    if (header->version[i] != TVERSION[i]){
                        return(-2);
                    }
                }
                
                // Vérification du checksum buffer[148 => 148 + 8]
                int expectedSum = TAR_INT(header->chksum);
                int sommeBuffer = 0;
                for (int i = 0; i < 512; i++){
                    if (148 <= i && i <= 155){
                        sommeBuffer += ' ';
                    } else {
                        sommeBuffer += buffer[i];
                    }
                }
                if(expectedSum != sommeBuffer){
                    return(-3);
                }
            }
            numberHeader ++;
        }
        else {
            return numberHeader;
        }
    }
    return numberHeader;
}

/**
 * Checks whether an entry exists in the archive.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive,
 *         any other value otherwise.
 */
int exists(int tar_fd, char *path) {
    // TODO
    for(;;){
        unsigned char buffer[512];
        int len_block = read(tar_fd, buffer, 512);
        tar_header_t *header = (tar_header_t *)buffer;
        bool blockIsEmpty = true;
        if (len_block == 512){
            for (int i = 0; i < 512; i++){ // on boucle sur les 512 octets du bloc
                if (buffer[i] != 0){
                    blockIsEmpty = false;
                }
            }
            if (blockIsEmpty){ // Si le bloc est vide, alors on est à la fin
                return 0;
            } else {
                // Le header n'est pas vide, on peut donc comparer si il est égal au path
                int isEqual = 1;
                int lenPath = strlen(path);
                int lenName = strlen(header->name);
                if (lenName == lenPath){
                    for (int i = 0; i < lenName; i++){
                        if (header->name[i] != path[i]){
                            isEqual = 0;
                            break;
                        }
                    }
                } else {
                    isEqual = 0;
                }
                // Si on a passé tout le for et que isEqual = 1, alors on a un match
                if (isEqual == 1){
                    return 1;
                }
                // Si non alors on continue
            }
        } else {
            return 0;
        }
    }
    return 0;
}

/**
 * Checks whether an entry exists in the archive and is a directory.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive or the entry is not a directory,
 *         any other value otherwise.
 */
int is_dir(int tar_fd, char *path) {
    // TODO
       for(;;){
        unsigned char buffer[512];
        int len_block = read(tar_fd, buffer, 512);
        tar_header_t *header = (tar_header_t *)buffer;
        bool blockIsEmpty = true;
        if (len_block == 512){
            for (int i = 0; i < 512; i++){ // on boucle sur les 512 octets du bloc
                if (buffer[i] != 0){
                    blockIsEmpty = false;
                }
            }
            if (blockIsEmpty){ // Si le bloc est vide, alors on est à la fin
                return 0;
            } else {
                // Le header n'est pas vide, on peut donc comparer si il est égal au path
                int isEqual = 1;
                int lenPath = strlen(path);
                int lenName = strlen(header->name);
                if (lenName == lenPath){
                    for (int i = 0; i < lenName; i++){
                        if (header->name[i] != path[i]){
                            isEqual = 0;
                            break;
                        }
                    }
                } else {
                    isEqual = 0;
                }
                // Si on a passé tout le for et que isEqual = 1, alors on a un match
                if (isEqual == 1){
                    if (header->typeflag == DIRTYPE){
                        return 1;
                    }
                }
                // Si non alors on continue
            }
        } else {
            return 0;
        }
    }
    return 0;
}

/**
 * Checks whether an entry exists in the archive and is a file.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive or the entry is not a file,
 *         any other value otherwise.
 */
int is_file(int tar_fd, char *path) {
    // TODO
    for(;;){
        unsigned char buffer[512];
        int len_block = read(tar_fd, buffer, 512);
        tar_header_t *header = (tar_header_t *)buffer;
        bool blockIsEmpty = true;
        if (len_block == 512){
            for (int i = 0; i < 512; i++){ // on boucle sur les 512 octets du bloc
                if (buffer[i] != 0){
                    blockIsEmpty = false;
                }
            }
            if (blockIsEmpty){ // Si le bloc est vide, alors on est à la fin
                return 0;
            } else {
                // Le header n'est pas vide, on peut donc comparer si il est égal au path
                int isEqual = 1;
                int lenPath = strlen(path);
                int lenName = strlen(header->name);
                if (lenName == lenPath){
                    for (int i = 0; i < lenName; i++){
                        if (header->name[i] != path[i]){
                            isEqual = 0;
                            break;
                        }
                    }
                } else {
                    isEqual = 0;
                }
                // Si on a passé tout le for et que isEqual = 1, alors on a un match
                if (isEqual == 1){
                    if (header->typeflag == REGTYPE){
                        return 1;
                    }
                }
                // Si non alors on continue
            }
        } else {
            return 0;
        }
    }
    return 0;
}

/**
 * Checks whether an entry exists in the archive and is a symlink.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 * @return zero if no entry at the given path exists in the archive or the entry is not symlink,
 *         any other value otherwise.
 */
int is_symlink(int tar_fd, char *path) {
    // TODO// TODO
    for(;;){
        unsigned char buffer[512];
        int len_block = read(tar_fd, buffer, 512);
        tar_header_t *header = (tar_header_t *)buffer;
        bool blockIsEmpty = true;
        if (len_block == 512){
            for (int i = 0; i < 512; i++){ // on boucle sur les 512 octets du bloc
                if (buffer[i] != 0){
                    blockIsEmpty = false;
                }
            }
            if (blockIsEmpty){ // Si le bloc est vide, alors on est à la fin
                return 0;
            } else {
                // Le header n'est pas vide, on peut donc comparer si il est égal au path
                int isEqual = 1;
                int lenPath = strlen(path);
                int lenName = strlen(header->name);
                if (lenName == lenPath){
                    for (int i = 0; i < lenName; i++){
                        if (header->name[i] != path[i]){
                            isEqual = 0;
                            break;
                        }
                    }
                } else {
                    isEqual = 0;
                }
                // Si on a passé tout le for et que isEqual = 1, alors on a un match
                if (isEqual == 1){
                    if (header->typeflag == SYMTYPE){
                        return 1;
                    }
                }
                // Si non alors on continue
            }
        } else {
            return 0;
        }
    }
    return 0;
}

/**
 * Lists the entries at a given path in the archive.
 * list() does *not* recurse into the directories listed at the given path.
 * If the path is NULL, it lists the entries at the root of the archive.
 *
 * Example:
 *  dir/          list(..., "dir/", ...) lists "dir/a", "dir/b", "dir/c/" and "dir/e/"
 *   ├── a
 *   ├── b
 *   ├── c/
 *   │   └── d
 *   └── e/
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive. If the entry is a symlink, it must be resolved to its linked-to entry.
 * @param entries An array of char arrays, each one is long enough to contain a tar entry path.
 * @param no_entries An in-out argument.
 *                   The caller set it to the number of entries in `entries`.
 *                   The callee set it to the number of entries listed.
 *
 * @return zero if no directory at the given path exists in the archive,
 *         1 in case of success,
 *         -1 in case of error.
 */
int list(int tar_fd, char *path, char **entries, size_t *no_entries) {
    // TODO
    return 0;
}

/**
 * Adds a file at the end of the archive, at the archive's root level.
 * The archive's metadata must be updated accordingly.
 * For the file header, only the name, size, typeflag, magic value (to "ustar"), version value (to "00") and checksum fields need to be correctly set.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param filename The name of the file to add. If an entry already exists with the same name, the file is not written, and the function returns -1.
 * @param src A source buffer containing the file content to add.
 * @param len The length of the source buffer.
 *
 * @return 0 if the file was added successfully,
 *         -1 if the archive already contains an entry at the given path,
 *         -2 if an error occurred
 */
int add_file(int tar_fd, char *filename, uint8_t *src, size_t len) {
    // TODO
    return 0;
}
