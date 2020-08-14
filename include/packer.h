//
// Created by antoine on 12/08/2020.
//

#ifndef ELF_PACKER_INCLUDE_PACKER_H_
#define ELF_PACKER_INCLUDE_PACKER_H_

#include <elf.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct  elf_file_s {
  void          *file;
  Elf64_Ehdr    *ehdr;
  Elf64_Phdr    *phdr;
  Elf64_Shdr    *shdr;
  uint8_t       **section_content;
}               elf_file_t;

elf_file_t *extract_and_copy_headers(elf_file_t *efile);

// ONLINE MAN: http://manpagesfr.free.fr/man/man5/elf.5.html

#endif //ELF_PACKER_INCLUDE_PACKER_H_
