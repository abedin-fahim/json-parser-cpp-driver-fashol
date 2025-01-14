To run the project, you need to have the following dependencies:

- OpenSSL
- nlohmann_json

You can install the dependencies using the following commands (Only first time):

```
sudo apt-get install libssl-dev
sudo apt-get install libnlohmann-json-dev
```

To build the project, you can use the following command:

```
g++ main.cpp -o main
```

To run the project, you can use the following command: (Make sure you are in the build directory)

```
<!-- Writing files -->
./main
<!-- Reading files -->
./main --read filename.json
./main --read filename.json --key your_encryption_key
or
php script.php
or
python3 script.py
```

ow you can use the command like this:
To read a normal file: ./main --read filename.json
To read an encrypted file: ./main --read filename.json --key your_encryption_key

To build and run the project using the provided script, you can use the following commands:

```
mkdrir build
cd build
cmake ..
make
dpkg-deb --build debian fahsol-json-parser-1.0.0.deb
sudo dpkg -i json-parser-1.0.0.deb
```

Recreate the Original Source Tarball
Generate a new .orig.tar.gz file:

bash
tar --exclude-vcs -czf ../fashol-json-parser_1.0.0.orig.tar.gz .
