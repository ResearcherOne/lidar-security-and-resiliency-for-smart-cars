#ifndef BOOKS_H
#define BOOKS_H

#include <string.h>

struct Books {
   char  title[50];
   int   book_id;
};

void printBook( struct Books *book );
void initializeBook(struct Books *book);

#endif