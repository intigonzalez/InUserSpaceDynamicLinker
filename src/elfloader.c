#include "elfloader.h"


const char* ElfTypes[]= {
        "NONE", "REL", "EXEC",
        "DYN", "CORE"
};

const char* SymbolTypes[]= {
        "NOTYPE", "OBJECT", "FUNC",
        "SECTION", "FILE", "LOPROC", "HIPROC"
};

const char* SymbolBindingTypes[]= {
        "LOCAL", "GLOBAL", "WEAK"
};

char* getStrFromSection(
        const FileImpl* file,
        Elf32_Off off_str_entry,
        Elf32_Word idx
)
{
    Elf32_Shdr* section;
    char* buf;
    section = (Elf32_Shdr*) read_file(file, off_str_entry, sizeof(Elf32_Shdr));
    buf = (char*)read_file(file, section->sh_offset + idx, 1000);
    return strdup(buf);
}

char*
getSectionName(
        const FileImpl* file,
        Elf32_Off off_sections,
        Elf32_Half entry_size,
        Elf32_Half sectionIdx,
        Elf32_Off off_str_entry
)
{
    Elf32_Shdr* section;
    if (sectionIdx == SHN_ABS)
        return strdup("ABS");
    section = (Elf32_Shdr*)read_file(file, entry_size*sectionIdx + off_sections, sizeof(Elf32_Shdr));
    char* s = getStrFromSection(file, off_str_entry, section->sh_name);
}
