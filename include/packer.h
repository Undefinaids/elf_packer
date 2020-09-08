//
// Created by antoine on 12/08/2020.
//

#ifndef ELF_PACKER_INCLUDE_PACKER_H_
#define ELF_PACKER_INCLUDE_PACKER_H_

#include <elf.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

extern int stub();
extern uint64_t stub_size;

typedef struct    match_s {
  int             index;
  struct match_s  *next;
}                 match_t;

typedef struct  elf_file_s {
  void          *file;
  Elf64_Ehdr    *ehdr;
  Elf64_Phdr    *phdr;
  Elf64_Shdr    *shdr;
  uint8_t       **section_content;
}               elf_file_t;

elf_file_t *extract_and_copy_headers(elf_file_t *efile);
void *my_memcpy(void *dest, const void *src, size_t n);
Elf64_Shdr *add_new_section(elf_file_t *efile);
int dump(elf_file_t *efile, const char *name);

//utils
void print_ephdr(elf_file_t *file);

// ONLINE MAN: http://manpagesfr.free.fr/man/man5/elf.5.html

#endif //ELF_PACKER_INCLUDE_PACKER_H_
