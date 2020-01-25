#include <stdio.h>
#include <assert.h>

#ifndef __TEST_FRAMEWORK_H__
#define __TEST_FRAMEWORK_H__


int test(int (*testfun)(), char msg[]){
    printf("TEST: %s\n", msg);
    if(testfun()){
      printf("\033[32m -- SUCCESS! -- \033[m\n");
      return 1;
    }else{
      printf("\033[31m -- FAIL! -- \033[m\n");
      return 0;
    }
}


#endif
