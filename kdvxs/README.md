# kdvxs
This is the main server component for the kdvx protocol  

## layout map
```
kdvxs -- Handles config generation and general starting of the server
|
server -- Handles incoming requests, "backend" part of the server
|--
|  |
|  voice_server -- Handles requirements for the voice portion of the server
|
text_server -- Handles the text portion of the server
|
filemanager -- handles files
```
