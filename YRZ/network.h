//
//  network.h
//  Beast Chess
//
//  Created by Runzhe Yang on 11/15/14.
//  Copyright (c) 2014 Runzhe Yang. All rights reserved.
//

#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#pragma once
#include <iostream>
#include <string>
#include <boost/asio.hpp>

namespace  network{
    
    using boost::asio::ip::tcp;
    
    class IO {
    public:
        IO(const std::string &host, const std::string &service) : io_service_(), socket_(io_service_)
        {
            tcp::resolver resolver(io_service_);
            tcp::resolver::query query(host, service);
            //tcp::resolver::query query(tcp::v4(), host, service);
            tcp::resolver::iterator iterator = resolver.resolve(query);
            boost::asio::connect(socket_, iterator);
        }
        
        void receive(std::string &message)
        {
            boost::asio::read_until(socket_, buffer, '\n');
            std::istream is(&buffer);
            std::getline(is, message);
        }
        
        void send(const std::string &message)
        {
            boost::asio::write(socket_, boost::asio::buffer(message.data(), message.size()));
        }
        
    private:
        boost::asio::io_service io_service_;
        tcp::socket socket_;
        boost::asio::streambuf buffer;
    };
    
}

#endif // NETWORK_H_INCLUDED
