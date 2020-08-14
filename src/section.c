//
// Created by antoine on 14/08/2020.
//

#include "packer.h"

static Elf64_Shdr *init_section(void) {
  Elf64_Shdr *section;

  if ((section = malloc(sizeof(Elf64_Shdr))) == NULL)
    return (NULL);
  section->sh_name = 0;
  section->sh_type = SHT_NOBITS;
  section->sh_flags = 0;
  section->sh_addr = 0;
  section->sh_offset = 0;
  section->sh_size = 0;
  section->sh_link = 0;
  section->sh_info = 0;
  section->sh_addralign = 0;
  section->sh_entsize = 0;
  return (section);
}

Elf64_Shdr *add_new_section(elf_file_t *efile) {
  Elf64_Shdr *new_section;

  if ((new_section = init_section()) == NULL)
    return (NULL);
  if (realloc(efile->shdr, (efile->ehdr->e_shnum + 1) * sizeof(Elf64_Shdr)) == NULL)
    return (NULL);

  // PROBLEM WITH THIS LINE (FIX IT)
  my_memcpy(efile->shdr + (efile->ehdr->e_shnum * sizeof(Elf64_Shdr)), new_section, sizeof(Elf64_Shdr));
  //free(new_section);
  efile->ehdr->e_shnum += 1;
  return (efile->shdr + (efile->ehdr->e_shnum - 1) * sizeof(Elf64_Shdr));
}