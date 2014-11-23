#include <stdio.h>
#include <elf.h>
#include <string.h>
#include <stdlib.h>

#include "FileManagement.h"
#include "Allocator.h"
#include "elfloader.h"
#include "MyHashMap.h"

#define DEBUG 0
#if DEBUG
#define PRINTF(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define PRINTF(fmt, ...) ;
#endif

typedef struct {
    void* addr;
} SectionMapping;

//char* MyName = "Inti Gonzalez-Herrera";
char MyName[100];
int numbers[] = {1,8,17,31};

void callingFunction(const char* function, void* addr)
{
    printf("Function %s at %p\n", function, addr);
    int (*f)(int n) = (int (*)(int n))addr;
    int r = f(5);
    printf("\t\t%s(5) = %d\n", function, r);
}

int main(int argc, char* argv[])
{
    Allocator* txtAllocator = createAllocator(4096);
    Allocator* dataAllocator = createAllocator(4096);
    Allocator* bssAllocator = createAllocator(4096);
    Allocator* rodataAllocator = createAllocator(4096);

    const char* file = argv[1];
    Elf32_Ehdr* header;

    strcpy(MyName, "Inti Yulien");

    hhandle functions = create_table();

    // table with all the global symbols
    hhandle GOT = create_table();
    h_add(GOT, "printf", &printf);
    h_add(GOT, "Name", MyName);
    h_add(GOT, "arr", numbers);

    FileImpl* thefile = open_file_read(file);

    if (thefile == NULL) {
        fprintf(stderr, "Couldn't open the file %s\n", file);
        exit(1);
    }

    /* read header */
    header = (Elf32_Ehdr*) read_file(thefile, 0, sizeof(Elf32_Ehdr));

    PRINTF("The class of the file is %s\n", ((header->e_ident[4]==1)?"32-bits":"64-bits"));

    Elf32_Half nb_sections = header->e_shnum;
    Elf32_Off off_sections = header->e_shoff;
    Elf32_Half sectionEntrySize = header->e_shentsize;
    Elf32_Off off_str_entry = off_sections + header->e_shstrndx*sectionEntrySize;

    PRINTF("The index for string section is %u\n", header->e_shstrndx);
    PRINTF("The type of the Elf File is %s\n", ElfTypes[header->e_type]);
    if (sectionEntrySize != sizeof(Elf32_Shdr)) {
        fprintf(stderr, "WARNING: Using wrong size to read sections from file");
        exit(2);
    }

    SectionMapping* sectionMapping = (SectionMapping*) calloc(nb_sections, sizeof(SectionMapping));

    // ALLOCATING SPACE FOR SECTIONS
    for (int iSec = 0 ; iSec < nb_sections ; iSec++ ) {
        Elf32_Shdr* section = read_file(thefile, sectionEntrySize*iSec + off_sections, sectionEntrySize);
        char* s = getStrFromSection(thefile, off_str_entry, section->sh_name);
        PRINTF("Section [%u] %s\n", iSec, s);

        if ((section->sh_flags & SHF_ALLOC)!= 0) {
            Allocator* alloc = txtAllocator;
            if (strstr(s, ".data") == s)
                alloc = dataAllocator;
            else if (strstr(s, ".rodata") == s)
                alloc = rodataAllocator;
            else if (strstr(s, ".bss") == s)
                alloc = bssAllocator;
            void* addr = reserveFrom(alloc, section->sh_size);
            sectionMapping[iSec].addr = addr;
            void* addrSrc = read_file(thefile, section->sh_offset, section->sh_size);
            memcpy(addr, addrSrc, section->sh_size);
            PRINTF("\tAllocating space in %p\n", addr);
        }
        free(s);
    }

    // RESOLVING REFERENCES TO FILE DEFINED  SYMBOLS
    for (int iSec = 0 ; iSec < nb_sections ; iSec++ ) {
        Elf32_Shdr* section = read_file(
                thefile,
                sectionEntrySize*iSec + off_sections,
                sectionEntrySize);
        if (section->sh_type == SHT_SYMTAB) {
            Elf32_Sym *syms = (Elf32_Sym *) read_file(
                    thefile, section->sh_offset, sizeof(Elf32_Sym));
            int idx = 0;
            while (idx < section->sh_size / sizeof(Elf32_Sym)) {
                char * sName = getStrFromSection(
                        thefile,
                        off_sections + section->sh_link*sectionEntrySize,
                        syms->st_name);
                const char* typeStr = SymbolTypes[ELF32_ST_TYPE(syms->st_info)];
                const char* bindingStr = SymbolBindingTypes[ELF32_ST_BIND(syms->st_info)];
                char* secName = getSectionName(
                        thefile,
                        off_sections, sectionEntrySize, syms->st_shndx, off_str_entry);
                PRINTF("\t\tSymbol %s, type:%s, bind:%s, section:%s, value:%u\n",
                        sName, typeStr,bindingStr, secName, syms->st_value);
                if (ELF32_ST_TYPE(syms->st_info) == STT_FUNC
                        && ELF32_ST_BIND(syms->st_info) == STB_GLOBAL) {
                    // calculate the symbol address
                    void *lookupSymbolAddr = sectionMapping[syms->st_shndx].addr + syms->st_value;
                    h_add(functions, strdup(sName), lookupSymbolAddr);
                    h_add(GOT, strdup(sName), lookupSymbolAddr);
                }
                free(sName);
                syms++;
                idx ++;
            }
        }
    }

    // relocating
    PRINTF("\nRelocating symbols\n");
    for (int iSec = 0; iSec < nb_sections; ++iSec) {
        Elf32_Shdr* section = (Elf32_Shdr*) read_file(
                thefile,
                sectionEntrySize*iSec + off_sections,
                sizeof(Elf32_Shdr));
        if (section->sh_type == SHT_REL) {
            Elf32_Word idxAssociatedSymbolTable = section->sh_link;
            Elf32_Word idxAppliesToSection = section->sh_info;
            const char* associatedSymbolTable = getSectionName(
                    thefile,
                    off_sections,
                    sectionEntrySize,
                    idxAssociatedSymbolTable,
                    off_str_entry);
            const char* appliesToSection = getSectionName(
                    thefile,
                    off_sections,
                    sectionEntrySize,
                    idxAppliesToSection,
                    off_str_entry);
            PRINTF("\tThe symbol table is %s, it applies to %s\n", associatedSymbolTable, appliesToSection);
            Elf32_Rel* relEntry = (Elf32_Rel*)read_file(thefile,section->sh_offset, sizeof(Elf32_Rel));
            Elf32_Rel* lastRelEntry = (Elf32_Rel*)(((void*)relEntry) + section->sh_size);
            while (relEntry < lastRelEntry) {
                Elf32_Word symbolIdx = ELF32_R_SYM(relEntry->r_info);
                Elf32_Word relocType = ELF32_R_TYPE(relEntry->r_info);
                Elf32_Shdr* symbolSection = (Elf32_Shdr*)read_file(thefile,
                        off_sections + sectionEntrySize*idxAssociatedSymbolTable, sizeof(Elf32_Shdr));
                Elf32_Sym* syms = (Elf32_Sym*)read_file(thefile,
                        symbolSection->sh_offset + sizeof(Elf32_Sym)*symbolIdx, sizeof(Elf32_Sym));

                // calculate the symbol address
                void* lookupSymbolAddr = NULL;
                if (syms->st_shndx != SHN_UNDEF)
                    lookupSymbolAddr = sectionMapping[syms->st_shndx].addr + syms->st_value;
                else {
                    char * sName = getStrFromSection(
                            thefile,
                            off_sections + symbolSection->sh_link*sectionEntrySize,
                            syms->st_name);

                    lookupSymbolAddr = h_find(GOT, sName);

                    free(sName);
                }
                // calculate the offset to modify
                void* addrToModify = sectionMapping[idxAppliesToSection].addr + relEntry->r_offset;
                if (relocType == R_386_PC32) {
                    Elf32_Sword diff = (Elf32_Sword)lookupSymbolAddr - (Elf32_Sword)(addrToModify + 4);
                    *((Elf32_Addr *) addrToModify) = diff;
                }
                else if (relocType == R_386_32) {
                    Elf32_Addr elf32_addr = *((Elf32_Addr *) addrToModify);
                    elf32_addr += (Elf32_Addr)lookupSymbolAddr;
                    *((Elf32_Addr *) addrToModify) = elf32_addr;
                }
                relEntry ++;
            }
        }
    }

    // calling functions
    printf("Calling functions\n");
    h_forEach(functions, callingFunction);

    close_file(thefile);
    return 0;
}