#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
/*
 Represents gate's terminal
 */
struct Terminal
{
  bool isOutput;
  /*
 Connection number
     */
  unsigned short conn_num;
  /*
 0 - Low, 1 - High, 2 - Undefined
     */
  unsigned short state;
  /*
 Construct a new Terminal object
    isout - Is terminal an output terminal?
  conns - Number of connections
  _state - Current state
   */
  Terminal(bool isout = false, unsigned short conns = 0, unsigned short _state = 0);
  /*
 Increases number of connections if its possible
   */
  unsigned short const& connect();
  /*
 Decreases number of connections if its possible
   */
  unsigned short const& disconnect();
  /*
 Input state from stream
    stream
   */
  std::istream& input(std::istream& stream = std::cin);
};

/*
 Gate repr
 */
class Gate
{
public:
  static constexpr size_t N = 20;

private:
  /*
 Gate's terminals
     */
  Terminal terminals[N];
  /*
 Current number of terminals
     */
  size_t _size;

public:
  inline size_t size() { return _size; }
  /*
 Construct a new Gate object (default type = invertor)
     */
  Gate();
  /*
 Construct a new Gate object
    in - number of input connections
  out - number of output connections
   */
  Gate(size_t in, size_t out);
  /*
 Construct a new Gate object
    terms - contents
   */
  Gate(Terminal terms[], size_t terms_size);
  /*
 Construct a new Gate object
    terms - vector of terminals to construct from
   */
  Gate(std::vector<Terminal> const& terms);

  /*
 Set the Terminal State
    n - terminal index
  val - value to set
   */
  unsigned short const& setTerminalState(size_t n, unsigned short val);
  /*
 Get the Terminal State
    n - terminal index
   */
  unsigned short const& getTerminalState(size_t n);
  /*
 Increase number of connections for terminal by index
    n - index
   */
  void connect(size_t n);
  /*
 Decrease number of connections for terminal by index
    n - index
   */
  void disconnect(size_t n);
  /*
 Add terminal to gate
    term - rvalue ref to terminal to be added
   */
  Gate& addTerminal(Terminal&& term);

  /*
 Input states from stream
    stream
   */
  std::istream& input(std::istream& stream = std::cin);

  /*
 Output gate to stream
    stream
   */
  std::ostream& output(std::ostream& stream = std::cout);
};
