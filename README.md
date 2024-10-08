# ex_server-ctrl-client_c

Server + Controller + Client example on C.

## Dependencies

- gcc
- make

## Init repository

Run:

```bash
make init
```

## How to use

Doing step by step:

1. In separate terminal run one server by `make run_server`.
2. In separate terminal run one controller by `make run_controller`.
3. In separate terminals run one on more clients by `make run_client`.

## FAQ

### How to find started server

```bash
$ lsof -i | grep "srvc_*"
```

### Test connection to server without client

By `telnet`

```bash
telnet localhost 8888
```