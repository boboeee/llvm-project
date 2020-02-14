// RUN: %check_clang_tidy %s misc-alphabetical-decl-order %t

// FIXME: Add something that triggers the check here.
typedef struct node {
    int data;
    node* next;
} node;

void f() {
    //Not ok
    int b, a, c;
    node bye, hi;

    //Ok
    int a, b, c;
}