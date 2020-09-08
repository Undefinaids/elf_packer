//
// Created by antoine on 14/08/2020.
//

#include "packer.h"

ssize_t padding(int fd, ssize_t length) {
  const char *c = 0;
  ssize_t written = 0;

  for (ssize_t i = 0; i < length; ++i, ++written)
    write(fd, &c, 1);
  return (written);
}

int dump(elf_file_t *efile, const char *name) {
  int fd;
  ssize_t wchar = 0;

  if ((fd = open(name, O_CREAT | O_RDWR, 0775)) == -1)
    return (-1);
  wchar += write(fd, efile->ehdr, sizeof(Elf64_Ehdr));
  if (wchar < efile->ehdr->e_phoff)
    wchar += padding(fd, efile->ehdr->e_phoff - wchar);
  wchar += write(fd, efile->phdr, efile->ehdr->e_phnum * sizeof(Elf64_Phdr));
  for (size_t i = 0; i < efile->ehdr->e_shnum; ++i) {
    if (efile->shdr[i].sh_type != SHT_NOBITS) {
      if (wchar < efile->shdr[i].sh_offset)
        wchar += padding(fd, efile->shdr[i].sh_offset - wchar);
      wchar += write(fd, efile->section_content[i], efile->shdr[i].sh_size);
    }
  }
  if (wchar < efile->ehdr->e_shoff)
    wchar += padding(fd, efile->ehdr->e_shoff - wchar);
  wchar += write(fd, efile->shdr, efile->ehdr->e_shnum * sizeof(Elf64_Ehdr));
  close(fd);
  return (0);
}
