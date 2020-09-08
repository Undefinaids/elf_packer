
#include "packer.h"

void *retrieve_elf_file(const char *name) {
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
  if (s.st_size != ehdr->e_shoff + ehdr->e_shentsize * ehdr->e_shnum)
    return (NULL);
  return ((void *) ehdr);
}

elf_file_t *init_efile(void) {
  elf_file_t *efile;

  if ((efile = malloc(sizeof(elf_file_t))) == NULL)
    return (NULL);
  efile->file = NULL;
  efile->ehdr = NULL;
  efile->phdr = NULL;
  efile->shdr = NULL;
  efile->section_content = NULL;
  return (efile);
}

int main(int ac, char **av) {
  elf_file_t *efile = init_efile();

  efile->file = retrieve_elf_file("test");
  extract_and_copy_headers(efile);

  add_new_section(efile);

  printf("Stub size %ld\n", stub_size);
  dump(efile, "testpacked");
  return (0);
}
