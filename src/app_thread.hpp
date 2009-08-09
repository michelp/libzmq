/*
    Copyright (c) 2007-2009 FastMQ Inc.

    This file is part of 0MQ.

    0MQ is free software; you can redistribute it and/or modify it under
    the terms of the Lesser GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    0MQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    Lesser GNU General Public License for more details.

    You should have received a copy of the Lesser GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __ZMQ_APP_THREAD_HPP_INCLUDED__
#define __ZMQ_APP_THREAD_HPP_INCLUDED__

#include <vector>

#include "stdint.hpp"
#include "object.hpp"
#include "ypollset.hpp"

namespace zmq
{

    class app_thread_t : public object_t
    {
    public:

        app_thread_t (class dispatcher_t *dispatcher_, int thread_slot_);

        ~app_thread_t ();

        //  Returns signaler associated with this application thread.
        i_signaler *get_signaler ();

        //  Nota bene: Following two functions are accessed from different
        //  threads. The caller (dispatcher) is responsible for synchronisation
        //  of accesses.

        //  Returns true is current thread is associated with the app thread.
        bool is_current ();

        //  Tries to associate current thread with the app thread object.
        //  Returns true is successfull, false otherwise.
        bool make_current ();

        //  Processes commands sent to this thread (if any). If 'block' is
        //  set to true, returns only after at least one command was processed.
        void process_commands (bool block_);

        //  Create a socket of a specified type.
        struct i_api *create_socket (int type_);

        //  Unregister the socket from the app_thread (called by socket itself).
        void remove_socket (struct i_api *socket_);

    private:

        //  All the sockets created from this application thread.
        typedef std::vector <struct i_api*> sockets_t;
        sockets_t sockets;

        //  Thread ID associated with this slot.
        //  TODO: Virtualise pid_t!
        //  TODO: Check whether getpid returns unique ID for each thread.
        int tid;

        //  App thread's signaler object.
        ypollset_t pollset;

        //  Timestamp of when commands were processed the last time.
        uint64_t last_processing_time;

        app_thread_t (const app_thread_t&);
        void operator = (const app_thread_t&);
    };

}

#endif