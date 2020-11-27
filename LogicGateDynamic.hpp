#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
/**
 * @brief Gate's terminal
 * 
 */
struct Terminal
{
  /**
   * @brief Determines whether terminal is output or input 
   * 
   */
  bool isOutput;
  /**
   * @brief Number of connections (3max for output teminal | 1max for input terminal)
   * 
   */
  unsigned short conn_num;
  /**
   * @brief Current terminal state (0 - Low, 1 - High, 2 - Undefined)
   *
   */
  unsigned short state;
  /**
   * @brief Construct a new Terminal object
   * 
   * @param isout is terminal output
   * @param conns number of connections
   * @param _state initial state
   */
  Terminal(bool isout = false, unsigned short conns = 0, unsigned short _state = 0)
      : isOutput{isout}, conn_num{conns}, state{_state}
  {
  }
  /**
   * @brief Increase number of connections
   * 
   * @return unsigned short const& 
   */
  unsigned short const& connect()
  {
    if ((isOutput && conn_num < 3) || (!isOutput && conn_num == 0))
      return ++conn_num;
    throw std::runtime_error("Number of connections can't be increased!");
  }
  /**
   * @brief Decrease number of connections
   * 
   * @return unsigned short const& 
   */
  unsigned short const& disconnect()
  {
    if (conn_num > 1)
      return --conn_num;
    throw std::runtime_error("Can not disconnect! No connections");
  }
  /**
   * @brief Input terminal state
   * 
   * @param stream e.g. std::cin
   * @param term terminal to input
   * @return std::istream& 
   */
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
/**
 * @brief Logical Gate
 * 
 */
struct Gate
{
  /**
   * @brief Terminals of gate
   * 
   */
  std::unique_ptr<Terminal[]> terminals;
  /**
   * @brief Number of terminals
   * 
   */
  size_t size;
  /**
   * @brief Construct a new Gate object (default)
   * 
   */
  Gate() : size(2)
  {
    terminals.reset(new Terminal[size]);
    terminals[0] = {false, 0, 0};
    terminals[1] = {true, 0, 1};
  }
  /**
   * @brief Construct a new Gate object
   * 
   * @param in number of input terminals
   * @param out number of output terminals
   */
  Gate(size_t in, size_t out) : size(in + out)
  {
    terminals.reset(new Terminal[size]);
    for (size_t i = 0; i < in; i++)
      terminals[i] = {false, 0, 0};
    for (size_t i = in; i < size; i++)
      terminals[i] = {true, 0, 0};
  }
  /**
   * @brief Construct a new Gate object
   * 
   * @param terms vector of terminals
   */
  Gate(std::vector<Terminal> terms) : size(terms.size())
  {
    terminals.reset(new Terminal[size]);
    for (size_t i = 0; i < size; i++)
      terminals[i] = terms[i];
  };
  /**
   * @brief Copy-construct a new Gate object
   * 
   * @param gt object to copy
   */
  Gate(Gate const& gt)
  {
    size = gt.size;
    terminals.reset(new Terminal[gt.size]);
    for (size_t i = 0; i < size; i++)
      terminals[i] = gt.terminals[i];
  }
  /**
   * @brief Move-construct a new Gate object
   * 
   * @param gt object to move
   */
  Gate(Gate&& gt)
  {
    size      = gt.size;
    terminals = std::move(gt.terminals);
  }
  /**
   * @brief Copy-assignment operator
   * 
   * @param gt object to copy
   * @return Gate& 
   */
  Gate& operator=(Gate const& gt)
  {
    size = gt.size;
    terminals.reset(new Terminal[gt.size]);
    for (size_t i = 0; i < size; i++)
      terminals[i] = gt.terminals[i];
  }
  /**
   * @brief Set terminal's state by index n
   * 
   * @param n index
   * @param val value to be set
   * @return unsigned short const& 
   */
  unsigned short const& operator()(size_t n, unsigned short val)
  {
    if (n >= size)
      throw std::out_of_range("");
    return terminals[n].state = val;
  }
  /**
   * @brief Get terminal's state by index n (without boundary checks)
   * 
   * @param n index
   * @return unsigned short const& 
   */
  unsigned short const& operator[](size_t n) { return terminals[n].state; }
  /**
   * @brief Get terminal's state by index n (with boundary cheking)
   * 
   * @param n index
   * @return unsigned short const& 
   */
  unsigned short const& at(size_t n)
  {
    if (n >= size)
      throw std::out_of_range("");
    return terminals[n].state;
  }
  /**
   * @brief Increase number of connections of terminal by index n
   * 
   * @param n index
   */
  void connect(size_t n)
  {
    if (n >= size)
      throw std::out_of_range("");
    terminals[n].connect();
  }
  /**
   * @brief Decrease number of connections of terminal by index n
   * 
   * @param n index
   */
  void disconnect(size_t n)
  {
    if (n >= size)
      throw std::out_of_range("");
    terminals[n].disconnect();
  }
  /**
   * @brief Add terminal to gate
   * 
   * @param term terminal to be added
   * @return Gate& 
   */
  Gate& operator+=(Terminal&& term)
  {
    terminals.reset((Terminal*)std::realloc(terminals.release(), size + 1));
    terminals[size++] = term;
    return *this;
  }
};
/**
 * @brief Input states of terminals from stream
 * 
 * @param stream 
 * @param gate 
 * @return std::istream& 
 */
std::istream& operator>>(std::istream& stream, Gate& gate)
{
  for (size_t i = 0; i < gate.size; i++)
  {
    std::cout << "Enter state for terminal#" << i + 1 << (gate.terminals[i].isOutput ? " (Output)>" : " (Input)>");
    stream >> gate.terminals[i].state;
  }
  return stream;
}

/**
 * @brief Formatted output of gate
 * 
 * @param stream 
 * @param gate 
 * @return std::ostream& 
 */
std::ostream& operator<<(std::ostream& stream, Gate& gate)
{
  stream << "Inputs:  ";
  for (size_t i = 0; i < gate.size; i++)
    if (!gate.terminals[i].isOutput)
      stream << (((gate.terminals[i].state == 2) ? "  X   " : (gate.terminals[i].state ? " High " : " Low  ")));
  stream << "\nOutputs: ";
  for (size_t i = 0; i < gate.size; i++)
    if (gate.terminals[i].isOutput)
      stream << (((gate.terminals[i].state == 2) ? "  X   " : (gate.terminals[i].state ? " High " : " Low  ")));
  return stream;
}