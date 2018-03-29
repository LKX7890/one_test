using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Net;
using System.Threading;
using System.Text;
using System.Runtime.InteropServices;

namespace SocketClient
{
    public struct MsgHeader
    {
        public int m_msg_type;
        public int m_msg_size;

        public MsgHeader(int msg_type, int msg_size)
        {
            m_msg_type = msg_type;
            m_msg_size = msg_size;
        }

        public int GetMsgtype()
        {
            return m_msg_type;
        }

        public int GetMsgSize()
        {
            return m_msg_size;
        }
    }

    class Program
    {
        public struct Heart
        {
            MsgHeader msg_header;

            public int m_h;
            public int m_m;
            public int m_s;

            public Heart(int type, int size, int hour, int min, int sec)
            {
                msg_header.m_msg_type = type;
                msg_header.m_msg_size = size;
                m_h = hour;
                m_m = min;
                m_s = sec;
            }
        }

        public static byte[] StructToBytes(object obj)
        {
            //得到结构体的大小
            int size = Marshal.SizeOf(obj);
            //创建byte数组
            byte[] bytes = new byte[size];
            //分配结构体大小的内存空间
            IntPtr structPtr = Marshal.AllocHGlobal(size);
            //将结构体拷到分配好的内存空间
            Marshal.StructureToPtr(obj, structPtr, false);
            //从内存空间拷到byte数组
            Marshal.Copy(structPtr, bytes, 0, size);
            //释放内存空间
            Marshal.FreeHGlobal(structPtr);
            //返回byte数组
            return bytes;

        }

        public static object BytesToStruct(byte[] bytes, Type type)
        {
            //得到结构的大小
            int size = Marshal.SizeOf(type);
            //Log(size.ToString(), 1);
            //byte数组长度小于结构的大小
            if (size > bytes.Length)
            {
                //返回空
                return null;
            }
            //分配结构大小的内存空间
            IntPtr structPtr = Marshal.AllocHGlobal(size);
            //将byte数组拷到分配好的内存空间
            Marshal.Copy(bytes, 0, structPtr, size);
            //将内存空间转换为目标结构
            object obj = Marshal.PtrToStructure(structPtr, type);
            //释放内存空间
            Marshal.FreeHGlobal(structPtr);
            //返回结构
            return obj;
        }

        private static byte[] result = new byte[1024];
        static void Main(string[] args)
        {
            //设定服务器IP地址  
            IPAddress ip = IPAddress.Parse("127.0.0.1");
            Socket clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                clientSocket.Connect(new IPEndPoint(ip, 5150)); //配置服务器IP与端口  
                Console.WriteLine("连接服务器成功");
            }
            catch
            {
                Console.WriteLine("连接服务器失败，请按回车键退出！");
                return;
            }

            //通过 clientSocket 发送数据  
            while(true)
            {
                Heart heart = new Heart(1, 20, DateTime.Now.Hour, DateTime.Now.Minute, DateTime.Now.Second);
                byte[] data = StructToBytes(heart);

                try
                {
                    Thread.Sleep(5000);    //等待1秒钟  
                    int client_send = clientSocket.Send(data);
                    Console.WriteLine("向服务器发送消息：{0}", client_send);
                }
                catch
                {
                    clientSocket.Shutdown(SocketShutdown.Both);
                    clientSocket.Close();
                    break;
                }

                //通过clientSocket接收数据  
                int receiveLength = clientSocket.Receive(result);
                MsgHeader header = new MsgHeader(0, 0);
                header = (MsgHeader)BytesToStruct(result, header.GetType());
                int type = header.GetMsgtype();
                int size = header.GetMsgSize();
                Console.WriteLine(type);
                Heart recv_heart = new Heart(0, 0, 0, 0, 0);
                recv_heart = (Heart)BytesToStruct(result, recv_heart.GetType());
                Console.WriteLine("{0}:{1}:{2}", recv_heart.m_h, recv_heart.m_m, recv_heart.m_s);
            }
            Console.WriteLine("发送完毕，按回车键退出");
            Console.ReadLine();
        }
    }
}