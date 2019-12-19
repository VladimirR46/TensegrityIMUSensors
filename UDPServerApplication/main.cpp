#include <cstdlib>
#include <iostream>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "UDPServer.hpp"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include "ConsoleTerminal.h"

using boost::asio::ip::tcp;

boost::asio::io_context io_context;

//----------------------------------------------------------------------------------------------------------------------------
void ThreadTerminal()
{
  setlocale(LC_ALL, "Russian");

  // Register signals
  //signal(SIGINT, my_function);

  InitCommand();

  for (std::string line; std::getline(std::cin, line);)
  {
    std::istringstream iss(line);

    std::string commandName;
    iss >> commandName;

    auto const it = commandMap.find(commandName);
    if (it != std::end(commandMap))
    {
      command::args_type const commandArgs(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{});
      it->second->exec(commandArgs);
    }
    else
    {
      std::cerr << "'" << commandName << "' - Unknown command" << std::endl;
    }
  }
}
//----------------------------------------------------------------------------------------------------------------------------
void TCPServerMatlab()
{

  while (1)
  {
    if (udp_server->imuModules.size() > 0)
      break;
    else
      boost::this_thread::sleep(boost::posix_time::millisec(500));
  }

  try
  {

    tcp::socket s(io_context);
    tcp::resolver resolver(io_context);
    boost::asio::connect(s, resolver.resolve("127.0.0.1", "4013"));

    while (1)
    {
      std::string mess = std::to_string(udp_server->dataIMU[0].q0) + " " +
                         std::to_string(udp_server->dataIMU[0].q1) + " " +
                         std::to_string(udp_server->dataIMU[0].q2) + " " +
                         std::to_string(udp_server->dataIMU[0].q3) + " " +

                         std::to_string(udp_server->dataIMU[1].q0) + " " +
                         std::to_string(udp_server->dataIMU[1].q1) + " " +
                         std::to_string(udp_server->dataIMU[1].q2) + " " +
                         std::to_string(udp_server->dataIMU[1].q3) + " " +

                         std::to_string(udp_server->dataIMU[2].q0) + " " +
                         std::to_string(udp_server->dataIMU[2].q1) + " " +
                         std::to_string(udp_server->dataIMU[2].q2) + " " +
                         std::to_string(udp_server->dataIMU[2].q3) + '\n'; // + '\n';

      boost::asio::write(s, boost::asio::buffer(mess, mess.length()));
      boost::this_thread::sleep(boost::posix_time::millisec(20));
    }
  }
  catch (std::exception &e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}
//----------------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  /*
  pt::ptime current_date_microseconds = pt::microsec_clock::local_time();
  long milliseconds = current_date_microseconds.time_of_day().total_milliseconds();
  pt::time_duration current_time_milliseconds = pt::milliseconds(milliseconds);
  pt::ptime current_date_milliseconds(current_date_microseconds.date(),
                                      current_time_milliseconds);
  std::cout << current_date_microseconds << " Milliseconds: " << current_date_milliseconds.time_of_day().total_microseconds() << std::endl;
*/

  boost::thread TT(ThreadTerminal);

  if (!boost::filesystem::exists("logs"))
    boost::filesystem::create_directory("logs");

  try
  {
    //server = new UDPServer(io_context, 4442);
    UDPServer server(io_context, 4442);
    udp_server = &server;

    //boost::thread TCPThread(TCPServerMatlab);

    io_context.run();
  }
  catch (std::exception &e)
  {
    std::cerr << "Exception:  " << e.what() << "\n";
    //LOGE << "Exception:  " << e.what();
  }

  return 0;
}