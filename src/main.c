#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "packer.h"

Elf64_Ehdr *retrieve_elf_file(const char *name) {
  int fd;
  struct stat s;
  Elf64_Ehdr *ehdr;

  fd = open(name, O_RDONLY);
  if (fd == -1)
    return (NULL);
  if (fstat(fd, &s) == -1)
    return (NULL);
  if ((ehdr = mmap(0, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
    return (NULL);
  // As I only handle 64 bits elf file, it is necessary to check file integrity
  if (!(ehdr->e_ident[EI_MAG0] == ELFMAG0 && ehdr->e_ident[EI_MAG1] == ELFMAG1
  && ehdr->e_ident[EI_MAG2] == ELFMAG2 && ehdr->e_ident[EI_MAG3] == ELFMAG3)
  || ehdr->e_ident[EI_CLASS] != ELFCLASS64)
    return (NULL);
  return (ehdr);
}

Elf64_Shdr *retrieve_symbol_table(Elf64_Ehdr *ehdr) {
  // Maybe encapsulate this into a structure that contains all the information needed (such as nb. of symbol)
  return ((Elf64_Shdr *) ((char *) ehdr + ehdr->e_shoff));
}

int main(int ac, char **av) {
  Elf64_Ehdr *ehdr;
  Elf64_Shdr *shdr;
  Elf64_Shdr *sh_strtab;

  ehdr = retrieve_elf_file("test");
  shdr = retrieve_symbol_table(ehdr);

  printf("ehdr = %p\noffset = %lu\nshdr = %p\nresult = %p\n", ehdr, ehdr->e_shoff, shdr, (char *) ehdr + ehdr->e_shoff);

  sh_strtab = &shdr[ehdr->e_shstrndx];
  const char *const sh_strtab_p = (char *) ((char *) ehdr + sh_strtab->sh_offset);

  for (int i = 0; i < ehdr->e_shnum; i++) {
    printf("%d Section %s:\n", i, (sh_strtab_p + shdr[i].sh_name));
  }
  return 0;
}
