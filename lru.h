#include "hashbag.h"
#include <stdlib.h>
#include "list.h"


#define _DECLARE_LRUCACHE_DS(LRUCACHE_TYPE, LRUCACHE_NAME, LRUCACHE_NODE, LRUCACHE_NODE_ptr, LRUCACHE_NODE_ptr_hash, LRUCACHE_NODE_ptr_eq, LRUCACHE_NODE_ptr_hash_fun, MODIFIER, MALLOC_FUN_NAME, FREE_FUN_NAME, EQ_FUN_NAME, HASH_FUN_NAME, DESTROY_FUN_NAME, EVICT_CONDITION_FUN_NAME) \
 \
typedef struct LRUCACHE_NODE { \
  struct LRUCACHE_NODE* prev;  \
  struct LRUCACHE_NODE* next;  \
  LRUCACHE_TYPE value; \
} LRUCACHE_NODE; \
 \
typedef LRUCACHE_NODE* LRUCACHE_NODE_ptr;  \
 \
 \
MODIFIER bool LRUCACHE_NODE_ptr_eq(LRUCACHE_NODE_ptr * o1, LRUCACHE_NODE_ptr * o2) { \
    return EQ_FUN_NAME(&(*o1)->value, &(*o2)->value);  \
} \
 \
MODIFIER bool LRUCACHE_NODE_ptr_hash_fun(LRUCACHE_NODE_ptr * object) { \
    return HASH_FUN_NAME(&(*object)->value);  \
} \
 \
DECLARE_HASHBAG_DS(LRUCACHE_NODE_ptr, MODIFIER, MALLOC_FUN_NAME, FREE_FUN_NAME, LRUCACHE_NODE_ptr_eq, LRUCACHE_NODE_ptr_hash_fun) \
 \
typedef struct LRUCACHE_NAME { \
    LRUCACHE_NODE dummy_first; \
    LRUCACHE_NODE dummy_last; \
    LRUCACHE_NODE_ptr_hash hashset; \
} LRUCACHE_NAME; \
 \
 \
MODIFIER void LRUCACHE_NAME ## _init(LRUCACHE_NAME * l) { \
     DLLIST_INIT_NODE(&l->dummy_first); \
     DLLIST_INIT_NODE(&l->dummy_last); \
     DLLIST_ADD_AFTER(LRUCACHE_NODE, &l->dummy_first, &l->dummy_last); \
     LRUCACHE_NODE_ptr_hash ## _init(&l->hashset); \
} \
 \
 \
MODIFIER void LRUCACHE_NAME ## _destroy(LRUCACHE_NAME * l) { \
     LRUCACHE_NODE * curr = &l->dummy_first; \
     LRUCACHE_NODE * next = curr->next; \
     LRUCACHE_NODE * last = &l->dummy_last; \
     while ( next != last) { \
        curr = next; \
        next = curr->next; \
        DESTROY_FUN_NAME(&curr->value); \
        FREE_FUN_NAME(curr); \
     } \
     LRUCACHE_NODE_ptr_hash ## _destroy(&l->hashset); \
} \
 \
MODIFIER LRUCACHE_NAME * LRUCACHE_NAME ## _new() { \
    LRUCACHE_NAME * l = (LRUCACHE_NAME*)MALLOC_FUN_NAME(sizeof(LRUCACHE_NAME));   \
    LRUCACHE_NAME ## _init(l); \
    return l; \
} \
 \
MODIFIER void LRUCACHE_NAME ## _free(LRUCACHE_NAME * l) { \
    LRUCACHE_NAME ## _destroy(l); \
    FREE_FUN_NAME(l); \
} \
 \
MODIFIER LRUCACHE_TYPE * LRUCACHE_NAME ## _get(LRUCACHE_NAME* h, \
                    LRUCACHE_TYPE object) { \
   LRUCACHE_NODE lookup_node; \
   lookup_node.value = object; \
   LRUCACHE_NODE_ptr* existing_node = LRUCACHE_NODE_ptr_hash ## _get(&h->hashset, &lookup_node); \
   if (existing_node == NULL) { \
       return NULL; \
   } \
   /* Remove this node and move it to the front */ \
   DLLIST_REMOVE(LRUCACHE_NODE, (*existing_node)); \
   DLLIST_ADD_AFTER(LRUCACHE_NODE, &h->dummy_first, (*existing_node)); \
   return &(*existing_node)->value; \
} \
 \
 \
MODIFIER void LRUCACHE_NAME ## _add(LRUCACHE_NAME* h, \
                  LRUCACHE_TYPE object) { \
   LRUCACHE_NODE* node = (LRUCACHE_NODE*)MALLOC_FUN_NAME(sizeof(LRUCACHE_NODE)); \
   node->value = object; \
   DLLIST_ADD_AFTER(LRUCACHE_NODE, &h->dummy_first, node); \
   LRUCACHE_NODE_ptr_hash ## _put(&h->hashset, node); \
   if (EVICT_CONDITION_FUN_NAME(h->hashset.size, &h->dummy_last.prev->value)) { \
       LRUCACHE_NODE * node_to_remove = h->dummy_last.prev; \
       DLLIST_REMOVE(LRUCACHE_NODE, node_to_remove); \
       LRUCACHE_NODE_ptr_hash ## _remove(&h->hashset, node_to_remove); \
       DESTROY_FUN_NAME(&node_to_remove->value); \
       FREE_FUN_NAME(node_to_remove); \
   } \
} \
\
MODIFIER size_t LRUCACHE_NAME ## _size(LRUCACHE_NAME* h) { \
    return h->hashset.size; \
} \
 \
MODIFIER int LRUCACHE_NAME ## _evict_if_condition_is_true(LRUCACHE_NAME* h) { \
   if (LRUCACHE_NAME ## _size(h) == 0) { \
       return 0; \
   } \
   if (EVICT_CONDITION_FUN_NAME(h->hashset.size, &h->dummy_last.prev->value)) { \
       LRUCACHE_NODE * node_to_remove = h->dummy_last.prev; \
       DLLIST_REMOVE(LRUCACHE_NODE, node_to_remove); \
       LRUCACHE_NODE_ptr_hash ## _remove(&h->hashset, node_to_remove); \
       DESTROY_FUN_NAME(&node_to_remove->value); \
       FREE_FUN_NAME(node_to_remove); \
       return 1; \
   } \
   return 0; \
} 

 

#define DECLARE_LRUCACHE_DS(LRUCACHE_TYPE, MODIFIER, MALLOC_FUN_NAME, FREE_FUN_NAME, EQ_FUN_NAME, HASH_FUN_NAME, DESTROY_FUN_NAME, EVICT_CONDITION_FUN_NAME) \
  _DECLARE_LRUCACHE_DS(LRUCACHE_TYPE, LRUCACHE_TYPE ## _lru, LRUCACHE_TYPE ## _lru_node, LRUCACHE_TYPE ## _lru_node_ptr, LRUCACHE_TYPE ## _lru_node_ptr_hash, LRUCACHE_TYPE ## _lru_node_ptr_eq, LRUCACHE_TYPE ## _lru_node_ptr_hash_fun, MODIFIER, MALLOC_FUN_NAME, FREE_FUN_NAME, EQ_FUN_NAME, HASH_FUN_NAME, DESTROY_FUN_NAME, EVICT_CONDITION_FUN_NAME)


