#include <iostream>
#include <string>
#include <map>
#include <tuple>
#include <deque>
#include <stdexcept>
using namespace std;

enum State { q0=1, q1=2, q2=3, q3=0 }; // machine states; halt states = 0
enum TapeSym { _, _0_0, _0_1, _1_0, _1_1, _1, _0 }; // Tape symbols
enum Dir { L, R }; // Tape direction
typedef map<tuple<State, TapeSym>, tuple<State, TapeSym, Dir>> program_t;
typedef deque<TapeSym> Tape;
ostream& operator << (ostream& os, TapeSym sym) {
  switch (sym) {
    case    _: os << "_"; break;
    case _0_0: os << "(0,0)"; break;
    case _0_1: os << "(0,1)"; break;
    case _1_0: os << "(1,0)"; break;
    case _1_1: os << "(1,1)"; break;
    case   _1: os << "1"; break;
    case   _0: os << "0"; break;
  }
  return os;
}

class TM {
  program_t transitions;
public:
  TM(program_t transitions_): transitions{transitions_} {}

  Tape& operator()(Tape& tape) {
    State state = q0;
    size_t idx = 0;
    while (state != 0) {
      try {
        auto[new_state, new_symbol, new_direction] = transitions.at({state, tape.at(idx)});
        tape.at(idx) = new_symbol; // update tape
        state = new_state;         // update state
        if (new_direction == L) {  // move in new direction
          if (idx == 0) {
            tape.push_front(_);
          } else
            --idx;
        }
        else {
          ++idx;
          if (idx == tape.size()) tape.push_back(_);
        }
      } catch (exception& e) { throw logic_error( "machine crashed." ); }
    }
    return tape;
  }
};

ostream& operator << (ostream& os, Tape tape) {
  os << "... ";
  for (auto sym: tape)
    os << sym << ", ";
  os << "...";
  return os;
}

int main() {
  program_t bin_add {
      {{q0, _0_0}, {q0, _0_0, R}}, // program to add two binary numbers
      {{q0, _0_1}, {q0, _0_1, R}}, // (transition function)
      {{q0, _1_0}, {q0, _1_0, R}},
      {{q0, _1_1}, {q0, _1_1, R}},
      {{q0,  _},   {q1, _,    L}},

      {{q1, _0_0}, {q1, _0,  L}},
      {{q1, _0_1}, {q1, _1,  L}},
      {{q1, _1_0}, {q1, _1,  L}},
      {{q1, _1_1}, {q2, _0,  L}},
      {{q1,   _ }, {q3, _,   R}},

      {{q2, _1_1},  {q2, _1, L}},
      {{q2, _1_0},  {q2, _0, L}},
      {{q2, _0_1},  {q2, _0, L}},
      {{q2, _0_0},  {q1, _1, L}},
      {{q2,   _ },  {q3, _1, R}}
  };

  TM tm( bin_add ); // create machine
  Tape tape { _1_1, _0_0, _0_0 }; // add 0b100 + 0b100

  cout << "Input tape: " << endl;
  cout << tape << endl;

  tm( tape ); // execute on input

  cout << "Output tape: " << endl;
  cout << tape << endl;
}
