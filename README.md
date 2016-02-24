# dining-philosophers
Implementation of dining philosophers problem using TCP server-client architecture.

Made as a school assignment for subject OSY (Operating systems).

# Build
`make`

# Usage of server
`./server_prog -s 5 -p 5555`

## Parameters
`-s` number of seats at the table (must be at least 1)

`-t` thread mode (without this parameter forks are used instead)

`-p` port number

`-d` debug mode

`-h` help

# Usage of klient
`./klient_prog -s localhost -p 5555`

## Parameters
`-s` server address

`-p` port number

`-c` minimum wait time in seconds before the client disconnects (default is 2)

`-d` debug mode

`-h` help
