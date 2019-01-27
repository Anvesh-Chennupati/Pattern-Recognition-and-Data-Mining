#!/opt/python-3.4/linux/bin/python3

import sys
from socket import *

def help():
  s = """
  clientInt.py - clientInt program for integer stream

  USAGE:
    clientInt.py -h
    clientInt.py <host> <port>

  OPTIONS:
    -h   get this help page
    <host> host IP address or host name (currently, only support localhost)
    <port> port number

  EXAMPLE:
    clientInt.py -h
    clientInt.py localhost 32767

  CONTACT:
    Ming-Hwa Wang, Ph.D. 408/805-4175  m1wan@scu.edu
  """
  print(s)
  raise SystemExit(1)

if len(sys.argv) != 3:
  help()

host = sys.argv[1]
port = int(sys.argv[2])
c = socket(AF_INET, SOCK_STREAM)
c.connect((host, port))
done = False
while not done:
  s = c.recv(8)
  if s == b'':
    done = True
  else:
    print(s)
c.close()
