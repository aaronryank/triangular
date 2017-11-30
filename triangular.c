#include <stdio.h>
#include <stdlib.h>

int stack[30000];
int size;
int memory;

struct { int x, y, d; } jump[300];
int jumps;

extern int display_code;

extern char buf[1000][1000];
int direction, skip, x, y, dx, dy;

enum { UNDEF, NORTHWEST, NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST };

void parse(char);
int swap(int*,int*);

void triangular(void)
{
    direction = SOUTHEAST;

    while (1)
    {
        if (!buf[y][0])
            return;

        parse(buf[y][x]);

        switch (direction) {
          case NORTHWEST:   dx = -1; dy = -1; break;
          case NORTH:       dx = -1; dy = -2; break;
          case NORTHEAST:   dx =  0; dy = -1; break;
          case WEST:        dx = -1; dy =  0; break;
          case EAST:        dx =  1; dy =  0; break;
          case SOUTHWEST:   dx =  0; dy =  1; break;
          case SOUTH:       dx =  1; dy =  2; break;
          case SOUTHEAST:   dx =  1; dy =  1; break;
        }

        skip = skip < 0 ? 0 : skip;
        x += dx * (skip + 1);
        y += dy * (skip + 1);
        skip = 0;

        if (x < 0 || x > y || y < 0 || y >= 1000 || buf[y][x] <= 0)
            exit(EXIT_SUCCESS);
    }
}

int next_direction(int direction, int change)
{
    if (change == 1) {
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
    } else {
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
    if (display_code) {
        printf("Executing %c\n",command);
        usleep(500000);
    }        

    switch (command) {
      /* directional */
      case '`': direction = NORTHWEST;                                      break;
      case '^': direction = NORTH;                                          break;
      case '/': direction = NORTHEAST;                                      break;
      case '<': direction = WEST;                                           break;
      case '>': direction = EAST;                                           break;
      case ',': direction = SOUTHWEST;                                      break;
      case 'v': direction = SOUTH;                                          break;
      case '\\':direction = SOUTHEAST;                                      break;
      case 'o': direction = next_direction(direction, 1);                   break;
      case 'e': direction = next_direction(direction, -1);                  break;
      case 'c': direction = next_direction(direction, 1); buf[y][x] = 'z';  break;
      case 'z': direction = next_direction(direction, -1); buf[y][x] = 'c'; break;

      /* more directionals jkmnqrtw */
      case 'j': size && stack[size-1] && (direction = NORTHWEST);  break;
      case 'k': size && stack[size-1] && (direction = NORTH);      break;
      case 'y': size && stack[size-1] && (direction = NORTHEAST);  break;
      case 'n': size && stack[size-1] && (direction = EAST);       break;
      case 'q': size && stack[size-1] && (direction = SOUTHEAST);  break;
      case 'r': size && stack[size-1] && (direction = SOUTH);      break;
      case 't': size && stack[size-1] && (direction = SOUTHWEST);  break;
      case 'w': size && stack[size-1] && (direction = WEST);       break;

      /* program */
      case '&': exit(EXIT_SUCCESS);

      /* stack */
      case 'p': size > 0 && (size--);                                               break;
      case ':': size > 0 && (stack[size] = stack[size-1]); size > 0 && (size++);    break;
      case '+': size > 1 && (stack[size-2] += stack[size-1]); size > 1 && (size--); break;
      case '-': size > 1 && (stack[size-2] -= stack[size-1]); size > 1 && (size--); break;
      case '*': size > 1 && (stack[size-2] *= stack[size-1]); size > 1 && (size--); break;
      case '_': size > 1 && (stack[size-2] /= stack[size-1]); size > 1 && (size--); break;
      case 'i': size > 0 ?  (stack[size-1]++) : (stack[size++] = 1);                break;
      case 'd': size > 0 ?  (stack[size-1]--) : (stack[size++] = -1);               break;
      case '|': size > 0 && (stack[size-1] = -stack[size-1]);                       break;
      case 'm': size > 1 && (stack[size-2] %= stack[size-1]); size > 1 && (size--); break;
      case '"': size > 1 && swap(&stack[size-2],&stack[size-1]);                    break;

      case '$': scanf("%d",&stack[size++]);                 break;
      case '~': stack[size++] = getchar();                  break;
      case '%': printf("%d",size ? stack[size-1] : 0);      break;
      case '@': size && putchar(stack[size-1]);             break;
      case '#': size && putchar(stack[size-1]) && (size--); break;

      case '=': stack[size] = size > 1 ? (stack[size-2] == stack[size-1]) : 0;
                size++;
                break;
      case 'l': size > 1 && (stack[size-2] = (stack[size-2] < stack[size-1])); size > 1 && (size--);
      case 'g': size > 1 && (stack[size-2] = (stack[size-2] > stack[size-1])); size > 1 && (size--);
      case 'u': size && stack[size-1] < 0 && (stack[size-1] = -stack[size-1]);
                break;

      case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case'8': case '9':
      stack[size++] = command - '0';             break;
      case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
      stack[size++] = command - 'A' + 10;        break;

      /* memory */
      case 'P': size && (memory = stack[size-1]); size && (size--); break;
      case 'S': size && (memory = stack[size-1]);                   break;
      case 'U': memory && (stack[size++] = memory);                 break;

      /* conditionals */
      case '?': skip = size ? (stack[size-1] <= 0) : 1;                break;
      case '!': skip = size ? (stack[size-1] > 0) : 0;                 break;
      case 's': skip = (size ? stack[size-1] : 0);                     break;
      case ';': if (!size || (stack[size-1] <= 0)) exit(EXIT_SUCCESS); break;

      case 'x': jumps && (jumps--); break;
      case '(':
        jump[jumps].x = x;
        jump[jumps].y = y;
        jump[jumps].d = direction;
        jumps++;
        break;
      case ')':
        if (jumps) {
          x = jump[jumps-1].x;
          y = jump[jumps-1].y;
          direction = jump[jumps-1].d;
        }
        break;
      case ']':
        if (jumps && size && stack[size-1]) {
          x = jump[jumps-1].x;
          y = jump[jumps-1].y;
          direction = jump[jumps-1].d;
        } else {
            jumps && (jumps--);
        }
        break;
    }
}

int swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
