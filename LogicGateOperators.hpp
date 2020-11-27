#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

struct Terminal
{
  bool isOutput;
  unsigned short conn_num;
  /**
   * @brief 0 - Low, 1 - High, 2 - Undefined
   *
   */
  unsigned short state;

  Terminal(bool isout = false, unsigned short conns = 0, unsigned short _state = 0)
      : isOutput{isout}, conn_num{conns}, state{_state}
  {
  }
  unsigned short const& connect()
  {
    if ((isOutput && conn_num < 3) || (!isOutput && conn_num == 0))
      return ++conn_num;
    throw std::runtime_error("Number of connections can't be increased!");
  }
  unsigned short const& disconnect()
  {
    if (conn_num > 1)
      return --conn_num;
    throw std::runtime_error("Can not disconnect! No connections");
  }

  friend std::istream& operator>>(std::istream& stream, Terminal& term)
  {
    char tmp;
    bool flag = 1;
    while (flag)
    {
      stream >> tmp;
      flag = 0;
      switch (tmp)
      {
      case '0':
        term.state = 0;
        break;
      case '1':
        term.state = 1;
        break;
      case 'X':
        term.state = 2;
        break;
      default:
        std::cout << "Retry>";
        flag = 1;
        break;
      }
    }
    return stream;
  }
};
struct Gate
{
  static constexpr size_t N = 20;
  Terminal terminals[N];
  size_t size;
  Gate() : terminals{{false, 0, 0}, {true, 0, 1}}, size{2} {}
  Gate(size_t in, size_t out)
  {
    if (in + out > N)
      throw std::runtime_error("Overflow");
    for (size_t i = 0; i < in; i++)
      terminals[i] = Terminal(false, 0, 0);
    for (size_t i = in; i < (size = in + out); i++)
      terminals[i] = Terminal(true, 0, 0);
  }
  Gate(Terminal terms[N]) : terminals{terms}, size{N} {};

  unsigned short const& operator()(size_t n, unsigned short val)
  {
    if (n >= size)
      throw std::out_of_range("");
    return terminals[n].state = val;
  }
  unsigned short const& operator[](size_t n)
  {
    return terminals[n].state;
  }
  unsigned short const& at(size_t n)
  {
    if (n >= size)
      throw std::out_of_range("");
    return terminals[n].state;
  }
  void connect(size_t n)
  {
    if (n >= size)
      throw std::out_of_range("");
    terminals[n].connect();
  }
  void disconnect(size_t n)
  {
    if (n >= size)
      throw std::out_of_range("");
    terminals[n].disconnect();
  }
  Gate& operator+=(Terminal&& term)
  {
    if (size == N)
      throw std::runtime_error("Overflow");
    terminals[size++] = term;
    return *this;
  }
};

std::istream& operator>>(std::istream& stream, Gate& gate)
{
  for (size_t i = 0; i < gate.size; i++)
  {
    std::cout << "Enter state for terminal#" << i + 1 << (gate.terminals[i].isOutput ? " (Output)>" : " (Input)>");
    stream >> gate.terminals[i].state;
  }
  return stream;
}

std::ostream& operator<<(std::ostream& stream, Gate& gate)
{
  stream << "Inputs:  ";
  for (size_t i = 0; i < gate.size; i++)
    if (!gate.terminals[i].isOutput)
      stream << (((gate.terminals[i].state == 2) ? "  X   " : (gate.terminals[i].state?" High ":" Low  ")));
  stream << "\nOutputs: ";
  for (size_t i = 0; i < gate.size; i++)
    if (gate.terminals[i].isOutput)
      stream << (((gate.terminals[i].state == 2) ? "  X   " : (gate.terminals[i].state?" High ":" Low  ")));
  return stream;
}