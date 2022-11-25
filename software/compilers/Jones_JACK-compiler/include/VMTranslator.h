#ifndef VMTRANSLATOR_H
#define VMTRANSLATOR_H

#define NO_ERR 0
#define MAX_VM_COMMAND_LENGTH 128
#define MAX_FOLDER_NAME_LEN 256
#define MAX_FILE_NAME_LEN 128

#ifdef DEBUG
#	define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#	define DEBUG_PRINTF(...)
#endif

char type_of_slash_used_in_argv1(char * argv1);
void extract_project_folder_and_asm_filename_from_argv1(char * argv1, char * project_folder, char * asm_filename);
void extract_vm_files_from_directory(DIR * folder, char * project_folder, char *** list_of_vm_files, int * num_vm_files);
void set_codeWriter_vm_filename(char * vm_full);

#endif // VMTRANSLATOR_H