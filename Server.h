/*  filename: Server.h    2009/02/09  */
/*************************************************************************
    LuLu Messenger: A LAN Instant Messenger For Chatting and File Exchanging.
    Copyright (C) 2008,2009  Wu Weisheng <wwssir@gmail.com>

    This file is part of LuLu Messenger.

    LuLu Messenger is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LuLu Messenger is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*************************************************************************/
/*************************************************************************
    LuLu 信使: 局域网络即时对话与文件交换通讯工具。
    著作权所有 (C) 2008,2009  武维生 <wwssir@gmail.com>

    此文件是 LuLu 信使源文件的一部分。

    LuLu 信使为自由软件；您可依据自由软件基金会所发表的GNU通用公共授权条款，
    对本程序再次发布和/或修改；无论您依据的是本授权的第三版，或（您可选的）
    任一日后发行的版本。

    LuLu 信使是基于使用目的而加以发布，然而不负任何担保责任；亦无对适售性或
    特定目的适用性所为的默示性担保。详情请参照GNU通用公共授权。

    您应已收到附随于本程序的GNU通用公共授权的副本；如果没有，请参照
    <http://www.gnu.org/licenses/>.
*************************************************************************/

// 说明：    
// 修改：    

#ifndef __Server_h__
#define __Server_h__

#include "defs.h"
#include <ace/OS.h>
#include <ace/SOCK_Dgram_Mcast.h>
#include <ace/Event_Handler.h>
#include <ace/Reactor.h>
#include "DataAccess.h"

extern QQueue<IMProto::Message_ptr> g_msgQueueBuf;

class Server_Events : public QObject, public ACE_Event_Handler
{
        Q_OBJECT

public:
        Server_Events (DataAccess& daccess);
        ~Server_Events (void);

        virtual int handle_input (ACE_HANDLE fd);
        virtual ACE_HANDLE get_handle (void) const;

private:
        char buf_[PayloadSize];
        char hostname_[MAXHOSTNAMELEN];

        ACE_Reactor *reactor_;
        ACE_SOCK_Dgram_Mcast mcast_dgram_;
        ACE_INET_Addr remote_addr_;
        ACE_Vector<ACE_INET_Addr> mcast_addr_;
        
        DataAccess& m_DataAccess;
};

//////////////////////////////////////////////////////////////////////////
class FormMain;

class RecvThread : public QThread
{
        Q_OBJECT

public:
        RecvThread(DataAccess& daccess, FormMain* fm, QObject* parent = 0)
                : QThread(parent)
                , m_formMain(fm)
                , m_DataAccess(daccess)
        {
        }

protected:
        void run();
private:
        FormMain* m_formMain;
        DataAccess& m_DataAccess;
};

class Consumer : public QThread
{
        Q_OBJECT

signals:
        void sigRecvReady();

public:
        Consumer(QObject* parent = 0)
                : QThread(parent)
        {

        }

protected:
        void run();
private:
};


#endif // __Server_h__
