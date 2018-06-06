#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

typedef struct file_info_c {

  char name[MAX_FILE_NAME];
	uint64_t isDir;
  char sons[MAX_SONS][MAX_FILE_NAME];
  uint64_t sonsAmount
}file_info;

/* Copia la información de un file a la estructura pública file_info.
** Dicha estructura funciona como interfaz entre Kernel y Userland para leer
** la información de un file */
void get_file_info (file_info * fi, file * thisfile);

#endif
