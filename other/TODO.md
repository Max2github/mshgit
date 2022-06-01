# BUGS

## special blocks
using multi-line code for if, def, for, ...
leads to a severe segmentation fault.
On mac it only leads to memory leaks, but when cross-compiling for linux or windows, it leads to a program crash. Ubuntu even says:
"segmentation fault (core dumped)"

## object
passing an object as an argument to a function only transfers a part of the object

## list
list-add() seems not to work globally anymore (?).

# TODO

## OBJECT
changing members of objects is jet not possible inside functions.
- ~~make it possible for gloabal Vars (easy)~~ ✔
- ~~make it possible for local Vars (not so easy)~~ ✔

## LOCAL vs GLOBAL VARS
Just do something (check what works + implement what is missing).
- ~~Global Vars can be changed inside funcs (including objects and Arrays)~~ ✔
- ~~Global Vars can be called inside funcs (including objects and Arrays)~~ ✔