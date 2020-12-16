#include "LogicGateDynamic.hpp"

Terminal::Terminal(bool isout, unsigned short conns, unsigned short _state) : isOutput{isout}, conn_num{conns}, state{_state} {}

unsigned short const& Terminal::connect()
{
  if ((isOutput && conn_num < 3) || (!isOutput && conn_num == 0))
    return ++conn_num;
  throw std::runtime_error("Number of connections can't be increased!");
}

unsigned short const& Terminal::disconnect()
{
  if (conn_num > 1)
    return --conn_num;
  throw std::runtime_error("Can not disconnect! No connections");
}

std::istream& operator>>(std::istream& stream, Terminal& term)
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

Gate::Gate() : _size(2)
{
  terminals.reset(new Terminal[_size]);
  terminals[0] = {false, 0, 0};
  terminals[1] = {true, 0, 1};
}

Gate::Gate(size_t in, size_t out) : _size(in + out)
{
  terminals.reset(new Terminal[_size]);
  for (size_t i = 0; i < in; i++)
    terminals[i] = {false, 0, 0};
  for (size_t i = in; i < _size; i++)
    terminals[i] = {true, 0, 0};
}

Gate::Gate(std::vector<Terminal> terms) : _size(terms.size())
{
  terminals.reset(new Terminal[_size]);
  for (size_t i = 0; i < _size; i++)
    terminals[i] = terms[i];
};

Gate::Gate(Gate const& gt)
{
  _size = gt._size;
  terminals.reset(new Terminal[gt._size]);
  for (size_t i = 0; i < _size; i++)
    terminals[i] = gt.terminals[i];
}

Gate::Gate(Gate&& gt)
{
  _size     = gt._size;
  terminals = std::move(gt.terminals);
}

Gate& Gate::operator=(Gate const& gt)
{
  _size = gt._size;
  terminals.reset(new Terminal[gt._size]);
  for (size_t i = 0; i < _size; i++)
    terminals[i] = gt.terminals[i];
  return *this;
}

unsigned short const& Gate::operator()(size_t n, unsigned short val)
{
  if (n >= _size)
    throw std::out_of_range("");
  return terminals[n].state = val;
}

unsigned short const& Gate::operator[](size_t n) { return terminals[n].state; }

unsigned short const& Gate::at(size_t n)
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

Gate& Gate::operator+=(Terminal&& term)
{
  terminals.reset((Terminal*)std::realloc(terminals.release(), _size + 1));
  terminals[_size++] = term;
  return *this;
}

std::istream& operator>>(std::istream& stream, Gate& gate)
{
  for (size_t i = 0; i < gate._size; i++)
  {
    if (&stream == &std::cin)
      std::cout << "Enter state for terminal#" << i + 1 << (gate.terminals[i].isOutput ? " (Output)>" : " (Input)>");
    stream >> gate.terminals[i];
  }
  return stream;
}

std::ostream& operator<<(std::ostream& stream, Gate& gate)
{
  stream << "Inputs:  ";
  for (size_t i = 0; i < gate._size; i++)
    if (!gate.terminals[i].isOutput)
      stream << (((gate.terminals[i].state == 2) ? "  X   " : (gate.terminals[i].state ? " High " : " Low  ")));
  stream << "\nOutputs: ";
  for (size_t i = 0; i < gate._size; i++)
    if (gate.terminals[i].isOutput)
      stream << (((gate.terminals[i].state == 2) ? "  X   " : (gate.terminals[i].state ? " High " : " Low  ")));
  return stream;
}