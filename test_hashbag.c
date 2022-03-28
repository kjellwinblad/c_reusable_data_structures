#include <stdbool.h>

#include <stdlib.h>


#include "test_framework.h"
#include "hashbag.h"


bool long_eq(long* o1, long* o2) {   \
    return *o1 == *o2;  \
}

size_t long_hash_fun(long* o) {
   return (size_t)(*o); 
}

DECLARE_HASHBAG_DS(long, static, malloc, free, long_eq, long_hash_fun)

int test_insert_lookup(){
    long_hash* h = long_hash_new();
    long_hash_put(h, 10);
    int ok = NULL != long_hash_get(h, 10);
    ok = NULL == long_hash_get(h, 42);
    long_hash_free(h);
    return ok;
}

int test_insert_remove_lookup() {
    long_hash* h = long_hash_new();
    long_hash_put(h, 10);
    long_hash_remove(h, 10);
    int ok = NULL == long_hash_get(h, 10);
    long_hash_free(h);
    return ok;
}


int test_insert_many_grow() {
    long_hash* h = long_hash_new();
    long_hash_put(h, 42);
    int ok = 1;
    ok = ok && (NULL != long_hash_get(h, 42));
    for (int i = 0; i < 2; i++) {
        long_hash_put(h, i);
        ok = ok && (NULL != long_hash_get(h, 42));
    }
    long_hash_free(h); 
    return ok;
}

int test_delete_many_shrink() {
    long_hash* h = long_hash_new();
    long_hash_put(h, 42000);
    int ok = 1;
    for (int i = 0; i < 40000; i++) {
        long_hash_put(h, i);
        ok = ok && (NULL != long_hash_get(h, 42000));
    }
    for (int i = 0; i < 40000; i++) {
        long_hash_remove(h, i);
        ok = ok && (NULL != long_hash_get(h, 42000));
    }
    long_hash_free(h); 
    return ok;
}

int main(){
  int res = true;
  res = res && test(test_insert_lookup, "test_dynarr_as_stack");
  res = res && test(test_insert_remove_lookup, "test_insert_remove_lookup");
  res = res && test(test_insert_many_grow, "test_insert_many_grow");
  res = res && test(test_delete_many_shrink, "test_delete_many_shrink");
  return !res;
}
