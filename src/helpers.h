#include "ijvm.h"

/**
 * Converts 4 bytes to a word (32-bit integer) in big-endian.
 * @param bytes pointer to the bytestream
 * @param offset offeset to the first byte in the bytestream
 * @returns The converted word
 **/ 
int32_t bytes_to_int(uint8_t *bytes,int offset);

/**
 * Converts 2 bytes to a short integer (16-bit) in big-endian.
 * @param bytes pointer to the bytestream
 * @returns The converted short integer
 **/ 
short bytes_to_short(uint8_t *bytes);

/**
 * Converts 2 bytes to a unsigned short integer (16-bit) in big-endian.
 * @param bytes pointer to the bytestream
 * @returns The converted unsigned short integer
 **/ 
unsigned short bytes_to_ushort(uint8_t *bytes);

/**
 * Calculates the size of the contents of a file.
 * @param fp pointer to the file
 * @returns The size of the file
 **/
long get_file_size(FILE *fp);

/**
 * Sets the ijvm status.
 * @param status 1 ijvm is running,
 *               0 ijvm stopped
 **/
void set_ijvm_status(bool status);

/**
 * @return ijvm status: 1 ijvm is running,
 *                      0 ijvm stopped
 **/
bool get_ijvm_status(void);

/**
 * Prints a help message if binary file is not found.
 **/
void print_help(void);

/**
 * Finds and creates the right directory for the different OS 
 * in which the GUI application will store data. 
 * @returns The path of the appliction data directory
 **/
char* get_appData_path(void);

/**
 * @param filename The filename to be accessed
 * @returns The path of a specific file inside the appication data directory
 **/
char* get_appData_filepath(char* filename);

/**
 * Removes the white spaces from a string.
 * @param str the specified string
 **/
void remove_spaces(char* str);
