# DLL Injection

by gon (2019/2/14)

## Compile

```
> cl message.c /LD
> cl agent.c message.lib
> cl alert.c /LD
> cl dllinjection.c
```

## for Security Test

1. Get imported DLL

```
> dumpbin agent.exe /IMPORTS
```

2. Trace Use of normalMsg() and Sleep()
 - Strings or ProcessExplorer, ...
 
3. Make DLL for injecting ...

