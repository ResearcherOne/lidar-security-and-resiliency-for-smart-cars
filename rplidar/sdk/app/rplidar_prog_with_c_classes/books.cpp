#include <stdio.h>
#include "books.h"

void initializeBook(struct Books *book)
{
    strcpy( book->title, "C Programming BOOK!");
    book->book_id = 1;
}

void printBook( struct Books *book )
{
   printf( "Book title : %s\n", book->title);
   printf( "Book book_id : %d\n", book->book_id);
}