#define DLLIST_INIT_NODE(L)                    \
  do{                                           \
    (L)->prev = NULL;                           \
    (L)->next = NULL;                           \
  }while(0)

#define DLLIST_ADD_AFTER(NODE_TYPE, N1, N2) \
    do{                                           \
        NODE_TYPE* prev_next = (N1)->next;        \
        (N1)->next = N2;                          \
        (N2)->next = prev_next;                   \
        if (prev_next != NULL) { prev_next->prev = N2; }  \
        (N2)->prev = N1;                          \
    }while(0)



#define DLLIST_ADD_BEFORE(NODE_TYPE, N1, N2) \
    do{                                           \
        NODE_TYPE* prev_prev = (N1)->prev;        \
        (N1)->prev = N2;                          \
        (N2)->prev = prev_prev;                   \
        if (prev_prev != NULL) { prev_prev->next = N2; }  \
        (N2)->next = N1;                          \
    }while(0)

#define DLLIST_REMOVE(NODE_TYPE, N) \
    do{                                           \
        NODE_TYPE* prev_prev = (N)->prev;        \
        NODE_TYPE* prev_next = (N)->next;        \
        if (prev_prev != NULL) { prev_prev->next = prev_next; }  \
        if (prev_next != NULL) { prev_next->prev = prev_prev; }  \
    }while(0)
