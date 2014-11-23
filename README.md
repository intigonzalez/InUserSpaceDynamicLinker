#Global Idea

This application implements a reduced version of 
an in user-space dynamic linker, which is kind of cool.

It receives as parameter a relocalisable
elf32 file, aka a file with extension *.o.

The application loads the file into its own address space using
a builtin elf loader. During the process, it loads all the 
globally defined functions and executes them with a single parameter.


#Limitations:
- A lot, this is only a prototype used to understand how an Elf loader (or Linker Editor) works
- It is only able to process functions with a single defined parameter of type int.
- It  only understand two types of relocations
- It only works for x86, 32 bits architecture
- Actually, you are lucky if you can use it for something else than executing the example I am providing

- [ ] Pe
- [ ] to

#How to use it?
Mostly, read the the source code. It is the only useful thing.
If, nevertheless, you want to execute it, the line below will do the magic.

```
make
make run
```

It will first build. Afterwards, it will execute the example

##Why are you publishing such a useless piece of code in GitHub?
Well, I found myself looking at the Elf Loader of Contiki, a well-known operating system for IoT devices,
while a college was trying to port the operating system to a new device.
We use to talk because I like the topic and have some experience. I believe doing this is not as uncommon as you may believe.

In general I believe this code can be usefull to people interested on compilation and/or operating systems; 
a beginner stuff. Cool anyway!!!

I tried, without success I fear, to make it as portable as possible by abstracting the access to files, which is very important in embedded devices. In general, your device has to provide random access to the file. 

Happy coding ;-)
