# Triangular

Triangular is a two-dimensional stack-based esoteric programming language, inspired by Surface and Hexagony.
Source code is read in the shape of the smallest triangle that the length of the code can fit into. Whitespace is ignored.
For example, if the source code was simply six periods, Triangular would format this into:

      .
     . .
    . . .

Command-line options:

    --verbose                     prettyprint the triangle
    --verbose --display-code      prettyprint the triangle and display code as it's read

Triangular has:

 - A stack size of 30,000
 - A maximum of 300 jumps
 - A maximum source code of 1000 lines, 1000 characters per line

There are eight directions in which the instruction pointer can move (lines added for prettiness):

      1 2 3
       \|/
      4-.-5
       /|\
      6 7 8

(. is the placeholder command, the do-nothing look-pretty character.)

The directional numbers correspond to the cardinal directions and their combinations like so:

    1 NorthWest
    2 North
    3 NorthEast
    4 West
    5 East
    6 SouthWest
    7 South
    8 SouthEast

IP starts from the top of the triangle, moving in direction 8 (SE).

        \
         .
        . .
       . . .
      . . . .
     . . . . .

Acronyms: **ToS** = top of stack, **IP** = instruction pointer

     `^/
     <.>  visualization of directional switches
     ,v\

# Commands

## IP switches

    ` switch to direction 1 (NW)
    ^ switch to direction 2 (N)
    / switch to direction 3 (NE)
    < switch to direction 4 (W)
    > switch to direction 5 (E)
    , switch to direction 6 (SW)
    v switch to direction 7 (S)
    \ switch to direction 8 (SE)
    o rotate the direction of the IP clockwise
    e rotate the direction of the IP counter-clockwise
    c rotate the direction of the IP clockwise, then become z
    z rotate the direction of the IP counter-clockwise, then become c

## Program commands

    & end program

## Stack commands

    p pop
    + add ToS and ToS-1, pop both, push result
    - subtract ToS from ToS-1, pop both, push result
    * multiply ToS and ToS-1 together, pop both, push result
    _ divide ToS-1 by ToS, pop both, push result
    d decrement ToS
    i increment ToS
    $ input ToS as a number
    ~ input ToS as a character
    % print ToS as integer
    @ print ToS as character
    1 push 1 to stack. 2 pushes 2, etc.

## Conditionals (stolen directly from Surface)

    ? skip one instruction in the direction the IP is facing if ToS is <1
    ! skip one instruction in the direction the IP is facing if ToS is >0
    s skip the quantity of following instructions as specified by the number contained in ToS
    ( set a point to be jumped to by ) and ]
    ) unconditionally move the instruction pointer back to the most recent (
    ] move the instruction pointer back to the most recent ( if the ToS is >0
    x do away with the most recently accumulated (
    ; end program if ToS <0

## Unused characters

    0#='":[{}|abfghjklmnqrtuwyABCDEFGHIJKLMNOPQRSTUVWXYZ
