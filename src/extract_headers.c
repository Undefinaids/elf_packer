//
// Created by antoine on 14/08/2020.
//

#include "packer.h"

static void *my_memcpy(void *dest, const void *src, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    ((uint8_t *)dest)[i] = ((uint8_t *)src)[i];
  }
  return (dest);
}

static Elf64_Ehdr *ex_cp_ehdr(void *file) {
  Elf64_Ehdr *ehdr;

  if ((ehdr = malloc(sizeof(Elf64_Ehdr))) == NULL)
    return (NULL);
  my_memcpy(ehdr, file, sizeof(Elf64_Ehdr));
  return (ehdr);
}

static Elf64_Phdr *ex_cp_phdr(void *file) {
  Elf64_Phdr *phdr;
  Elf64_Ehdr *ehdr;

  ehdr = (Elf64_Ehdr *) file;
  if ((phdr = malloc(sizeof(Elf64_Phdr) * ehdr->e_phnum)) == NULL)
	return (NULL);
  my_memcpy(phdr, ((uint8_t *) file) + ehdr->e_phoff, sizeof(Elf64_Phdr) * ehdr->e_phnum);
  return (phdr);
}

static Elf64_Shdr *ex_cp_shdr(void *file) {
  Elf64_Shdr *shdr;
  Elf64_Ehdr *ehdr;

  ehdr = (Elf64_Ehdr *) file;
  if ((shdr = malloc(sizeof(Elf64_Shdr) * ehdr->e_shnum)) == NULL)
	return (NULL);
  my_memcpy(shdr, ((uint8_t *) file) + ehdr->e_shoff, sizeof(Elf64_Shdr) * ehdr->e_shnum);
  return (shdr);
}

static uint8_t **ex_cp_section_content(void *file, Elf64_Ehdr *ehdr, Elf64_Shdr *shdr) {
  uint8_t **section_content;

  if ((section_content = malloc(sizeof(uint8_t *) * ehdr->e_shnum)) == NULL)
    return (NULL);
  for (int i = 0; i < ehdr->e_shnum; ++i) {
    if ((section_content[i] = malloc(sizeof(uint8_t) * shdr[i].sh_size)) == NULL)
      return (NULL);
    my_memcpy(section_content[i], ((uint8_t *) file) + shdr[i].sh_offset, shdr[i].sh_size);
  }
  return (section_content);
}

elf_file_t *extract_and_copy_headers(elf_file_t *efile) {
  if (efile->file == NULL)
    return (NULL);
  efile->ehdr = ex_cp_ehdr(efile->file);
  efile->phdr = ex_cp_phdr(efile->file);
  efile->shdr = ex_cp_shdr(efile->file);
  efile->section_content = ex_cp_section_content(efile->file, efile->ehdr, efile->shdr);
  return (efile);
}