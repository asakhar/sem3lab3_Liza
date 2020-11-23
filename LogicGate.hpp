#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
/**
 * @brief Represents gate's terminal
 *
 */
struct Terminal
{
  bool isOutput;
  unsigned short conn_num;
  /**
   * @brief 0 - Low, 1 - High, 2 - Undefined
   *
   */
  unsigned short state;
  /**
   * @brief Construct a new Terminal object
   *
   * @param isout Is terminal an output terminal?
   * @param conns Number of connections
   * @param _state Current state
   */
  Terminal(bool isout = false, unsigned short conns = 0, unsigned short _state = 0)
      : isOutput{isout}, conn_num{conns}, state{_state}
  {
  }
  /**
   * @brief Increases number of connections if its possible
   * 
   * @return unsigned short const reference to value
   */
  unsigned short const& connect()
  {
    if ((isOutput && conn_num < 3) || (!isOutput && conn_num == 0))
      return ++conn_num;
    throw std::runtime_error("Number of connections can't be increased!");
  }
  /**
   * @brief Decreases number of connections if its possible
   * 
   * @return unsigned short const reference to value
   */
  unsigned short const& disconnect()
  {
    if (conn_num > 0)
      return --conn_num;
    throw std::runtime_error("Can not disconnect! No connections");
  }

  /**
   * @brief Input state from stream
   * 
   * @param stream
   * @return std::istream& 
   */
  std::istream& input(std::istream& stream = std::cin)
  {
    char tmp;
    goto start;
  retry:
    std::cout << "Retry>";
  start:
    stream >> tmp;
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
      goto retry;
    }
    return stream;
  }
};

/**
 * @brief Gate representation
 * 
 * @tparam N compile-time defined size of terminals array
 */
template <const size_t N>
struct Gate
{
  Terminal terminals[N];
  size_t size;
  /**
   * @brief Construct a new Gate object (default type = invertor)
   * 
   */
  Gate() : terminals{{false, 0, 0}, {true, 0, 1}}, size{2} {}
  /**
   * @brief Construct a new Gate object
   * 
   * @param in number of input connections
   * @param out number of output connections
   */
  Gate(size_t in, size_t out)
  {
    for (size_t i = 0; i < in; i++)
      terminals[i] = Terminal(false, 0, 0);
    for (size_t i = in; i < (size = in + out); i++)
      terminals[i] = Terminal(true, 0, 0);
  }
  /**
   * @brief Construct a new Gate object
   * 
   * @param terms contents
   */
  Gate(Terminal terms[N]) : terminals{terms}, size{N} {};
  /**
   * @brief Construct a new Gate object
   * 
   * @param terms vector of terminals to construct from
   */
  Gate(std::vector<Terminal>& terms)
  {
    for (size = 0; size < N && size < terms.size(); size++)
    {
      terminals[size] = terms[size];
    }
  }

  /**
   * @brief Set the Terminal State
   * 
   * @param n terminal index
   * @param val value to set
   * @return unsigned short const ref to value
   */
  unsigned short const& setTerminalState(size_t n, unsigned short val)
  {
    if (val < 3)
      return terminals[n].state = val;
    return terminals[n].state;
  }
  /**
   * @brief Get the Terminal State
   * 
   * @param n terminal index
   * @return unsigned short const ref to value 
   */
  unsigned short const& getTerminalState(size_t n) { return terminals[n].state; }
  /**
   * @brief Increase number of connections for terminal by index
   * 
   * @param n index
   */
  void connect(size_t n) { terminals[n].connect(); }
  /**
   * @brief Decrease number of connections for terminal by index
   * 
   * @param n index
   */
  void disconnect(size_t n) { terminals[n].disconnect(); }
  /**
   * @brief Add terminal to gate
   * 
   * @param term rvalue ref to terminal to be added
   * @return Gate<N>& 
   */
  Gate<N>& addTerminal(Terminal&& term)
  {
    if (size == N)
      throw std::runtime_error("Overflow");
    terminals[size++] = term;
    return *this;
  }
  operator bool() = delete;
  operator int()  = delete;

  /**
   * @brief Input states from stream
   * 
   * @param stream 
   * @return std::istream& 
   */
  std::istream& input(std::istream& stream = std::cin)
  {
    for (size_t i = 0; i < size; i++)
    {
      std::cout << "Enter state for terminal#" << i + 1 << (terminals[i].isOutput ? " (Output)>" : " (Input)>");
      stream >> terminals[i].state;
    }
    return stream;
  }

  /**
   * @brief Output gate to stream
   * 
   * @param stream 
   * @return std::ostream& 
   */
  std::ostream& output(std::ostream& stream = std::cout)
  {
    stream << "Inputs:  ";
    for (size_t i = 0; i < size; i++)
      if (!terminals[i].isOutput)
        stream << (((terminals[i].state == 2) ? 'X' : static_cast<char>('0' + terminals[i].state)));
    stream << "\nOutputs: ";
    for (size_t i = 0; i < size; i++)
      if (terminals[i].isOutput)
        stream << (((terminals[i].state == 2) ? 'X' : static_cast<char>('0' + terminals[i].state)));
    return stream;
  }
};
