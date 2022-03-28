#include "dynarr.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#define HASHBAG_MINIMUM_NUMBER_OF_BUCKETS 4

#define _DECLARE_HASHBAG_DS(HASHBAG_TYPE, \
                            HASHBAG_NAME, \
                            HASHBAG_TYPE_dynarr, \
                            HASHBAG_TYPE_dynarr_dynarr, \
                            HASHBAG_TYPE_dynarr_eq, \
                            MODIFIER, \
                            MALLOC_FUN_NAME, \
                            FREE_FUN_NAME, \
                            EQ_FUN_NAME, \
                            HASH_FUN_NAME)                            \
  \
  \
DECLARE_DYNARR_DS(HASHBAG_TYPE, MODIFIER, MALLOC_FUN_NAME, free, EQ_FUN_NAME, 1)  \
  \
bool HASHBAG_TYPE_dynarr_eq(HASHBAG_TYPE_dynarr* o1, HASHBAG_TYPE_dynarr* o2) {  \
    DS_SUPPRESS_WARNING(HASHBAG_TYPE_dynarr_eq);                       \
    assert(0 && "should never be used");  \
    return false;  \
} \
   \
DECLARE_DYNARR_DS(HASHBAG_TYPE_dynarr, MODIFIER, MALLOC_FUN_NAME, free, HASHBAG_TYPE_dynarr_eq, HASHBAG_MINIMUM_NUMBER_OF_BUCKETS)  \
  \
typedef struct {  \
    size_t size;  \
    HASHBAG_TYPE_dynarr_dynarr buckets;  \
} HASHBAG_NAME;  \
  \
MODIFIER void HASHBAG_NAME ## _init(HASHBAG_NAME* h) {  \
    DS_SUPPRESS_WARNING(HASHBAG_NAME ## _init);                       \
    h->size = 0;  \
    HASHBAG_TYPE ## _dynarr_dynarr_init(&h->buckets);  \
    HASHBAG_TYPE ## _dynarr_dynarr_push_void(&h->buckets, h->buckets.capacity);  \
    size_t number_of_buckets = HASHBAG_TYPE ## _dynarr_dynarr_size(&h->buckets);  \
    for (size_t i = 0; i < number_of_buckets; i++) {  \
        HASHBAG_TYPE ## _dynarr_init(HASHBAG_TYPE ## _dynarr_dynarr_item_at_ptr(&h->buckets, i));  \
    }  \
}  \
  \
MODIFIER void HASHBAG_NAME ## _print(HASHBAG_NAME* h);  \
  \
MODIFIER HASHBAG_NAME* HASHBAG_NAME ## _new(){  \
    DS_SUPPRESS_WARNING(HASHBAG_NAME ## _new);                       \
    HASHBAG_NAME* new_hashbag = (HASHBAG_NAME*)MALLOC_FUN_NAME(sizeof(HASHBAG_NAME));  \
    HASHBAG_NAME ## _init(new_hashbag);  \
    return new_hashbag;  \
  }  \
  \
MODIFIER void HASHBAG_NAME ## _destroy(HASHBAG_NAME* h) {  \
   DS_SUPPRESS_WARNING(HASHBAG_NAME ## _destroy);                       \
   size_t nr_of_buckets = HASHBAG_TYPE ## _dynarr_dynarr_size(&h->buckets);  \
   for (size_t i = 0; i < nr_of_buckets; i++) {  \
      HASHBAG_TYPE ## _dynarr_destroy(HASHBAG_TYPE ## _dynarr_dynarr_item_at_ptr(&h->buckets, i));  \
   }  \
   HASHBAG_TYPE ## _dynarr_dynarr_destroy(&h->buckets);  \
}  \
  \
MODIFIER void HASHBAG_NAME ## _free(HASHBAG_NAME* h) {  \
    DS_SUPPRESS_WARNING(HASHBAG_NAME ## _free);                       \
    HASHBAG_NAME ## _destroy(h);  \
    free(h);  \
}  \
  \
