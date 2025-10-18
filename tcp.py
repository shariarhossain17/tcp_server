import socket
import threading


class TcpServer:
    def __init__(self,host="localhost",port=6379):
        self.server_socket=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
        self.server_socket.bind((host,port))
        self.server_socket.listen(5)
        print(f"Server listening on {host}:{port}")

    def handle_client(self,conn,addr):
        print(f"Connect By {addr}")
        try:
            while True:
                data=conn.recv(1024).decode().strip()
                print(f"Raw data received: {repr(data)}")
                if not data:
                    break
                if data.upper()=="P":
                    conn.send(b"+PONG\r\n")
                else:
                    conn.send(b"-ERR unknown command\r\n")
        except Exception as e:
            conn.send(f"-ERR {str(e)}\r\n".encode())
        finally:
            conn.close()
            print(f"Disconnected: {addr}")



    def run(self):
        try:
            while True:
                conn,addr=self.server_socket.accept()
                #creating new thread for each client 
                threading.Thread(target=self.handle_client, args=(conn, addr)).start()
        except KeyboardInterrupt:
            print("server closed by user")
        finally:
            self.server_socket.close()



if __name__== "__main__" :
    server=TcpServer()
    server.run()
    



