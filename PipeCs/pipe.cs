using System;
using System.IO;
using System.IO.Pipes;

namespace PipeCs
{
    class PipeCs
    {
        public static int bufSize = 1024;
        public static string pipeName = "CanastaPipe";
    }

    public static class PipeClient
    {
        private static NamedPipeClientStream client;
        private static StreamReader reader;
        private static StreamWriter writer;

        public static void ConnectPipe()
        {
            client = new NamedPipeClientStream(
                ".",
                PipeCs.pipeName,
                PipeDirection.InOut,
                PipeOptions.None);
            if (client.IsConnected != true) { client.Connect(20000); }
            reader = new StreamReader(client);
            writer = new StreamWriter(client);
        }
        public static string ReadPipe()
        {
            if (!IsReadable())
                ConnectPipe();

            return reader.ReadLine();
        }
        public static bool IsReadable()
        {
            return client.IsConnected && client.CanRead;
        }
        public static void WritePipe(string input)
        {
            if (String.IsNullOrEmpty(input)) return;
            if (!IsWritable())
                ConnectPipe();
            writer.WriteLine(input);
            writer.Flush();
        }
        public static bool IsWritable()
        {
            return client.IsConnected && client.CanWrite;
        }
    }

    public static class PipeServer
    {
        private static NamedPipeServerStream server;
        private static StreamReader reader;
        private static StreamWriter writer;

        public static string m_strRead = "";
        public static string m_strWrite = "";
        public static byte[] m_strReadBytes;
        public static byte[] m_strWriteBytes;

        public static void CreatePipe()
        {
            PipeSecurity pipeSec = new PipeSecurity();
            pipeSec.SetAccessRule(new PipeAccessRule("Everyone", PipeAccessRights.ReadWrite, System.Security.AccessControl.AccessControlType.Allow));
            server = new NamedPipeServerStream(
                PipeCs.pipeName,
                PipeDirection.InOut,
                NamedPipeServerStream.MaxAllowedServerInstances,
                PipeTransmissionMode.Message,
                PipeOptions.None,
                PipeCs.bufSize,
                PipeCs.bufSize,
                pipeSec,
                HandleInheritability.None);
            reader = new StreamReader(server);
            writer = new StreamWriter(server);

            server.WaitForConnection();
            Console.WriteLine("connect!");
        }
        public static void ClosePipe()
        {
            server.WaitForPipeDrain();
            Console.WriteLine("disconnect!");
            server.Disconnect();
            server.Close();
        }
        public static string ReadPipe()
        {
            return reader.ReadLine();
        }
        public static bool IsReadable()
        {
            return server.IsConnected && server.CanRead;
        }
        public static void WritePipe(string input)
        {
            if (String.IsNullOrEmpty(input)) return;
            writer.WriteLine(input);
            writer.Flush();
        }
        public static bool IsWritable()
        {
            return server.IsConnected && server.CanWrite;
        }
    }
}
