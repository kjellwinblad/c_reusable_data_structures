#include "test_framework.h"

#include "dynarr.h"
#include <stdlib.h>
#include <stdbool.h>


DECLARE_DYNARR_DS(int, static, malloc, free, 1)

static int test_dynarr(){
  int_dynarr* s = int_dynarr_new();
  int_dynarr_push(s, 3);
  int_dynarr_push(s, 6);
  int_dynarr_push(s, 10);
  int_dynarr_push(s, 14);
  int_dynarr_push(s, 20);
  if(int_dynarr_size(s) == 5 &&
     int_dynarr_pop(s) == 20 &&
     int_dynarr_size(s) == 4 &&
     int_dynarr_pop(s) == 14 &&
     int_dynarr_top(s) == 10 &&
     int_dynarr_pop(s) == 10 &&
     int_dynarr_pop(s) == 6 &&
     int_dynarr_pop(s) == 3 &&
     int_dynarr_size(s) == 0) {
  } else {
    return false;
  }
  int_dynarr_push_array(s, 3, (int[]){0,1,2});
  if(int_dynarr_size(s) == 3 &&
     int_dynarr_pop(s) == 2 &&
     int_dynarr_pop(s) == 1 &&
     int_dynarr_pop(s) == 0 &&
     int_dynarr_size(s) == 0) {
  } else {
    return false;
  }
  int_dynarr_push(s, 0);
  int* array = int_dynarr_push_void(s, 100);
  for(int i = 0; i < 100;i++){
    array[i] = i + 1;
  }
  int* whole_array = int_dynarr_current_raw_array(s);
  for(int i = 0; i < 101;i++){
    if(whole_array[i] != i){
      return false;
    }
  }
  if(int_dynarr_size(s) != 101){
    return false;
  }
  int_dynarr_free(s);
  return true;
}

int main(){
  int res = true;
  res = res && test(test_dynarr, "test_dynarr");
  return !res;
}
