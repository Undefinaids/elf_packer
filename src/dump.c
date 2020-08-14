//
// Created by antoine on 14/08/2020.
//

#include "packer.h"

int dump(elf_file_t *efile, const char *name) {
  int fd;
  ssize_t wchar = 0;
  static const char	c = 0;

  if ((fd = open(name, O_CREAT | O_RDWR, 0775)) == -1)
    return (-1);
  wchar += write(fd, efile->ehdr, sizeof(Elf64_Ehdr));
  if (wchar < efile->ehdr->e_phoff)
    wchar += write(fd, &c, efile->ehdr->e_phoff - wchar);
  wchar += write(fd, efile->phdr, efile->ehdr->e_phnum * sizeof(Elf64_Phdr));
  for (size_t i = 0; i < efile->ehdr->e_shnum; ++i) {
    if (efile->shdr[i].sh_type != SHT_NOBITS) {
      if (wchar < efile->shdr[i].sh_offset)
	    wchar += write(fd, &c, efile->shdr[i].sh_offset - wchar);
      wchar += write(fd, efile->section_content[i], efile->shdr[i].sh_size);
    }
  }
  wchar += write(fd, &c, efile->ehdr->e_shoff - wchar);
  wchar += write(fd, efile->shdr, efile->ehdr->e_shnum * sizeof(Elf64_Ehdr));
  close(fd);
  return (0);
}
