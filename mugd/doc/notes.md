
+  Linux _read(fd, ...)_ function will _end_ when meets \0 characters.
   Hence, when using _write(fd, ...)_ to send data through sockets;
   Pass just the size which the _strlen()_ returns, don't add the extra
   \0 character.

   - Yiwei Chiao, 05-04-2017
