// Choose which tutorial video will be built
#define PART5

#ifdef PART2
#include "Part2.h"
#endif
#ifdef PART3
#include "Part3.h"
#endif
#ifdef PART3CHALLENGE
#include "Part3Challenge.h"
#endif
#ifdef PART4
#include "Part4.h"
#endif
#ifdef PART4CHALLENGE
#include "Part4Challenge.h"
#endif
#ifdef PART5
#include "Part5.h"
#endif
#ifdef PART5CHALLENGE
#include "Part5Challenge.h"
#endif
#ifdef PART6
#include "Part6.h"
#endif
#ifdef PART7
#include "Part7.h"
#endif
#ifdef PART8
#include "Part8.h"
#endif
#ifdef PART9
#include "Part9.h"
#endif
#ifdef PART10
#include "Part10.h"
#endif
#ifdef PART11
#include "Part11.h"
#endif
#ifdef PART12
#include "Part12.h"
#endif

void setup() {
  #ifdef PART2
  setup2();
  #endif
  #ifdef PART3
  setup3();
  #endif
  #ifdef PART3CHALLENGE
  setup3challenge();
  #endif
  #ifdef PART4
  setup4();
  #endif
  #ifdef PART4CHALLENGE
  setup4challenge();
  #endif
  #ifdef PART5
  setup5();
  #endif
  #ifdef PART5CHALLENGE
  setup5challenge();
  #endif
  #ifdef PART6
  setup6();
  #endif
  #ifdef PART7
  setup7();
  #endif
  #ifdef PART8
  setup8();
  #endif
  #ifdef PART9
  setup9();
  #endif
  #ifdef PART10
  setup10();
  #endif
  #ifdef PART11
  setup11();
  #endif
  #ifdef PART12
  setup12();
  #endif
}

void loop() {
  #ifdef PART2
  loop2();
  #endif
  #ifdef PART3
  loop3();
  #endif
  #ifdef PART3CHALLENGE
  loop3challenge();
  #endif
  #ifdef PART4
  loop4();
  #endif
  #ifdef PART4CHALLENGE
  loop4challenge();
  #endif
  #ifdef PART5
  loop5();
  #endif
  #ifdef PART5CHALLENGE
  loop5challenge();
  #endif
  #ifdef PART6
  loop6();
  #endif
  #ifdef PART7
  loop7();
  #endif
  #ifdef PART8
  loop8();
  #endif
  #ifdef PART9
  loop9();
  #endif
  #ifdef PART10
  loop10();
  #endif
  #ifdef PART11
  loop11();
  #endif
  #ifdef PART12
  loop12();
  #endif
}
