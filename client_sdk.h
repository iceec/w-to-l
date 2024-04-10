#include"Client_Socket.h"
#include "data_stream.h"
using namespace Yu::Socket;
using namespace Yu::serialize;



namespace Yu::rpc
{

    class Client_Sdk:public Client_Socket{
        struct fuc_info
        {
            uint16_t cmd;
            uint16_t len;
            fuc_info(){}
            fuc_info(uint16_t c,uint16_t l):cmd(c),len(l){}
        };

        static const int send_buff_size=1024;
        static const int recv_buff_size=1024;
        
        public:
        Client_Sdk()=delete;
        ~Client_Sdk()=default;
        Client_Sdk(const string ip,int port):Client_Socket(ip,port){/*to do  need init log*/  }

        template<class R,class ...Arg>
        auto Call(const string & func_name,Arg && ...args)->R;
    };

    template<class R,class ...Arg>
        auto Client_Sdk::Call(const string & func_name, Arg&& ...args)->R
        {
                data_stream in;
                in<<func_name;
                in.write_args(std::forward<Arg>(args)...);
                char send_buf[send_buff_size]{0};
                fuc_info mess(0,in.size());
                std::memcpy(send_buf,static_cast<char *>(mess),sizeof(mess));
                std::memcpy(send_buf+sizeof(mess),static_cast<char *>(in.data()),in.size());
                int len=in.size()+sizeof(mess);
                send(send_buf,len);
            
                // to do  需要先接受信息 这样收到相同的长度才行
                char recv_buf[recv_buff_size]{0};
                int recv_len=read(recv_buf,recv_buff_size);
                data_stream out;
                out.write(recv_buf,recv_len);
                R v;
                out>>v;
                return v;



                

        }

}
