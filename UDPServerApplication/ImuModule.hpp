#ifndef __IMUMODULE_HPP
#define __IMUMODULE_HPP

#include <cstdlib>
#include <iostream>
#include <fstream>

#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"

namespace pt = boost::posix_time;
//#include "UDPServer.hpp"

class ImuModule
{
public:
    ImuModule(boost::asio::io_context &io_context, int id) : of(), Timer1(io_context, boost::posix_time::millisec(1000))
    {
        id_ = id;
        Timer1.async_wait(boost::bind(&ImuModule::Timer1_Hendle, this, boost::asio::placeholders::error, &Timer1));
    }

    struct DataIMU
    {
        float AccelX;
        float AccelY;
        float AccelZ;

        float GyroX;
        float GyroY;
        float GyroZ;

        float MagX;
        float MagY;
        float MagZ;

        //float Temperature;

        float q0;
        float q1;
        float q2;
        float q3;
    };
    //---------------------------------------------------------------
    ~ImuModule()
    {
    }
    //---------------------------------------------------------------
    void StartRec(std::string Patch)
    {

        std::string name = "logs/" + Patch + "/" + std::to_string(id_) + "-" + name_ + ".txt";
        of.open(name);

        isStart = true;
    }
    //---------------------------------------------------------------
    void StopRec()
    {
        isStart = false;
        of.close();
    }
    //---------------------------------------------------------------
    void Timer1_Hendle(const boost::system::error_code & /*e*/, boost::asio::deadline_timer *t)
    {
        t->expires_at(t->expires_at() + boost::posix_time::millisec(1000));
        t->async_wait(boost::bind(&ImuModule::Timer1_Hendle, this, boost::asio::placeholders::error, t));

        FPS = NumberReceivPack;
        NumberReceivPack = 0;
    }
    //---------------------------------------------------------------
    float GetRoll(DataIMU dataIMU)
    {
        return -atan2(2.0f * (dataIMU.q0 * dataIMU.q2 - dataIMU.q1 * dataIMU.q3), 1.0f - 2.0f * (dataIMU.q2 * dataIMU.q2 + dataIMU.q1 * dataIMU.q1)) * 57.2;
    }
    //---------------------------------------------------------------
    float GetPitch(DataIMU dataIMU)
    {
        // calculate pitch angles
        return atan2(2 * dataIMU.q2 * dataIMU.q3 - 2 * dataIMU.q0 * dataIMU.q1, 2 * dataIMU.q0 * dataIMU.q0 + 2 * dataIMU.q3 * dataIMU.q3 - 1) * 57.2;
    }
    //---------------------------------------------------------------
    float GetYaw(DataIMU dataIMU)
    {
        // calculate yaw angles
        return atan2(2 * dataIMU.q1 * dataIMU.q2 - 2 * dataIMU.q0 * dataIMU.q3, 2 * dataIMU.q0 * dataIMU.q0 + 2 * dataIMU.q1 * dataIMU.q1 - 1) * 57.2;
    }
    //---------------------------------------------------------------
    void Update(DataIMU *dataIMU)
    {
        ConnectionStatus = true;
        NumberReceivPack++;

        pt::ptime current_date_microseconds = pt::microsec_clock::local_time();
        //long milliseconds = current_date_microseconds.time_of_day().total_milliseconds();
        //pt::time_duration current_time_milliseconds = pt::milliseconds(milliseconds);
        //pt::ptime current_date_milliseconds(current_date_microseconds.date(),
        //current_time_milliseconds);
        //std::cout << current_date_microseconds << std::endl;
        //<< " Milliseconds: " << current_date_milliseconds << std::endl;

        if (isStart)
        {
            of << current_date_microseconds.time_of_day().total_microseconds() << " ";
            for (int i = 0; i < 3; i++)
            {
                of << dataIMU[i].AccelX << " " << dataIMU[i].AccelY << " " << dataIMU[i].AccelZ << " "
                   << dataIMU[i].GyroX << " " << dataIMU[i].GyroY << " " << dataIMU[i].GyroZ << " "
                   << dataIMU[i].MagX << " " << dataIMU[i].MagY << " " << dataIMU[i].MagZ << " "
                   << dataIMU[i].q0 << " " << dataIMU[i].q1 << " " << dataIMU[i].q2 << " " << dataIMU[i].q3 << " "
                   << GetYaw(dataIMU[i]) << " " << GetPitch(dataIMU[i]) << " " << GetRoll(dataIMU[i]) << " "; // short macro
            }
            of << "\n";
        }
    }

    //---------------------------------------------------------------
    boost::asio::ip::address GetIpAddress()
    {
        return ip;
    }
    //---------------------------------------------------------------
    void SetIpAddress(boost::asio::ip::address ip_)
    {
        ip = ip_;
    }
    //---------------------------------------------------------------
    void SetName(std::string name)
    {
        name_ = name;
    }
    //----------------------------------------------------------------
    std::string GetName()
    {
        return name_;
    }
    //-----------------------------------------------------------------
    int GetFPS()
    {
        return FPS;
    }

private:
    boost::asio::ip::address ip;
    std::string name_;

    boost::asio::deadline_timer Timer1;
    //boost::asio::deadline_timer Timer_TimeOut;

    int FPS;
    unsigned int NumberReceivPack;

    bool ConnectionStatus;

    std::ofstream of;

    boost::asio::io_context io_context2;

    bool isStart = false;
    int id_ = 0;
};

#endif