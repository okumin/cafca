#!/bin/bash

/usr/local/bin/zkCli create '/brokers' ''
/usr/local/bin/zkCli create '/brokers/ids' ''
/usr/local/bin/zkCli create '/brokers/ids/1' '{"version": 1, "host": "127.0.0.1", "port": 9092}'

/usr/local/bin/zkCli create '/brokers/topics/test' '{"version": 1, "partitions": {"1": [1]}}'
/usr/local/bin/zkCli create '/brokers/topics/test/partitions' ''
/usr/local/bin/zkCli create '/brokers/topics/test/partitions/1' ''
/usr/local/bin/zkCli create '/brokers/topics/test/partitions/1/state' '{"version": 1, "isr": [1], "leader": 1, "controller_epoch": 1, "leader_epoch": 0}'


