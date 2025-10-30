#include "library.h"

void swap(Piece *a, Piece *b)
{
    Piece* tmp = a;
    a = b;
    b = tmp;
}

int abs (int a)
{
    if (a < 0) return (-1)*a;
    return a;
}