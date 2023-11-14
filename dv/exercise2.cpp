#include <VExercise2.h>
#include <cstdint>
#include <catch2/catch_test_macros.hpp>
TEST_CASE(){
  VExercise2 model;
  model.clk=1;
  model.reset=1;
  uint16_t init=rand() % 0xFFFF;
  model.init=init;
  model.eval();
  model.clk=0;
  model.eval();
  
  REQUIRE((uint16_t)~model.out == init);

  uint16_t save=model.out;
  model.clk=1;
  model.reset=0;
  model.eval();
  for(int i=0;i<=500;i++){
    model.clk=0;
    model.eval();
    uint16_t change=((save>>10)^(save>>8)^(save>>3)^(save>>1))&1;
    save=(save<<1)|change;
    REQUIRE(model.out == save);
    model.clk=1;
    model.eval();
  }



}