#include "library.h"

void swap(Piece *a, Piece *b)
{
    Piece* tmp = a;
    a = b;
    b = tmp;
}
