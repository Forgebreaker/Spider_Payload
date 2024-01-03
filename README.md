# Installation
- git clone https://github.com/hungthinhtran/Spider_Payload
- cd Spider_Payload
- python3 Spider_Payload.py

# Usage
![image](https://user-images.githubusercontent.com/112708857/229701475-22b66b24-b246-4f0e-ac4e-306d65e6cffc.png)

# Mindset
This tool generates a listener and a payload in Python (using HTTP protocol). The listener waits for connections from the payload, receives commands, encrypts them, sends them to the payload, and then decrypts the output and prints it. The payload waits for commands from the listener, decrypts them, executes them, encrypts the output, and sends it back to the listener.

The tool uses  the cryptocode module to encrypt and decrypt the commands and output.

To generate the payload and listener, the tool prompts the user to enter the IP address and port number for the listener then generates a random encryption key and uses it to encrypt and decrypt the commands and output.

The listener is written to a file named Listener_{random}.py, and the payload is written to a file named Payload_{random}.py.

Optionally, the tool can automatically start the listener after generating the payload and listener.





