//
// Created by antoine on 14/08/2020.
//

#include "packer.h"

static Elf64_Shdr *init_section(void) {
  Elf64_Shdr *section;

  if ((section = malloc(sizeof(Elf64_Shdr))) == NULL)
    return (NULL);
  section->sh_name = 0;
  section->sh_type = SHT_PROGBITS;
  section->sh_flags = SHF_EXECINSTR | SHF_ALLOC;
  section->sh_addr = 0;
  section->sh_offset = 0;
  section->sh_size = 0;
  section->sh_link = 0;
  section->sh_info = 0;
  section->sh_addralign = 16;
  section->sh_entsize = 0;
  return (section);
}

match_t *find_good_phdr(elf_file_t *efile) {
  match_t *segments = NULL;
  match_t *elem;
  match_t *it;
  Elf64_Phdr phdr;

  for (int i = 0; i < efile->ehdr->e_phnum; ++i) {
    phdr = efile->phdr[i];
    if (phdr.p_type == PT_LOAD && phdr.p_flags & PF_X) {
      if ((elem = malloc(sizeof(match_t))) == NULL)
        return (NULL);
      elem->index = i;
      elem->next = NULL;
      if (!segments)
        segments = elem;
      else {
        for (it = segments; it->next != NULL; it = it->next);
        it->next = elem;
      }
    }
  }
  return (segments);
}

int find_last_section_of_segment(elf_file_t *efile, int phdr_id) {
  int result = -1;

  Elf64_Phdr phdr = efile->phdr[phdr_id];
  for (int i = 0; i < efile->ehdr->e_shnum; ++i) {
    if (efile->shdr[i].sh_offset >= phdr.p_offset &&
    efile->shdr[i].sh_offset + efile->shdr[i].sh_size <= phdr.p_offset + phdr.p_filesz) {
      if (result == -1 || efile->shdr[result].sh_offset < efile->shdr[i].sh_offset)
        result = i;
    }
  }
  printf("LA DERNIERE SECTION DU SEGMENT EST %d\n", result);
  return (result);
}

Elf64_Shdr *add_new_section(elf_file_t *efile) {
  Elf64_Shdr *new_section;
  match_t *matching_segments = find_good_phdr(efile);
  int ephdr_id = -1;
  int last_section = -1;
  uint8_t *stub_fct;

  if ((new_section = init_section()) == NULL)
    return (NULL);
  if ((efile->shdr = realloc(efile->shdr, (efile->ehdr->e_shnum + 1) * sizeof(Elf64_Shdr))) == NULL)
    return (NULL);
  if ((efile->section_content = realloc(efile->section_content, (efile->ehdr->e_shnum + 1) * sizeof(uint8_t *))) == NULL)
	return (NULL);

  for (match_t *match = matching_segments; match != NULL; match = match->next) {
    if (match->index == efile->ehdr->e_phnum - 1 &&
    efile->ehdr->e_shoff - (efile->phdr[match->index].p_offset + efile->phdr[match->index].p_filesz) < stub_size)
      continue;
    if (efile->phdr[match->index + 1].p_offset - (efile->phdr[match->index].p_offset + efile->phdr[match->index].p_filesz) < stub_size)
      continue;
    printf("Taille disponible %ld\n", efile->phdr[match->index + 1].p_offset - (efile->phdr[match->index].p_offset + efile->phdr[match->index].p_filesz));
    printf("Le candidat %d possède assez de place :)\n", match->index);
    ephdr_id = match->index;
    last_section = find_last_section_of_segment(efile, ephdr_id);
  }
  printf("Le segment selectionné est %d et sa dernière section est %d\n", ephdr_id, last_section);
  // Insérer la section et le stub
  // Incrémenter les valeurs p_memsz et p_filesz du segment
  memmove(efile->shdr + last_section + 2, efile->shdr + last_section + 1, sizeof(Elf64_Shdr) * (efile->ehdr->e_shnum - last_section - 1));
  memmove(efile->section_content + last_section + 2, efile->section_content + last_section + 1, sizeof(uint8_t *) * (efile->ehdr->e_shnum - last_section - 1));
  efile->section_content[last_section + 1] = 0;

  new_section->sh_offset = efile->shdr[last_section].sh_offset + efile->shdr[last_section].sh_size;
  new_section->sh_addr = efile->phdr[ephdr_id].p_vaddr + efile->phdr[ephdr_id].p_memsz;
  new_section->sh_size = stub_size;

  my_memcpy((uint8_t *) efile->shdr + ((last_section + 1) * sizeof(Elf64_Shdr)), new_section, sizeof(Elf64_Shdr));
  efile->ehdr->e_shnum += 1;
  if (efile->ehdr->e_shstrndx > last_section)
    efile->ehdr->e_shstrndx += 1;
  efile->phdr[ephdr_id].p_memsz += stub_size;
  efile->phdr[ephdr_id].p_filesz += stub_size;

  stub_fct = malloc(sizeof(uint8_t) * stub_size);
  my_memcpy(stub_fct, (void *) stub, stub_size);
  efile->section_content[last_section + 1] = stub_fct;

//  //Test to switch entry point
  efile->ehdr->e_entry = new_section->sh_addr;
//
//  //Update symbol table
//  Elf64_Sym *table;
//  unsigned long nb_sym = 0;
//  for (int i = 0; i < efile->ehdr->e_shnum; ++i) {
//    if (efile->shdr[i].sh_type == SHT_SYMTAB) {
//      table = (Elf64_Sym *) efile->section_content[i];
//      nb_sym = efile->shdr[i].sh_size / efile->shdr[i].sh_entsize;
//    }
//  }
//  printf("nb_sym = %ld\n", nb_sym);
//  for (unsigned long i = 0; i < nb_sym; ++i) {
//    if (table[i].st_info == STT_SECTION && table[i].st_shndx > last_section)
//      table[i].st_shndx += 1;
//      //printf("%hu\n", table[i].st_shndx);
//  }
  free(new_section);
  return (efile->shdr + (efile->ehdr->e_shnum - 1) * sizeof(Elf64_Shdr));
}