MODIFIER void ___ ## HASHBAG_NAME ## _adjust_number_of_buckets(HASHBAG_NAME* h,  \
                                       size_t nr_of_buckets,  \
                                       size_t new_nr_of_buckets) {  \
   DS_SUPPRESS_WARNING(___ ## HASHBAG_NAME ## _adjust_number_of_buckets);                       \
   HASHBAG_TYPE ## _dynarr_dynarr new_buckets;  \
   HASHBAG_TYPE ## _dynarr_dynarr_init(&new_buckets);  \
   HASHBAG_TYPE ## _dynarr_dynarr_push_void(&new_buckets, new_nr_of_buckets);  \
   for (size_t i = 0; i < new_nr_of_buckets; i++) {  \
        HASHBAG_TYPE ## _dynarr_init(HASHBAG_TYPE ## _dynarr_dynarr_item_at_ptr(&new_buckets, i));  \
   }  \
   for (size_t i = 0; i < nr_of_buckets; i++) {  \
       HASHBAG_TYPE_dynarr* bucket = HASHBAG_TYPE ## _dynarr_dynarr_item_at_ptr(&h->buckets, i);  \
       size_t bucket_size = HASHBAG_TYPE ## _dynarr_size(bucket);  \
       for (size_t j = 0; j < bucket_size; j++) {  \
            HASHBAG_TYPE* item = HASHBAG_TYPE ## _dynarr_item_at_ptr(bucket, j);  \
            size_t hash_v = HASH_FUN_NAME(item);  \
            size_t target_bucket_index = hash_v % new_nr_of_buckets;  \
            HASHBAG_TYPE_dynarr * target_bucket = HASHBAG_TYPE ## _dynarr_dynarr_item_at_ptr(&new_buckets, target_bucket_index);  \
            HASHBAG_TYPE ## _dynarr_push(target_bucket, *item);   \
       }  \
       HASHBAG_TYPE ## _dynarr_destroy(bucket);  \
   }  \
   HASHBAG_TYPE ## _dynarr_dynarr_destroy(&h->buckets);  \
   h->buckets = new_buckets;  \
   HASHBAG_TYPE ## _dynarr_dynarr_fix_after_move(&h->buckets);  \
   if (new_nr_of_buckets == HASHBAG_MINIMUM_NUMBER_OF_BUCKETS) {  \
        for(size_t i = 0; i < HASHBAG_MINIMUM_NUMBER_OF_BUCKETS; i++) {  \
            HASHBAG_TYPE ## _dynarr_fix_after_move(HASHBAG_TYPE ## _dynarr_dynarr_item_at_ptr(&h->buckets, i));  \
        }  \
   }  \
}  \
  \
MODIFIER void  ___ ## HASHBAG_NAME ## _expand(HASHBAG_NAME* h) {  \
   DS_SUPPRESS_WARNING( ___ ## HASHBAG_NAME ## _expand);                       \
   size_t nr_of_buckets = HASHBAG_TYPE ## _dynarr_dynarr_size(&h->buckets);  \
   if (nr_of_buckets > (h->size * 2)) {  \
        return;  \
   }  \
   size_t new_nr_of_buckets = nr_of_buckets * 2;  \
   ___ ## HASHBAG_NAME ## _adjust_number_of_buckets(h, nr_of_buckets, new_nr_of_buckets);  \
}  \
  \
MODIFIER void  ___ ## HASHBAG_NAME ## _shrink(HASHBAG_NAME* h) {  \
   DS_SUPPRESS_WARNING( ___ ## HASHBAG_NAME ## _shrink);                       \
   size_t nr_of_buckets = HASHBAG_TYPE ## _dynarr_dynarr_size(&h->buckets);  \
   if (nr_of_buckets == HASHBAG_MINIMUM_NUMBER_OF_BUCKETS ||  \
       ((h->size  * 8) > nr_of_buckets)) {  \
        return;  \
   }  \
   size_t new_nr_of_buckets = nr_of_buckets / 2;  \
   ___ ## HASHBAG_NAME ## _adjust_number_of_buckets(h, nr_of_buckets, new_nr_of_buckets);  \
}  \
  \
MODIFIER HASHBAG_NAME* HASHBAG_NAME ## _put(HASHBAG_NAME* h,  \
                               HASHBAG_TYPE object) {  \
    DS_SUPPRESS_WARNING(HASHBAG_NAME ## _put);                       \
   size_t nr_of_buckets = HASHBAG_TYPE ## _dynarr_dynarr_size(&h->buckets);  \
   size_t hash_v = HASH_FUN_NAME(&object);  \
   size_t target_bucket_index = hash_v % nr_of_buckets;  \
   HASHBAG_TYPE_dynarr * bucket = HASHBAG_TYPE ## _dynarr_dynarr_item_at_ptr(&h->buckets, target_bucket_index);  \
   HASHBAG_TYPE ## _dynarr_push(bucket, object);   \
   h->size += 1;  \
   ___ ## HASHBAG_NAME ## _expand(h);  \
   return h;  \
}  \
  \
MODIFIER HASHBAG_TYPE * HASHBAG_NAME ## _get(HASHBAG_NAME* h,  \
                       HASHBAG_TYPE object) {  \
    DS_SUPPRESS_WARNING(HASHBAG_NAME ## _get);                       \
   size_t nr_of_buckets = HASHBAG_TYPE ## _dynarr_dynarr_size(&h->buckets);  \
   size_t hash_v = HASH_FUN_NAME(&object);  \
   size_t target_bucket_index = hash_v % nr_of_buckets;  \
   HASHBAG_TYPE_dynarr * bucket = HASHBAG_TYPE ## _dynarr_dynarr_item_at_ptr(&h->buckets, target_bucket_index);  \
   return HASHBAG_TYPE ## _dynarr_ptr_to(bucket, object);   \
}  \
  \
MODIFIER HASHBAG_NAME* HASHBAG_NAME ## _remove(HASHBAG_NAME* h,  \
                                  HASHBAG_TYPE object) {  \
    DS_SUPPRESS_WARNING(HASHBAG_NAME ## _remove);                       \
   size_t nr_of_buckets = HASHBAG_TYPE ## _dynarr_dynarr_size(&h->buckets);  \
   size_t hash_v = HASH_FUN_NAME(&object);  \
   size_t target_bucket_index = hash_v % nr_of_buckets;  \
   HASHBAG_TYPE_dynarr * bucket = HASHBAG_TYPE ## _dynarr_dynarr_item_at_ptr(&h->buckets, target_bucket_index);  \
   HASHBAG_TYPE ## _dynarr_remove(bucket, object);  \
   h->size -= 1;  \
   ___ ## HASHBAG_NAME ## _shrink(h);  \
   return h;  \
}  \
  \
MODIFIER HASHBAG_NAME* HASHBAG_NAME ## _put_replace(HASHBAG_NAME* h,  \
                                       HASHBAG_TYPE object) {  \
    DS_SUPPRESS_WARNING(HASHBAG_NAME ## _put_replace);                       \
   size_t nr_of_buckets = HASHBAG_TYPE ## _dynarr_dynarr_size(&h->buckets);  \
   size_t hash_v = HASH_FUN_NAME(&object);  \
   size_t target_bucket_index = hash_v % nr_of_buckets;  \
   HASHBAG_TYPE_dynarr* bucket = HASHBAG_TYPE ## _dynarr_dynarr_item_at_ptr(&h->buckets, target_bucket_index);  \
   size_t bucket_size_before = HASHBAG_TYPE ## _dynarr_size(bucket);  \
   HASHBAG_TYPE ## _dynarr_remove(bucket, object);  \
   HASHBAG_TYPE ## _dynarr_push(bucket, object);   \
   h-> size += (HASHBAG_TYPE ## _dynarr_size(bucket) - bucket_size_before);   \
   return h;  \
}  
  

// Printing the table for debugging

/* MODIFIER void HASHBAG_NAME ## _print(HASHBAG_NAME* h) {  \ */
/*     DS_SUPPRESS_WARNING(HASHBAG_NAME ## _print);                       \ */
/*     printf("============================================================================\n");  \ */
/*    size_t nr_of_buckets = HASHBAG_TYPE ## _dynarr_dynarr_size(&h->buckets);  \ */
/*    for (size_t i = 0; i < nr_of_buckets; i++) {  \ */
/*        HASHBAG_TYPE_dynarr* bucket = HASHBAG_TYPE ## _dynarr_dynarr_item_at_ptr(&h->buckets, i);  \ */
/*        printf("BUCKET %lu %p:\n", i, bucket);  \ */
/*        size_t bucket_size = HASHBAG_TYPE ## _dynarr_size(bucket);  \ */
/*        for (size_t j = 0; j < bucket_size; j++) {  \ */
/*             HASHBAG_TYPE* item = HASHBAG_TYPE ## _dynarr_item_at_ptr(bucket, j);  \ */
/*             printf("    ITEM %ld\n", *item);  \ */
/*       }  \ */
/*    }  \ */
/*   \ */
/*     printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");  \ */
/* } */



#define DECLARE_HASHBAG_DS(HASHBAG_TYPE, MODIFIER, MALLOC_FUN_NAME, FREE_FUN_NAME, EQ_FUN_NAME, HASH_FUN_NAME) \
  _DECLARE_HASHBAG_DS(HASHBAG_TYPE, HASHBAG_TYPE ## _hash, HASHBAG_TYPE ## _dynarr, HASHBAG_TYPE ## _dynarr_dynarr, HASHBAG_TYPE ## _dynarr_eq, MODIFIER, MALLOC_FUN_NAME, FREE_FUN_NAME, EQ_FUN_NAME, HASH_FUN_NAME)

