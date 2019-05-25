
# Chat protocol (in development)

This document describes network protocol for "client-server chat" educational exercise.

The chat consist of server and client. Client can issue three types of commands:
- Set username
- Send private message
- Send broadcast message

Server can send two types of messege to client.
- Incoming message
- Repsonse code

---

### Connection
Clients establish a **UDP** (see RFC 793) connection with a server and communicate with text messages.

Clients and servers MAY do anything, including dropping the connection, after recieving a message badly formed according to this RFC.

---
### Nick format

Words (strings without spaces)

---


## Messages send from client to server

**Message delimiting: At the beginning and at the end of each message, a special byte 0x7C ("|") is transmitted. A packet delimiter cannot be found inside a message.**

---
### User messages
Command: "\|USER [nick]\|"
The USER command is used at the beginning of connection to specify  the username.
[nick] - username of user

| Example	| Description |
| --- | --- |
| \|USER VLAD\|  | User sending message "blah blah blah" to user with nickname "Vlad" |

##### Responses:
NICKCOLLISION
NICKFORMAT
OK

****
### Private messages
Command: "\|PMSG [msgtarget] [msg]\|"
- [msgtarget] is the nickname of the recipient of the message
- [msg] is text to be send to another user

| Example	| Description |
| --- | --- |
| \|PMSG VLAD  blah blah blah\| | User sending message "blah blah blah" to user with nickname "Vlad" |

##### Responses:
NOLOGIN
EMPTYMESG
USERNOTEXIST
OK

---
### Broadcast messages
Command: "|BMSG [msg]|"
- [msg]  - text to be send to another user

| Example	| Description |
| --- | --- |
| \|BMSG blah blah blah\| | User sending message "blah blah blah" to all another user |

##### Responses:
NOLOGIN
EMPTYMESG
OK

---
### Quit messages
Command: \|QUIT\|

A client session is terminated with a quit message.
##### Responses:
none

---
## Messages send from server to client

### Incoming messages
Format: "\|MESG [from] [msg]\|"
- [from] - sender nickname
- [msg] - message text

---
### Responses

| Response code           | Response name  | Description  |
|-------------------------|----------------|--------------|
|0                        | OK             |  Success - The action was successfully done  |
|1                        | NICKCOLLISION  |  Returned by a server when user try enter with nickname that is already  taken |
|2                        | NICKFORMAT     |  Incorrect nickname                    |
|3                        | EMPTYMESG      |  User send empty message                  |нужно оно?
|4                        | USERNOTEXIST   | User is send message to not existing or already disconected user  |
|5                        | NOLOGIN        |  Returned by a server when trying to send message before login                  |
|6                        | NOCOMMAND      | No such command