#include <cstdint>
#include <random>
#include <catch2/catch_test_macros.hpp>
#include <VExercise3.h>

uint8_t mystery1(uint8_t a, uint8_t b, uint8_t c) {
  switch(a) {
    case 0:
      return ((b & 7) << 3) | ((c & 7));
    case 1:
      return 0b01000000|((c & 7) << 3) | ( b & 7);
    case 2:
      return b;
    default:
      return c;
  }
}

struct Mystery2 {
  uint8_t count {0};
  uint16_t out;

  uint16_t reset(uint8_t a, uint8_t b) {
    out = (b << 8) | a;
    count = 0;
    return out;
  }

  uint16_t step(uint8_t a, uint8_t b) {
    count %= 5;
    switch(count++) {
      case 0:
        return out = (a << 8) | (out & 0b11111111);
      case 1:
        return out = ((out & 0b1111111100000000) ) | b;
      case 2:
        return out = (out <<8) | (out >> 8);
      case 3:
        return out = (out<<12)| ((out & 0b11110000)<<4)|((out & 0b0000111100000000)>>4)|((out )>>12);
      default:
        return out = (std::popcount(out) & 1);
    }
  }
};

struct Excercise3Sim {
  Mystery2 state;

  uint16_t reset(uint8_t a, uint16_t b, uint16_t c) {
    uint8_t a_in {mystery1(a & 0x3, b & 0xFF, c & 0xFF)};
    uint8_t b_in {mystery1(a >> 2, b >> 8, c >> 8)};
    return state.reset(a_in, b_in);
  }

  uint16_t step(uint8_t a, uint16_t b, uint16_t c) {
    uint8_t a_in {mystery1(a & 0x3, b & 0xFF, c & 0xFF)};
    uint8_t b_in {mystery1(a >> 2, b >> 8, c >> 8)};
    return state.step(a_in, b_in);
  }
};

void step(VExercise3& model) {
  model.clk = 1;
  model.eval();
  model.clk = 0;
  model.eval();
};

TEST_CASE("Test Random") {
  VExercise3 model;
  Excercise3Sim sim;

  std::default_random_engine re {std::random_device {}()};
  std::uniform_int_distribution<uint8_t> rand4 {0, 15};
  std::uniform_int_distribution<uint16_t> rand16;

  model.a = rand4(re);
  model.b = rand16(re);
  model.c = rand16(re);
  model.out=0;
  model.reset = 1;
  step(model);

  REQUIRE(model.out == sim.reset(model.a, model.b, model.c));

  model.reset = 0;

  for(size_t cycles {0}; cycles < 100; ++cycles) {
    model.a = rand4(re);
    model.b = rand16(re);
    model.c = rand16(re);
    step(model);
    REQUIRE(model.out == sim.step(model.a, model.b, model.c));
  }
}