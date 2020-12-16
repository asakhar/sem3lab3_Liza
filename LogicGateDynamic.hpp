#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
/**
 *  Gate's terminal
 *
 */
struct Terminal
{
  /**
   *  Determines whether terminal is output or input
   *
   */
  bool isOutput;
  /**
   *  Number of connections (3max for output teminal | 1max for input terminal)
   *
   */
  unsigned short conn_num;
  /**
   *  Current terminal state (0 - Low, 1 - High, 2 - Undefined)
   *
   */
  unsigned short state;
  /**
   *  Construct a new Terminal object
   *
   *  isout is terminal output
   *  conns number of connections
   *  _state initial state
   */
  Terminal(bool isout = false, unsigned short conns = 0, unsigned short _state = 0);
  /**
   *  Increase number of connections
   *
   *  unsigned short const&
   */
  unsigned short const& connect();
  /**
   *  Decrease number of connections
   *
   *  unsigned short const&
   */
  unsigned short const& disconnect();
  /**
   *  Input terminal state
   *
   *  stream e.g. std::cin
   *  term terminal to input
   *  std::istream&
   */
  friend std::istream& operator>>(std::istream& stream, Terminal& term);
};
/**
 *  Logical Gate
 *
 */
class Gate
{
  /**
   *  Terminals of gate
   *
   */
  std::unique_ptr<Terminal[]> terminals;
  /**
   *  Number of terminals
   *
   */
  size_t _size;
  /**
   *  Construct a new Gate object (default)
   *
   */
public:
  inline size_t size() { return _size; }
  Gate();
  /**
   *  Construct a new Gate object
   *
   *  in number of input terminals
   *  out number of output terminals
   */
  Gate(size_t in, size_t out);
  /**
   *  Construct a new Gate object
   *
   *  terms vector of terminals
   */
  Gate(std::vector<Terminal> terms);
  /**
   *  Copy-construct a new Gate object
   *
   *  gt object to copy
   */
  Gate(Gate const& gt);
  /**
   *  Move-construct a new Gate object
   *
   *  gt object to move
   */
  Gate(Gate&& gt);
  /**
   *  Copy-assignment operator
   *
   *  gt object to copy
   *  Gate&
   */
  Gate& operator=(Gate const& gt);
  /**
   *  Set terminal's state by index n
   *
   *  n index
   *  val value to be set
   *  unsigned short const&
   */
  unsigned short const& operator()(size_t n, unsigned short val);
  /**
   *  Get terminal's state by index n (without boundary checks)
   *
   *  n index
   *  unsigned short const&
   */
  unsigned short const& operator[](size_t n);
  /**
   *  Get terminal's state by index n (with boundary cheking)
   *
   *  n index
   *  unsigned short const&
   */
  unsigned short const& at(size_t n);
  /**
   *  Increase number of connections of terminal by index n
   *
   *  n index
   */
  void connect(size_t n);
  /**
   *  Decrease number of connections of terminal by index n
   *
   *  n index
   */
  void disconnect(size_t n);
  /**
   *  Add terminal to gate
   *
   *  term terminal to be added
   *  Gate&
   */
  Gate& operator+=(Terminal&& term);

  /**
   *  Input states of terminals from stream
   *
   *  stream
   *  gate
   *  std::istream&
   */
  friend std::istream& operator>>(std::istream& stream, Gate& gate);

  /**
   *  Formatted output of gate
   *
   *  stream
   *  gate
   *  std::ostream&
   */
  friend std::ostream& operator<<(std::ostream& stream, Gate& gate);
};
