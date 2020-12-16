#include "LogicGate.hpp"

Terminal::Terminal(bool isout, unsigned short conns, unsigned short _state) : isOutput{isout}, conn_num{conns}, state{_state} {}

unsigned short const& Terminal::connect()
{
  if ((isOutput && conn_num < 3) || (!isOutput && conn_num == 0))
    return ++conn_num;
  throw std::runtime_error("Number of connections can't be increased!");
}

unsigned short const& Terminal::disconnect()
{
  if (conn_num > 0)
    return --conn_num;
  throw std::runtime_error("Can not disconnect! No connections");
}

std::istream& Terminal::input(std::istream& stream)
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
      state = 0;
      break;
    case '1':
      state = 1;
      break;
    case 'X':
      state = 2;
      break;
    default:
      std::cout << "Retry>";
      flag = 1;
      break;
    }
  }
  return stream;
}

Gate::Gate() : terminals{{false, 0, 0}, {true, 0, 1}}, _size{2} {}

Gate::Gate(size_t in, size_t out)
{
  if (in + out > N)
    throw std::runtime_error("Overflow");
  for (_size = 0; _size < in; _size++)
    terminals[_size] = Terminal(false, 0, 0);
  for (; _size < in + out; _size++)
    terminals[_size] = Terminal(true, 0, 0);
}

Gate::Gate(Terminal terms[], size_t terms_size)
{
  if (terms_size > N)
    throw std::runtime_error("Array is too long.");
  _size = terms_size;
  for (size_t i = 0; i < _size; i++)
    terminals[i] = terms[i];
};

Gate::Gate(std::vector<Terminal> const& terms)
{
  for (_size = 0; _size < N && _size < terms.size(); _size++)
  {
    terminals[_size] = terms[_size];
  }
}

unsigned short const& Gate::setTerminalState(size_t n, unsigned short val)
{
  if (n >= _size)
    throw std::out_of_range("");
  if (val < 3)
    return terminals[n].state = val;
  return terminals[n].state;
}

unsigned short const& Gate::getTerminalState(size_t n)
{
  if (n >= _size)
    throw std::out_of_range("");
  return terminals[n].state;
}

void Gate::connect(size_t n)
{
  if (n >= _size)
    throw std::out_of_range("");
  terminals[n].connect();
}

void Gate::disconnect(size_t n)
{
  if (n >= _size)
    throw std::out_of_range("");
  terminals[n].disconnect();
}

Gate& Gate::addTerminal(Terminal&& term)
{
  if (_size == N)
    throw std::runtime_error("Overflow");
  terminals[_size++] = term;
  return *this;
}

std::istream& Gate::input(std::istream& stream)
{
  for (size_t i = 0; i < _size; i++)
  {
    if (&stream == &std::cin)
      std::cout << "Enter state for terminal#" << i + 1 << (terminals[i].isOutput ? " (Output)>" : " (Input)>");
    terminals[i].input(stream);
  }
  return stream;
}

std::ostream& Gate::output(std::ostream& stream)
{
  stream << "Inputs:  ";
  for (size_t i = 0; i < _size; i++)
    if (!terminals[i].isOutput)
      stream << (((terminals[i].state == 2) ? "  X   " : (terminals[i].state ? " High " : " Low  ")));
  stream << "\nOutputs: ";
  for (size_t i = 0; i < _size; i++)
    if (terminals[i].isOutput)
      stream << (((terminals[i].state == 2) ? "  X   " : (terminals[i].state ? " High " : " Low  ")));
  return stream;
}