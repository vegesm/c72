# c72
A port of the [earliest C compiler](https://www.bell-labs.com/usr/dmr/www/primevalC.html) to x86. The compiler generates 32-bit code and works on modern Linux with a current libc. It follows the architecture of the original implementation closely, keeping the bugs and missing features.

## Usage
To compile the compiler:
```shell
# install 32-bit libraries
sudo apt-get install gcc-multilib

make
./c72 examples/fizzbuzz.c fizzbuzz
./fizzbuzz
```
Note: if you get errors on missing "bits/libc-header-start.h" headers make sure you have the 32bit libc installed.

I've also attached an [early implementation of cp](https://www.tuhs.org/cgi-bin/utree.pl?file=V5/usr/source/s1/cp.c) from UNIX v5. It worked with minor modifications with this compiler, on my Ubuntu 18.04. It's pretty neat, to have a nearly 50 years old code compile with a nearly 50 years old compiler and run on a modern OS!


## Differences to modern C

This version of C is from around 1972. While the general syntax is pretty much the same as today,
there are tons of missing features:
- no preprocessor, no for loops
- bitwise NOT and XOR is not implemented
- no short-circuiting AND and OR
- even though there is a keyword for `float` and `double`, floating point calculations are not implemented, you can not even write a floating point literal
- the type system is very weak: pointers, chars, ints can be freely converted into one another
- types of the function parameters are not checked, anything can be passed to any function
- compound assignment operators are reversed, they are `=+`, `=*`
- only integer global variables can be defined, and the syntax is strange:
```
/* defines globalvar to have the value of 2 */
globalvar 2;

/* equivalent to int globalarr[]={1, 2, 3}; */
globalarr[] 1, 2, 3;
```
- variable names can be of any length but only the first 8 characters are used; i.e. deadbeef1 and deadbeef2 are effectively the same variables
  

## Internals
The compiler has two stages: `c0` and `c1`. `c0` parses the input C file and generates a half-assembly half-parsed syntax tree temporary file. It is not meant to make the second stage easily portable, rather the memory restrictions of the PDP-11 made the split necessary. Then `c1` generates the final assembly, translating the syntax trees to actual code.

`c0` is a fairly straightforward parser. One interesting bit is that `a[b]` is parsed and then converted to `*(a+b)` already in this very early version of C (if you  are not familiar with the C standard, `a[b]` is defined to be equivalent to `*((a)+(b))`).

`c1` uses a set of code-generation tables to create the final assmebly. For example, addition is defined as:
```
%n,aw
F
add A2,R
```
The part starting with `%` is the pattern the syntax tree has to match. This pattern means the first operand can be anything (`n`), the second operand is a word sized (`w`) variable (`a` - addressible). Then the code generation instructions say that calculate and place the first operand in the current register (`F`), and create an `add` instruction where the source argument is the address of the second operand (`A2`) and the target argument is the current register (`R`).  
For example, the C code `2+x` would generate the following assembly:

```asm
mov $2, %eax
add 4(%ebp), %eax
```
Here I assumed that `x` is the first parameter of the function so it is placed right after the stack frame.

If you are interested in a more in-depth description see Dennis Ritchie's [A Tour through the UNIX C compiler](docs/ctour.pdf). Although it is for a newer version with some differences, it is still helpful to understand `c1`.

