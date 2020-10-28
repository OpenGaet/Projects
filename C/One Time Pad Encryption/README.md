## Description
  This program encrypts and decrypts a text file using the one time pad method of encryption. The plaintext files are example files that can be used to demonstrate the encryption.

## To Compile
```BASH
$ compileall
```

## Program Options

  ### To Generate the Key
  ```BASH
    $ keygen [number of characters desired in key]
  ```

  ### To Encrypt a File
  ```BASH
    $ otp_enc_d [port number] &
    $ otp_enc [plaintext file] [key file] [port number] > [encrypted file]
    $ kill -TERM [otp_enc_d pid]
    $ cat [encrypted file]
  ```

  ### To Decrypt a File
  ```BASH
    $ otp_dec_d [port number] &
    $ otp_dec [encrypted file] [key file] [port number] > [decrypted file]
    $ kill -TERM [otp_dec_d pid]
    $ cat [decrypted file]
  ```
