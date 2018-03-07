using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace PipeCs
{
    class Program
    {
        static void Main(string[] args)
        {
            bool choose = false;
            if (choose)
            {
                string read = "0";
                while (true)
                {
                    PipeClient.ConnectPipe();

                    if (read[0] == '9'+1)
                    {
                        PipeClient.WritePipe("quit");
                        break;
                    }
                    else
                        read = (char)(read[0] + 1) + read.Substring(1);
                    PipeClient.WritePipe(read);
                    Console.WriteLine("<<" + read);

                    read = PipeClient.ReadPipe();
                    Console.WriteLine(">>" + read);
                }
            }
            else
            {
                string read = "";
                while (true)
                {
                    PipeServer.CreatePipe();

                    read = PipeServer.ReadPipe();
                    Console.WriteLine(">>" + read);
                    if (read != "quit")
                    {
                        read = (char)(read[0] + 1) + read.Substring(1);
                        Thread.Sleep(200);
                        PipeServer.WritePipe(read);
                        Console.WriteLine("<<" + read);
                    }
                    PipeServer.ClosePipe();
                }
            }
        }
    }
}
