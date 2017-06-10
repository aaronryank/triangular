#include <stdio.h>
#include <stdlib.h>

int stack[30000];
int size;

struct { int x, y, d; } jump[300];
int jumps;

extern int verbose, display_code;

extern char buf[1000][1000];
int direction, skip, x, y;

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
          case 1: x--; y--;    break;
          case 2: y -= 2; x--; break;
          case 3: y--;         break;
          case 4: x--;         break;
          case 5: x++;         break;
          case 6: y++;         break;
          case 7: y += 2; x++; break;
          case 8: x++, y++;    break;
        }

        if ((direction == 4 && x < 0) || (direction == 2 && y < 0) || (direction == 1 && (y < 0 || x < 0)))
            exit(EXIT_SUCCESS);

        if (x < 0)
            x = 0;
        if (y < 0)
            y = 0;
        if (buf[y][x] == 0)
            exit(EXIT_SUCCESS);
    }
}

void parse(char command)
{
    if (skip) {
        skip--;
        return;
    }

    if (display_code)
        putchar(command);

    switch (command) {
      /* directional */
      case '`': direction = 1; break;
      case '^': direction = 2; break;
      case '/': direction = 3; break;
      case '<': direction = 4; break;
      case '>': direction = 5; break;
      case ',': direction = 6; break;
      case '|': direction = 7; break;
      case '\\':direction = 8; break;
      case 'o': direction = direction == 8 ? 1 : direction + 1; break;
      case 'e': direction = direction == 1 ? 8 : direction - 1; break;
      case 'c': direction = direction == 8 ? 1 : direction + 1; buf[y][x] = 'z'; break;
      case 'z': direction = direction == 1 ? 8 : direction - 1; buf[y][x] = 'c'; break;

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
      case '%': printf("%d",stack[size-1]);      break;
      case '@': putchar(stack[size-1]);          break;

      case '1': case '2': case '3': case '4': case '5': case '6': case '7': case'8': case '9':
      stack[size++] = command - '0';             break;

      /* conditionals */
      case '?': skip = stack[size-1] < 1;                  break;
      case '!': skip = stack[size-1] > 0;                  break;
      case 's': skip = stack[size-1];                      break;
      case ';': if (stack[size-1] < 1) exit(EXIT_SUCCESS); break;

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
