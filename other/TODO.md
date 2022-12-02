# BUGS

## object
- passing an object as an argument to a function only transfers a part of the object // haven't checked for a while - eventually already fixed

## list
- list-add() seems not to work globally anymore (?). // haven't checked for a while  - eventually already fixed

## find segmentation faults
- none

### ideas & possible solutions
- none

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
