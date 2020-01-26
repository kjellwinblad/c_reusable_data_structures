#ifndef DYNARR_H
#define DYNARR_H 1

#include <assert.h>
#define DS_SUPPRESS_WARNING(a) (void)a

#define _DECLARE_DYNARR_DS(DYNARR_TYPE,\
                           DYNARR_NAME,\
                           MODIFIER,\
                           MALLOC_FUN_NAME,\
                           FREE_FUN_NAME,\
                           DEFAULT_CAPACITY)                            \
  typedef struct {                                                      \
    int size;                                                           \
    int capacity;                                                       \
    DYNARR_TYPE* dynarr_array;                                          \
    DYNARR_TYPE first_array[DEFAULT_CAPACITY];                          \
  } DYNARR_NAME;                                                        \
                                                                        \
  MODIFIER void DYNARR_NAME ## _init_with_capacity(DYNARR_NAME* new_dynarr,\
                                                   int capacity){       \
    DS_SUPPRESS_WARNING(DYNARR_NAME ## _init_with_capacity);            \
    assert(DEFAULT_CAPACITY >= 1);                                      \
    new_dynarr->size = 0;                                               \
    if(capacity <= DEFAULT_CAPACITY){                                   \
      new_dynarr->capacity = DEFAULT_CAPACITY;                          \
      new_dynarr->dynarr_array = new_dynarr->first_array;               \
    } else {                                                            \
      new_dynarr->capacity = capacity;                                  \
      new_dynarr->dynarr_array =                                        \
        MALLOC_FUN_NAME(sizeof(DYNARR_TYPE) * new_dynarr->capacity);    \
    }                                                                   \
  }                                                                     \
                                                                        \
  MODIFIER void DYNARR_NAME ## _init(DYNARR_NAME* new_dynarr){          \
    DS_SUPPRESS_WARNING(DYNARR_NAME ## _init);                          \
    DYNARR_NAME ## _init_with_capacity(new_dynarr, 0);                  \
  }                                                                     \
                                                                        \
  MODIFIER void DYNARR_NAME ## _destroy(DYNARR_NAME* dynarr){           \
    if(dynarr->capacity != DEFAULT_CAPACITY){                           \
      FREE_FUN_NAME(dynarr->dynarr_array);                              \
    }                                                                   \
  }                                                                     \
                                                                        \
                                                                        \
  MODIFIER DYNARR_TYPE* DYNARR_NAME ## _current_raw_array(DYNARR_NAME* dynarr){ \
    return dynarr->dynarr_array;                                        \
  }                                                                     \
                                                                        \
  MODIFIER void DYNARR_NAME ## _free(DYNARR_NAME* dynarr){              \
    DYNARR_NAME ## _destroy(dynarr);                                    \
    free(dynarr);                                                       \
  }                                                                     \
                                                                        \
  MODIFIER DYNARR_NAME* DYNARR_NAME ## _new(){                          \
    DYNARR_NAME* new_dynarr = MALLOC_FUN_NAME(sizeof(DYNARR_NAME));  \
    DYNARR_NAME ## _init(new_dynarr);                                   \
    return new_dynarr;                                                  \
  }                                                                     \
                                                                        \
  MODIFIER DYNARR_TYPE* DYNARR_NAME ## _push_void(DYNARR_NAME* s,       \
                                                  size_t nr_of_void_entries){ \
    int new_capacity = s->capacity;                                     \
    while((s->size + nr_of_void_entries) > new_capacity){               \
      new_capacity = new_capacity * 2;                                  \
    }                                                                   \
    if(new_capacity != s->capacity){                                    \
      DYNARR_TYPE* prev_array = s->dynarr_array;                        \
      s->dynarr_array = MALLOC_FUN_NAME(sizeof(DYNARR_TYPE) * new_capacity); \
      for(int i = 0; i < s->capacity; i++){                             \
        s->dynarr_array[i] = prev_array[i];                             \
      }                                                                 \
      if(s->capacity != DEFAULT_CAPACITY){                              \
        free(prev_array);                                               \
      }                                                                 \
      s->capacity = new_capacity;                                       \
    }                                                                   \
    DYNARR_TYPE* ret = &s->dynarr_array[s->size];                       \
    s->size = s->size + nr_of_void_entries;                             \
    return ret;                                                         \
  }                                                                     \
                                                                        \
  MODIFIER DYNARR_NAME* DYNARR_NAME ## _push_array(DYNARR_NAME* s,      \
                                                   size_t item_array_size, \
                                                   DYNARR_TYPE* item_array){ \
    DYNARR_TYPE* store_to = DYNARR_NAME ## _push_void(s, item_array_size); \
    for(int i = 0; i < item_array_size; i++){                           \
      store_to[i] = item_array[i];                                      \
    }                                                                   \
    return s;                                                           \
  }                                                                     \
                                                                        \
  MODIFIER DYNARR_NAME* DYNARR_NAME ## _push(DYNARR_NAME* s, DYNARR_TYPE item){ \
    DYNARR_TYPE* store_to = DYNARR_NAME ## _push_void(s, 1);            \
    *store_to = item;                                                   \
    return s;                                                           \
  }                                                                     \
                                                                        \
                                                                        \
  MODIFIER DYNARR_TYPE DYNARR_NAME ## _top(DYNARR_NAME* s){             \
    return s->dynarr_array[s->size - 1];                                \
  }                                                                     \
                                                                        \
                                                                        \
  MODIFIER DYNARR_TYPE DYNARR_NAME ## _item_at(DYNARR_NAME* s,          \
                                               int index){              \
    return s->dynarr_array[index];                                      \
  }                                                                     \
                                                                        \
                                                                        \
  MODIFIER DYNARR_NAME* DYNARR_NAME ## _set_item_at(DYNARR_NAME* s,     \
                                                    int index,          \
                                                    DYNARR_TYPE new_item){ \
    s->dynarr_array[index] = new_item;                                  \
    return s;                                                           \
  }                                                                     \
                                                                        \
                                                                        \
  MODIFIER  DYNARR_TYPE DYNARR_NAME ## _pop(DYNARR_NAME* s){            \
    DYNARR_TYPE t = DYNARR_NAME ## _top(s);                             \
    if(s->size > 0) s->size = s->size - 1;                              \
    return t;                                                           \
  }                                                                     \
                                                                        \
  MODIFIER DYNARR_NAME* DYNARR_NAME ## _drop_last(DYNARR_NAME* s,       \
                                                  int nr_of_items_to_drop) { \
    if(s->size - nr_of_items_to_drop > 0){                              \
      s->size = s->size - nr_of_items_to_drop;                          \
    }                                                                   \
    return s;                                                           \
  }                                                                     \
                                                                        \
  MODIFIER  int DYNARR_NAME ## _size(DYNARR_NAME* s){                   \
    return s->size;                                                     \
  }                                                                     \
                                                                        \
  MODIFIER DYNARR_NAME* DYNARR_NAME ## _concat(DYNARR_NAME* s1,         \
                                               DYNARR_NAME* s2){        \
    int s2_size = DYNARR_NAME ## _size(s2);                             \
    DYNARR_TYPE* store_to =                                             \
      DYNARR_NAME ## _push_void(s1,                                     \
                                s2_size);                               \
    DYNARR_TYPE * a =                                                   \
      DYNARR_NAME ## _current_raw_array(s1);                            \
    for(int i = 0; i < s2_size; i++){                                   \
      store_to[i] = a[i];                                               \
    }                                                                   \
    return s1;                                                          \
  }                                                                     \
                                                                        \
  MODIFIER int DYNARR_NAME ## _index_of(DYNARR_NAME* s,                 \
                                        DYNARR_TYPE element){           \
    int size = DYNARR_NAME ## _size(s);                                 \
    DYNARR_TYPE * a =                                                   \
      DYNARR_NAME ## _current_raw_array(s);                             \
    for(int i = 0; i < size; i++){                                      \
      if(a[i] == element) {                                             \
        return i;                                                       \
      }                                                                 \
    }                                                                   \
    return -1;                                                          \
  }                                                                     \
                                                                        \
  MODIFIER DYNARR_NAME* DYNARR_NAME ## _replace(DYNARR_NAME* s,         \
                                                DYNARR_TYPE to_replace, \
                                                DYNARR_TYPE replace_with){ \
    int index = DYNARR_NAME ## _index_of(s, to_replace);                \
    if(index != -1) {                                                   \
      DYNARR_NAME ## _set_item_at(s, index, replace_with);              \
    }                                                                   \
    return s;                                                           \
  }                                                                     \
                                                                        \
  MODIFIER DYNARR_NAME* DYNARR_NAME ## _remove_item_at(DYNARR_NAME* s,  \
                                                       int index){      \
    int size = DYNARR_NAME ## _size(s);                                 \
    DYNARR_TYPE * a =                                                   \
      DYNARR_NAME ## _current_raw_array(s);                             \
    for(int i = index; i < size -1; i++){                               \
      a[i] = a[i + 1];                                                  \
    }                                                                   \
    DYNARR_NAME ## _drop_last(s, 1);                                    \
    return s;                                                           \
  }                                                                     \
                                                                        \
  MODIFIER DYNARR_NAME* DYNARR_NAME ## _remove(DYNARR_NAME* s,          \
                                               DYNARR_TYPE item){       \
    int index = DYNARR_NAME ## _index_of(s, item);                      \
    if(index != -1) {                                                   \
      DYNARR_NAME ## _remove_item_at(s, index);                         \
    }                                                                   \
    return s;                                                           \
  }                                                                     \
                                                                        \
  MODIFIER DYNARR_NAME* DYNARR_NAME ## _insert_at(DYNARR_NAME* s,       \
                                                  int index,            \
                                                  DYNARR_TYPE item){    \
    DYNARR_NAME ## _push_void(s, 1);                                    \
    int size = DYNARR_NAME ## _size(s);                                 \
    DYNARR_TYPE * a =                                                   \
      DYNARR_NAME ## _current_raw_array(s);                             \
    for(int i = size -1; i > index; i--){                               \
      a[i] = a[i -1];                                                   \
    }                                                                   \
    a[index] = item;                                                    \
    return s;                                                           \
  }                                                                     \
                                                                        \
  MODIFIER DYNARR_NAME* DYNARR_NAME ## _push_first(DYNARR_NAME* s,      \
                                                   DYNARR_TYPE item){   \
    DYNARR_NAME ## _insert_at(s, 0, item);                              \
    return s;                                                           \
  }                                                                     \
                                                                        \
  MODIFIER DYNARR_NAME* DYNARR_NAME ## _insert_before(DYNARR_NAME* s,   \
                                                      DYNARR_TYPE before_item, \
                                                      DYNARR_TYPE item){ \
    int index = DYNARR_NAME ## _index_of(s, before_item);               \
    if(index != -1) {                                                   \
      DYNARR_NAME ## _insert_at(s, index, item);                        \
    }                                                                   \
    return s;                                                           \
  }                                                                     \
                                                                        \
  MODIFIER DYNARR_NAME* DYNARR_NAME ## _insert_after(DYNARR_NAME* s,    \
                                                  DYNARR_TYPE after_item, \
                                                  DYNARR_TYPE item){    \
    int index = DYNARR_NAME ## _index_of(s, after_item);                \
    if(index != -1) {                                                   \
      DYNARR_NAME ## _insert_at(s, index + 1, item);                    \
    }                                                                   \
    return s;                                                           \
  }

#define DECLARE_DYNARR_DS(DYNARR_TYPE, MODIFIER, MALLOC_FUN_NAME, FREE_FUN_NAME, DEFAULT_CAPACITY) \
  _DECLARE_DYNARR_DS(DYNARR_TYPE, DYNARR_TYPE ## _dynarr, MODIFIER, MALLOC_FUN_NAME, FREE_FUN_NAME, DEFAULT_CAPACITY)


#endif
