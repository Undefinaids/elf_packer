//
// Created by antoine on 12/08/2020.
//

#ifndef ELF_PACKER_INCLUDE_PACKER_H_
#define ELF_PACKER_INCLUDE_PACKER_H_

#include <elf.h>

// Handle 64 and 32 bits elf ?

typedef struct  elf_file_s {
  void          *file;
  Elf64_Ehdr    *ehdr;
}               elf_file_t;


// union elf64_file elf32_file ?

// ONLINE MAN: http://manpagesfr.free.fr/man/man5/elf.5.html

//Objectif -> Rajouter une section vide

#endif //ELF_PACKER_INCLUDE_PACKER_H_
