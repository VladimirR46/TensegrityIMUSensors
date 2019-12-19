#ifndef __CONSOLETERMINAL_HPP
#define __CONSOLETERMINAL_HPP

#include <unordered_map>
#include <signal.h> //  our new library
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

volatile sig_atomic_t flag = 0;

UDPServer *udp_server;

struct command
{

    using args_type = std::vector<std::string>;

    virtual void exec(args_type const &args) const = 0;

    virtual ~command() {}
};
//------------------------------------------------------
struct help_command : command
{
    virtual void exec(args_type const &args) const override
    {
        std::cout << "----------------------------" << std::endl;
        if (args.size() == 1)
        {
            if (args[0] == "print")
            {
                std::cout << "print -m 0" << std::endl;
            }
        }
        else
        {
            std::cout << "hjkhj \n " << std::endl;
        }
    }
};
//------------------------------------------------------
struct start_command : command
{
    virtual void exec(args_type const &args) const override
    {
        for (int i = 0; i < udp_server->imuModules.size(); i++)
        {
            boost::posix_time::ptime utcCur = boost::posix_time::second_clock::local_time();
            //std::cout << utcCur << "\n";
            std::string Patch = to_simple_string(utcCur);
            boost::filesystem::create_directory("logs/" + Patch);

            udp_server->imuModules[i]->StartRec(Patch);
        }
    }
};
//------------------------------------------------------
struct stop_command : command
{
    virtual void exec(args_type const &args) const override
    {
        for (int i = 0; i < udp_server->imuModules.size(); i++)
        {
            udp_server->imuModules[i]->StopRec();
        }
    }
};
//------------------------------------------------------
struct ls_command : command
{
    virtual void exec(args_type const &args) const override
    {

        if (udp_server->imuModules.size() == 0)
            std::cout << "not found" << std::endl;

        for (int i = 0; i < udp_server->imuModules.size(); i++)
        {
            std::cout << i << " - " << udp_server->imuModules[i]->GetName() << " FPS: " << udp_server->imuModules[i]->GetFPS() << std::endl;
        }
        std::cout << std::endl;
    }
};
//------------------------------------------------------
void my_function(int sig)
{             // can be called asynchronously
    flag = 1; // set flag
    signal(SIGINT, my_function);
}

using command_ptr = std::unique_ptr<command>;
using command_map = std::unordered_map<std::string, command_ptr>;
command_map commandMap;

void InitCommand()
{
    commandMap["help"] = command_ptr(new help_command);
    commandMap["ls"] = command_ptr(new ls_command);
    commandMap["start"] = command_ptr(new start_command);
    commandMap["stop"] = command_ptr(new stop_command);
}

#endif