# Triangular

Triangular is a two-dimensional stack-based esoteric programming language, inspired by [Surface](http://esolangs.org/wiki/Surface) and [Hexagony](https://github.com/m-ender/hexagony).
Source code is read in the shape of the smallest triangle that the length of the code can fit into. Whitespace is ignored.
For example, if the source code was `123456`, Triangular would format this into:

      1
     2 3
    4 5 6

To build, type `make`. This will generate an executable `triangular`. Command-line options:

    --verbose                     prettyprint the triangle
    --display-code                display code as it's read

Triangular has:

 - A stack size of 30,000
 - A maximum of 300 jumps
 - A maximum source code of 1000 lines, 1000 characters per line
 - A second stack (see section Memory), which is currently just one value.

Acronyms: **ToS** = top of stack, **IP** = instruction pointer

There are eight directions in which the IP can move (the IP is located at `.`):

        2  
       1 3
      8 . 4
       7 5
        6

(`.` is a no-op.)

The directional numbers correspond to the cardinal directions and their combinations like so:

    1 NorthWest  `
    2 North      ^
    3 NorthEast  /
    4 East       >
    5 SouthEast  \
    6 South      v
    7 SouthWest  ,
    8 West       <

Visualization of directional switches:

       ^
      ` /
     < . >  
      , \
       v

IP starts from the top of the triangle, moving in direction 5 (SE).

        \
         .
        . .
       . . .
      . . . .
     . . . . .

If the IP goes off the playing field, the program will terminate.

# Commands

## IP switches

    ` switch to direction 1 (NW)
    ^ switch to direction 2 (N)
    / switch to direction 3 (NE)
    > switch to direction 4 (E)
    \ switch to direction 5 (SE)
    v switch to direction 6 (S)
    , switch to direction 7 (SW)
    < switch to direction 8 (W)
    o rotate the direction of the IP clockwise
    e rotate the direction of the IP counter-clockwise
    c rotate the direction of the IP clockwise, then become z
    z rotate the direction of the IP counter-clockwise, then become c

## Program commands

    & end program

## Stack commands

    p pop
    : duplicate ToS
    + add ToS and ToS-1, pop both, push result
    - subtract ToS from ToS-1, pop both, push result
    * multiply ToS and ToS-1 together, pop both, push result
    _ divide ToS-1 by ToS, pop both, push result
    = compare ToS and ToS-1 for equality, DON'T pop, push result
    l push 1 if ToS-1 < ToS, otherwise 0
    g push 1 if ToS-1 > ToS, otherwise 0
    | negate ToS (turns positive into negative, negative into positive)
    d decrement ToS
    i increment ToS
    0 push 0 to stack. 1 pushes 1, 2 pushes 2, etc.
    A push 10 to stack. B pushes 11, etc. up to F.
    " swap top two stack values

## I/O commands

    $ input ToS as a number
    ~ input ToS as a character
    % print ToS as integer
    @ print ToS as character
    # print ToS as character and pop

## Memory commands

    P pop ToS stack to memory
    S stash ToS in memory
    U pull memory to stack

## Conditionals (stolen directly from Surface)

    ? skip one instruction in the direction the IP is facing if ToS is <= 0
    ! skip one instruction in the direction the IP is facing if ToS is > 0
    s skip the quantity of following instructions as specified by the number contained in ToS
    ( set a point to be jumped to by ) and ]
    ) unconditionally move the instruction pointer back to the most recent (
    ] move the instruction pointer back to the most recent ( if the ToS is > 0, otherwise do away with the most recently accumulated (
    x do away with the most recently accumulated (
    ; end program if ToS <= 0

## Unused characters

    '[{}abfhjkmnqrtuwyGHIJKLMNOQRTVWXYZ
