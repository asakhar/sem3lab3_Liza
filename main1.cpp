#include "LogicGate.hpp"
#include <map>
constexpr size_t SIZE = Gate::N;
typedef Gate LogicGate;
typedef std::map<std::string, LogicGate> GateMap;

void exit(GateMap& lg, std::string& sel) { exit(0); }
void new_gate(GateMap& lg, std::string& sel)
{
  std::cout << "Input gate name: ";
  std::string name;
  std::cin.get();
  std::getline(std::cin, name);
  std::cout << "Do you want to input terminals now?(0_/1) > ";
  char ch;
  std::cin >> ch;
  std::vector<Terminal> terms{};
  while (ch == '1' && terms.size() != SIZE)
  {
    std::cout << "Input iotype ('in':'out'): ";
    std::string ans;
    while (ans != "in" && ans != "out")
      std::cin >> ans;
    std::cout << "Input number of connections" << (ans == "out" ? "(from 0 to 3): " : "(0 or 1): ");
    unsigned short conns;
    std::cin >> conns;
    terms.push_back({ans == "out", conns});
    std::cout << "Input state (0,1 or X): ";
    terms.back().input();
    std::cout << "Do you want to continue?(0_/1) > ";
    std::cin >> ch;
  }
  lg[name] = LogicGate(terms);
  sel      = name;
  std::cout << "Successfully created!";
}

void remove_gate(GateMap& lg, std::string& sel)
{
  std::cout << "Input gate name: ";
  std::string name;
  std::cin.get();
  std::getline(std::cin, name);
  std::cout << (lg.erase(name) == 1 ? "Successfully removed!" : "Key not found!");
}

void print_gate(GateMap& lg, std::string& sel) { lg[sel].output(std::cout << sel <<" gate: \n"); }

void list_gates(GateMap& lg, std::string& sel)
{
  for (auto& keyval : lg)
  {
    std::cout << keyval.first << (&*lg.rbegin() == &keyval ? "" : ", ");
  }
  std::cout << "\n";
}

void select_gate(GateMap& lg, std::string& sel)
{
  std::cout << "Input gate name: ";
  std::string name;
  std::cin.get();
  std::getline(std::cin, name);
  if (lg.find(name) == lg.end())
  {
    std::cout << "Gate not found!";
    return;
  }
  sel = name;
  std::cout << "Gate '" << sel << "' selected!";
}

void add_terminals(GateMap& lg, std::string& sel)
{
  char ch = '1';
  while (ch == '1' && lg[sel].size != SIZE)
  {
    std::cout << "Input iotype ('in':'out'): ";
    std::string ans;
    while (ans != "in" && ans != "out")
      std::cin >> ans;
    std::cout << "Input number of connections" << (ans == "out" ? "(from 0 to 3): " : "(0 or 1): ");
    unsigned short conns;
    std::cin >> conns;

    Terminal term{ans == "out", conns};
    std::cout << "Input state (0,1 or X): ";
    term.input();
    lg[sel].addTerminal(std::move(term));
    std::cout << "Do you want to continue?(0_/1) > ";
    std::cin >> ch;
  }
}

void get_term_state(GateMap& lg, std::string& sel)
{
  size_t pos;
  while (pos >= lg[sel].size)
  {
    std::cout << "Input terminal number (from 0 to " << lg[sel].size - 1 << "): ";
    std::cin >> pos;
  }

  std::cout << "Terminal state: " << lg[sel].getTerminalState(pos);
}

void set_term_state(GateMap& lg, std::string& sel)
{
  size_t pos;
  while (pos >= lg[sel].size)
  {
    std::cout << "Input terminal number (from 0 to " << lg[sel].size - 1 << "): ";
    std::cin >> pos;
  }
  unsigned short st;
  std::cout << "Input terminal state: ";
  std::cin >> st;

  std::cout << "Terminal state set to: " << lg[sel].setTerminalState(pos, st);
}

void connect_term(GateMap& lg, std::string& sel)
{
  size_t pos;
  while (pos >= lg[sel].size)
  {
    std::cout << "Input terminal number (from 0 to " << lg[sel].size - 1 << "): ";
    std::cin >> pos;
  }
  try
  {
    lg[sel].connect(pos);
  }
  catch (std::runtime_error& e)
  {
    std::cout << e.what() << std::endl;
  }
}

void disconnect_term(GateMap& lg, std::string& sel)
{
  size_t pos;
  while (pos >= lg[sel].size)
  {
    std::cout << "Input terminal number (from 0 to " << lg[sel].size - 1 << "): ";
    std::cin >> pos;
  }
  try
  {
    lg[sel].disconnect(pos);
  }
  catch (std::runtime_error& e)
  {
    std::cout << e.what() << std::endl;
  }
}

void renew_states(GateMap& lg, std::string& sel) { lg[sel].input(); }

void (*options[])(GateMap&, std::string&) = {exit,           new_gate,     remove_gate,     list_gates,
                                             select_gate,    print_gate,   add_terminals,   get_term_state,
                                             set_term_state, connect_term, disconnect_term, renew_states};

int main()
{
  GateMap gates;
  std::string selected = "invertor";
  gates["invertor"]    = LogicGate{};

  while (1)
  {
    std::cout << "Ask... \n\
    [0]Exit\n\
    [1]New gate\n\
    [2]Remove gate\n\
    [3]List gates\n\
    [4]Select gate\n\
    [5]Print gate\n\
    [6]Add terminals\n\
    [7]Get terminal state\n\
    [8]Set terminal state\n\
    [9]Connect terminal\n\
    [10]Disconnect terminal\n\
    [11]Renew satates\n"
                 ">>";
    int choice;
    std::cin >> choice;
    if (choice > sizeof(options) / 8 - 1 || choice < 0)
    {
      std::cout << "Try again!\n";
      continue;
    }
    options[choice](gates, selected);
    std::cout << std::endl;
  }
}