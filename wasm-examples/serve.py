#!/usr/bin/env python3

from http import server 

DIRECTORY = None

class MyHTTPRequestHandler(server.SimpleHTTPRequestHandler):
    def __init__(self, request, client_address, server):
        super().__init__(request, client_address, server, directory=DIRECTORY)


    def end_headers(self):
        self.send_header("Cross-Origin-Opener-Policy", "same-origin")
        self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
        super().end_headers()

if __name__ == '__main__':
    import argparse
    import sys 
    
    parser = argparse.ArgumentParser(prog=sys.argv[0], description="Simple python http server for sherpa ncnn wams validation.")
    parser.add_argument("-b","--bind", type=str,action="store",dest="addr",default="0.0.0.0:8000", help="bind address for serving (default 0.0.0.0:8000)")
    parser.add_argument("-d", "--directory",type=str,action="store", dest="directory",default=None,help="listening directory (default current directory)")

    args = parser.parse_args()
    DIRECTORY = args.directory 
    d_des = DIRECTORY
    if DIRECTORY is None:
        d_des = "."
    
    addr = args.addr
    addr = addr.split(":")
    addr[1] = int(addr[1])
    addr = tuple(addr)
    httpd = server.HTTPServer(addr, MyHTTPRequestHandler)
    print(f"Listening at {addr} with directory {d_des}")
    httpd.serve_forever()
