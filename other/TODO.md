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
