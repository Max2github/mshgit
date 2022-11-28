# BUGS

## object
passing an object as an argument to a function only transfers a part of the object

## list
list-add() seems not to work globally anymore (?).

## find segmentation faults
- When creating a msh server (doesn't have to be the cause) on macOS (ARM64)
  -> word_len      458:1
  -> word_compare  466:1
  -> msh_readZeile  36:1 
- when starting a msh client (doesn't have to be the cause) on macOS  (ARM64)
  word_copy        450:1
  msh_fillVar       87:1
- when running a http server (doesn't have to be the cause) on macOS (ARM64)
  msh_fill_local_Var 47

### ideas & possible solutions
- stack-overflow - especially, because these segmentation faults do not appear in linux
  (I only saw them on macOS)
  This is also very likely, because since removing the speicher[4000] in msh_io,
  the segmentation fault occurs a lot less often.
  -> copy & use less memory and thik about putting VAR_SPEICHER (and VAR_NAMES) in the heap.
- several problems -> make copying safer with limits (do not use word_copy directly, 
  check if string is not too long)

# TODO

## LOCAL OBJECTs
using local (declared a inside function) objects is yet not possible.
- need fillObj + ...

## ref asign 
write to refs like this:

    // append
    myref << mydata
    // overwrite
    myref <- mydata
