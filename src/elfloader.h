#ifndef __ELF_LOADER__
#define __ELF_LOADER__

#include <stdint.h>
#include <elf.h>

#include "FileManagement.h"

extern const char* ElfTypes[];

extern const char* SymbolTypes[];

extern const char* SymbolBindingTypes[];

char* getStrFromSection(
        const FileImpl* file,
        Elf32_Off off_str_entry,
        Elf32_Word idx
);

char*
getSectionName(
        const FileImpl* file,
        Elf32_Off off_sections,
        Elf32_Half entry_size,
        Elf32_Half sectionIdx,
        Elf32_Off off_str_entry
);


#endif