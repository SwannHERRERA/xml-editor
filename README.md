# xml-editor

Un editeur de XML en C avec GTK qui prend en compte les DTD

## Utilisation

`make build` Build exe

`make start` run exe. This one needs argument inputFile

exemple:

```ZSH
make start inputFile=./dtd-exemple.xml
```

`make dev` run build and start

```ZSH
make dev inputFile=./dtd-exemple.xml
```

`make` is a shortcut for `make dev`
