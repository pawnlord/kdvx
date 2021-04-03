# kdvxp
The kdvx protocol, an all inclusive decentralized chat protocol for anything  

## protocol list

### client
The following commands need to be implemented for this protocol for the client:  
```
:login <username> <hashed_passwd>: -- login functionallity, username and password must match servers
:pong:  -- tell server you're alive, must be sent in response to :ping <s>: in 's' seconds 
:msg <channel>: <message> -- Generic text message
:send <filename>:  -- start sending file bytes
:dnes <filename>:  -- stop sending file bytes
:take <filename> <channel>: -- start taking filename from <channel>
:open <channel>:  -- start getting data from <channel>
:read <channel>:  -- update content from an already open <channel>
:close <channel>:  --stop getting data from <channel> 
:open-voice <voice-channel>:  -- start sending voice data over separate port
:close-voice <voice-channel>:  -- end sending voice data
:err <err-code>:  -- signal an error has occured
```

### server
The following commands need to be implemented for the server:
```
:welcome:  -- signal succesful lof in
:ping <s>:  -- await 'pong' command for <s> seconds
:send <filename>:  -- response to 'take' command
:dnes <filename>:  -- end response to 'take' command
:contents <channel>:  -- start sending new contents of <channel>
:stnetnoc <channel>:  -- signal end of new contents of <channel>
:err <err-code>: <message>: -- signal an error has occured
```

### err codes
'''
0: EndConnection	Signals that an error ends connection, 
			can be used in-front of any other error 

1: BadLogin		Signals that the login failed

2: PermDenied		User does not have permission to 
			perform requested command

3: Failed		Indicates requested action failed,
			stop awaiting response related to it
...
'''

## voice
voice will be done over a separate port in order to ensure you can send text while in a voice channel.  
This will mostly be controlled by the main connection, with only content being sent through this port.  
