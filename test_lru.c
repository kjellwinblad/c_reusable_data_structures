
#include <stdbool.h>

#include <stdlib.h>


#include "test_framework.h"
#include "lru.h"

bool long_eq(long* o1, long* o2) {
    return *o1 == *o2; 
}

size_t long_hash(long* o) {
   return (size_t)(*o); 
}

void long_destroy(long* o) {
   (void)o; 
}

bool shall_eject(size_t current_size, long* value) {
    (void)value;
    return current_size > 10;
}
DECLARE_LRUCACHE_DS(long, static, malloc, free, long_eq, long_hash, long_destroy, shall_eject)


int test_create_delete(){
    long_lru* l = long_lru_new();
    long_lru_free(l);
    return 1;
}

int test_insert_get(){
    long_lru* l = long_lru_new();
    long_lru_add(l, 10);
    int ok = 1;
    ok = ok && NULL == long_lru_get(l, 9);
    long* res = long_lru_get(l, 10);
    ok = ok && 10 == *res;
    long_lru_add(l, 42);
    ok = ok && NULL == long_lru_get(l, 9);
    res = long_lru_get(l, 10);
    ok = ok && 10 == *res;
    res = long_lru_get(l, 42);
    ok = ok && 42 == *res;
    long_lru_free(l);
    return ok;
}


int test_eject(){
    long_lru* l = long_lru_new();
    int ok = 1;
    for (long i = 0; i < 10; i++) {
        long_lru_add(l, i);
        ok = ok && NULL == long_lru_get(l, 42);
    }
    for (long i = 0; i < 10; i++) {
        long* res = long_lru_get(l, i);
        ok = ok && i == *res;
    }
    long_lru_add(l, 10);
    // 0 should have got ejected
    ok = ok && NULL == long_lru_get(l, 0);
    long* res = long_lru_get(l, 1);
    ok = ok && 1 == *res;
    for (long i = 0; i < 1000; i++) {
        long_lru_add(l, i);
    }
    // Only the the 10 last should be in the LRU
    for (long i = 0; i < (1000 -10); i++) {
        ok = ok && NULL == long_lru_get(l, i);
    }
    for (long i = (1000 -10); i < 1000; i++) {
        long* res = long_lru_get(l, i);
        ok = ok && i == *res;
    }
    long_lru_free(l);
    return ok;
}


int test_get_move_to_front(){
    long_lru* l = long_lru_new();
    int ok = 1;
    for (long i = 0; i < 10; i++) {
        long_lru_add(l, i);
        ok = ok && NULL == long_lru_get(l, 42);
    }
    for (long i = 0; i < 10; i++) {
        long* res = long_lru_get(l, i);
        ok = ok && i == *res;
    }
    // move 0 to front
    long* res = long_lru_get(l, 0);
    ok = ok && 0 == *res;
    long_lru_add(l, 10);
    // 1 should have got ejected
    ok = ok && NULL == long_lru_get(l, 1);
    // move 0 to front again
    res = long_lru_get(l, 0);
    ok = ok && 0 == *res;
    for (long i = 11; i < 20; i++) {
        long_lru_add(l, i);
        ok = ok && NULL == long_lru_get(l, 42);
    }
    res = long_lru_get(l, 0);
    ok = ok && 0 == *res;
    for (long i = 1; i < 11; i++) {
        ok = ok && NULL == long_lru_get(l, i);
    }
    res = long_lru_get(l, 0);
    ok = ok && 0 == *res;
    for (long i = 11; i < 20; i++) {
        long* res = long_lru_get(l, i);
        ok = ok && i == *res;
    }
    long_lru_add(l, 10);
    ok = ok && NULL == long_lru_get(l, 0);
    long_lru_free(l);
    return ok;
}


int main(){
  int res = true;
  res = res && test(test_create_delete, "test_create_delete");
  res = res && test(test_insert_get, "test_insert_get");
  res = res && test(test_eject, "test_eject");
  res = res && test(test_get_move_to_front, "test_get_move_to_front");
  return !res;
}
