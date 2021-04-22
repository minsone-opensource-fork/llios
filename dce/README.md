# Dead Code Elimination

This demonstrates how the linker treats unused code. Toggle `DEAD_STRIP` and `USE_STATIC_LIB` to compare different results.

```
DEAD_STRIP=0 USE_STATIC_LIB=1 ./build.sh
```

## Notes

### Swift public symbols
⚠️ [Swift public symbols are always `no_dead_strip`.](https://forums.swift.org/t/why-public-symbols-are-always-no-dead-strip/47264)

### .no_dead_strip directive
The assembler directive *.no_dead_strip* symbol_name can be used to specify that a symbol is not to be dead stripped. For example:
```
.no_dead_strip _my_version_string
.cstring
_my_version_string:
.ascii “cctools-501”
```

This can be seen (in object files only) with `nm -m` option and shows up as *[no dead strip]* in the following output:
```
% nm -m x.o
00000000 (__TEXT,__cstring) non-external *[no dead strip]* _my_version_string
```
The *.no_dead_strip* directive is generated by the new compiler when the __attribute__((used)) is specified on a symbol. LLVM assembly language use [@llvm.used](https://llvm.org/docs/LangRef.html#the-llvm-used-global-variable).
In object files, this bit is N_NO_DEAD_STRIP as defined in <mach-o/nlist.h>. And is set in relocatable .o files (MH_OBJECT filetype) only in the n_desc field of an nlist struct.

[Mac OS X Compiler Tools Release Notes](https://opensource.apple.com/source/cctools/cctools-622.5.1/RelNotes/CompilerTools.html?txt)

### Difference between linking .o and .a
When linking object files (.o):  
* If no symbol is used, all symbols defined in the .o will still be linked, but can be striped by DCE.  

When linking static libraries (.a):  
* If no symbol is used, the .a will be ignored.  
* If some symbols are used, the .o files (inside .a) which contain those symbols will be linked, while other .o will be ignored.  

Please note this behavior is different than DCE, which can strip unused methods from a linked object file. In other words, DCE can strip code at function level.

### -ObjC
From `man ld`, "-ObjC loads all members of static archive libraries that implement an Objective-C class or category". However, this is not the whole truth. From the [source code](https://github.com/apple-opensource/ld64/blob/e28c028b20af187a16a7161d89e91868a450cadc/src/ld/parsers/macho_relocatable_file.cpp#L1520~L1525) of `ld64`, `-ObjC` will actually load all Swift static libraries as well. This behavior can be confirmed by a sample code.

Because of this and non-strippable public symbols, linking swift libraries with `-ObjC` can enlarge the binary size a lot.

[Building Objective-C static libraries with categories](https://developer.apple.com/library/archive/qa/qa1490/_index.html)

