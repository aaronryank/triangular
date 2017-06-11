#include <stdio.h>
#include <stdlib.h>

int stack[30000];
int size;

struct { int x, y, d; } jump[300];
int jumps;

extern int verbose, display_code;

extern char buf[1000][1000];
int direction, skip, x, y, dx, dy;

const int NORTHWEST = 1;
const int NORTH = 2;
const int NORTHEAST = 3;
const int WEST = 4;
const int EAST = 5;
const int SOUTHWEST = 6;
const int SOUTH = 7;
const int SOUTHEAST = 8;

void parse(char);

void triangular(void)
{
    direction = 8;

    while (1)
    {
        if (!buf[y][0])
            return;

        parse(buf[y][x]);

        switch (direction) {
          case NORTHWEST:   dx = -1; dy = -1; break;
          case NORTH:       dx = -1; dy =  2; break;
          case NORTHEAST:   dx =  0; dy = -1; break;
          case WEST:        dx = -1; dy =  0; break;
          case EAST:        dx =  1; dy =  0; break;
          case SOUTHWEST:   dx =  0; dy =  1; break;
          case SOUTH:       dx =  1; dy =  2; break;
          case SOUTHEAST:   dx =  1; dy =  1; break;
        }
        
        x += dx * (skip + 1);
        y += dy * (skip + 1);
        skip = 0;

        if ((direction == WEST && x < 0) || (direction == NORTH && y < 0) || (direction == NORTHWEST && (y < 0 || x < 0)))
            exit(EXIT_SUCCESS);

        if (x < 0)
            x = 0;
        if (y < 0)
            y = 0;
        if (buf[y][x] == 0)
            exit(EXIT_SUCCESS);
    }
}

int next_direction(int direction, int change)
{
    if (change == 1){
        switch (direction) {
            case NORTH:     return NORTHEAST;
            case NORTHEAST: return EAST;
            case EAST:      return SOUTHEAST;
            case SOUTHEAST: return SOUTH;
            case SOUTH:     return SOUTHWEST;
            case SOUTHWEST: return WEST;
            case WEST:      return NORTHWEST;
            case NORTHWEST: return NORTH;
        }
    }else{
        switch (direction) {
            case NORTH:     return NORTHWEST;
            case NORTHEAST: return NORTH;
            case EAST:      return NORTHEAST;
            case SOUTHEAST: return EAST;
            case SOUTH:     return SOUTHEAST;
            case SOUTHWEST: return SOUTH;
            case WEST:      return SOUTHWEST;
            case NORTHWEST: return WEST;
        }        
    }
    return -1;
}

void parse(char command)
{
    if (display_code)
        putchar(command);

    switch (command) {
      /* directional */
      case '`': direction = NORTHWEST; break;
      case '^': direction = NORTH; break;
      case '/': direction = NORTHEAST; break;
      case '<': direction = WEST; break;
      case '>': direction = EAST; break;
      case ',': direction = SOUTHWEST; break;
      case 'v': direction = SOUTH; break;
      case '\\':direction = SOUTHEAST; break;
      case 'o': direction = next_direction(direction, 1); break;
      case 'e': direction = next_direction(direction, -1); break;
      case 'c': direction = next_direction(direction, 1); buf[y][x] = 'z'; break;
      case 'z': direction = next_direction(direction, -1); buf[y][x] = 'c'; break;

      /* program */
      case '&': exit(EXIT_SUCCESS);

      /* stack */
      case 'p': size > 0 && (size--);   break;
      case '+': size > 1 && (stack[size-2] += stack[size-1]); size > 1 && (size--); break;
      case '-': size > 1 && (stack[size-2] -= stack[size-1]); size > 1 && (size--); break;
      case '*': size > 1 && (stack[size-2] *= stack[size-1]); size > 1 && (size--); break;
      case '_': size > 1 && (stack[size-2] /= stack[size-1]); size > 1 && (size--); break;
      case 'i': size > 0 && (stack[size-1]++);     break;
      case 'd': size > 0 && (stack[size-1]--);     break;

      case '$': stack[size++] = getchar() - '0'; break;
      case '~': stack[size++] = getchar();       break;
      case '%': printf("%d",(size && stack[size-1]));      break;
      case '@': putchar((size && stack[size-1]));          break;

      case '1': case '2': case '3': case '4': case '5': case '6': case '7': case'8': case '9':
      stack[size++] = command - '0';             break;

      /* conditionals */
      case '?': skip = (size && stack[size-1]) < 1;                  break;
      case '!': skip = (size && stack[size-1]) > 0;                  break;
      case 's': skip = (size && stack[size-1]);                      break;
      case ';': if ((size && stack[size-1]) < 1) exit(EXIT_SUCCESS); break;

      case 'x': jumps--;
      case '(':
        jump[jumps].x = x;
        jump[jumps].y = y;
        jump[jumps].d = direction;
        jumps++;
        break;
      case ')':
        x = jump[jumps-1].x;
        y = jump[jumps-1].y;
        direction = jump[jumps-1].d;
        break;
      case ']':
        if (stack[size-1]) {
          x = jump[jumps-1].x;
          y = jump[jumps-1].y;
          direction = jump[jumps-1].d;
        }
        break;
    }
}
