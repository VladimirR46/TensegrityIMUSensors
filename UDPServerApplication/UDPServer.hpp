#ifndef __UDPSERVER_HPP
#define __UDPSERVER_HPP

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
//#include <boost/array.hpp>
#include <vector>
#include "ImuModule.hpp"

//#include "plog/Log.h"

#define SIZE_BUFF 1024

using boost::asio::ip::udp;

class UDPServer
{
public:
    UDPServer(boost::asio::io_context &io_context, unsigned short port)
        : io_context_(io_context), socket_(io_context, udp::endpoint(udp::v4(), port))
    {

        std::cout << "Server started.... PORT: " << port << std::endl;
        //LOGD << "Starting asynchronous UDP server. Port: " << port;
        start_receive();
    }

    ~UDPServer()
    {
        for (int i = 0; i < imuModules.size(); i++)
            delete imuModules[i];
    }

    ImuModule::DataIMU dataIMU[3];
    std::vector<ImuModule *> imuModules;

private:
    void start_receive()
    {
        socket_.async_receive_from(boost::asio::buffer(boost::asio::buffer(recv_buff)), remote_endpoint_,
                                   boost::bind(&UDPServer::handle_receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    //-------------------------------------------------------------------------------------------
    void handle_receive(const boost::system::error_code &error, size_t bytes_recvd)
    {
        if (!error && bytes_recvd > 0)
        {
            bool Ident = false;

            for (int i = 0; i < imuModules.size(); i++)
            {
                if (imuModules[i]->GetIpAddress() == remote_endpoint_.address())
                {
                    memcpy(&dataIMU, recv_buff, sizeof(dataIMU));

                    imuModules[i]->Update(dataIMU);

                    Ident = true;
                    break;
                }
            }

            if (Ident == false && bytes_recvd < 20)
            {

                recv_buff[bytes_recvd] = 0;
                std::string str(recv_buff);

                std::vector<std::string> words;
                std::istringstream ist(str);
                std::string tmp;
                while (ist >> tmp)
                    words.push_back(tmp);

                //std::cout << words[0] << " !!!!!!! " << words[1] << "/n";

                if (words[0] == "Ident" /*strcmp(recv_buff, "Ident") == 0*/)
                {
                    ImuModule *module = new ImuModule(io_context_, imuModules.size());
                    module->SetIpAddress(remote_endpoint_.address());
                    module->SetName(words[1]);
                    imuModules.push_back(module);

                    std::string str = remote_endpoint_.address().to_string();
                    std::cout << "Identification: " << words[1] << " IP: " << str << std::endl;

                    std::string mess = "MasterIP";
                    send_size = mess.size();
                    strcpy(send_buff, mess.c_str());

                    start_send();
                    return;
                }
            }
            //start_send();
            start_receive();
        }
        else
        {
            //LOGW << "Could not read UDP message. Error: " << error.message() << " bytes_recvd: " << bytes_recvd;
            std::cout << "Could not read UDP message. Error: " << error.message() << " bytes_recvd: " << bytes_recvd << "\n";
            //start_receive();
        }
    }
    //-------------------------------------------------------------------------------------------

    void start_send()
    {
        socket_.async_send_to(boost::asio::buffer(send_buff, send_size), remote_endpoint_,
                              boost::bind(&UDPServer::handle_send, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    //-------------------------------------------------------------------------------------------

    void handle_send(const boost::system::error_code &error, size_t bytes_sent)
    {
        if (!error)
        {
            start_receive();
        }
        else
        {
            //LOGW << "Could not send UDP message. Error: " << error.message() << " bytes_sent: " << bytes_sent;
            //start_receive();
        }
    }

    udp::socket socket_;
    udp::endpoint remote_endpoint_;

    char recv_buff[SIZE_BUFF];
    char send_buff[SIZE_BUFF];
    size_t send_size;

    boost::asio::io_context &io_context_;
};

#endif