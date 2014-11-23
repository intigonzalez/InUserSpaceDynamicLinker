#Global Idea

This application receives as parameter a relocalisable
elf32 file, aka a file with extension *.o.

The application loads the file into its own adress space using
a builtin elf loader. During the process, it loads all the 
globally defined functions and executes them with a single parameter.

Somehow, this application is implementing a reduced version of 
an in user-space dynamic linker, which is kind of cool.

#Limitatios:
- A lot, this is only a prototype used to understand how an Elf loader (or Linker Editor) works
- It is only able to process functions with a single defined parameter of type int.
- It  only understand two types of relocations
- It only works for x86, 32 bits architecture
- Actually, you are lucky if you can use it for something else than executing the example I am providing
- ;-)
