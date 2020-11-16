#include "LogicGate.hpp"
int main() {
  Gate<12> gate;
  std::cout << gate;
  
  // std::cout.operator<<()
}
// template <size_t Size>
// struct Gate
// {
//   Terminal terminals[Size];
//   //std::function<void(Terminal[Size])> mapping;
//   Gate()
//   {
//     terminals[0] = Terminal();
//     terminals[1] = Terminal(true);
//     // mapping = [](Terminal terms[Size]) {
//     //   for (size_t i = 0; i+1 < Size; i+=2)
//     //   {
//     //     if (terms[i].state == Terminal::SignalState_t::Low)
//     //       terms[i+1].state = Terminal::SignalState_t::High;
//     //     else
//     //       terms[i+1].state = Terminal::SignalState_t::Low;
//     //   }
//     // };
//     // Invoke();
//   }
//   void Invoke()
//   {
//     for (size_t i = 0; i < Size; i++)
//       if (!terminals[i].isOutput && terminals[i].connection)
//         terminals[i].state = *terminals[i].connection;
//     mapping(terminals);
//   }
// };

// int main()
// {
//   Gate<2> invertor;
//   Terminal::SignalState_t a = Terminal::SignalState_t::High;
//   invertor.terminals[0].connection = &a;

//   //invertor.terminals[0].state = Terminal::SignalState_t::High;
//   invertor.Invoke();
//   a = Terminal::SignalState_t::Low;
//   //invertor.terminals[0].state = Terminal::SignalState_t::Low;
//   invertor.Invoke();
// }