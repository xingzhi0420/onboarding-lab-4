#include <VExercise4.h>
#include <cstdint>
#include <random>
#include <catch2/catch_test_macros.hpp>
// void step(VExercise4& model) {
//   model.clk = 1;
//   model.eval();
//   model.clk = 0;
//   model.eval();
// };

TEST_CASE("Test Random1") {
  VExercise4 model;

  std::default_random_engine re {std::random_device {}()};
  std::uniform_int_distribution<uint8_t> rand8;
  model.alpha=rand8(re);
  model.beta=rand8(re);
  model.gamma=rand8(re);
  model.cs=0;
  for (uint8_t i=0;i<=7;i++){
    model.sel=i;
    model.eval();
    REQUIRE(model.out==0);
  }
  model.cs=1;
  model.sel=0;
  model.eval();
  REQUIRE(model.out==model.alpha);
  model.sel=1;
  model.eval();
  REQUIRE(model.out==model.beta);
  model.sel=2;
  model.eval();
  REQUIRE(model.out==model.gamma);
  model.sel=3;
  model.eval();
  REQUIRE(model.out==(model.alpha & (model.beta|model.gamma )));

}
