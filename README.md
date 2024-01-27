# Arm Shell

Arm shell is my attempt at creating an implementation of a shell written in c.

## Installation

Clone the repository and run make. This will generate an executable called armsh in the bin directory. Run it and experiment.

```bash
1. make
2. ./bin/armsh
```

## Usage

Once you have run the shell, you can hit help and check out the arm shell builtins currently supported.

```bash
🇦🇲  help
Armenian shell
Below are the builtins
 echo
 cd
 pwd
 help
 exit
🇦🇲
```

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

## Future Improvements

- Create an ~/.armsh_history file in the user's home directory and at exit append the commands from the current session to that file
- add more builtins ( alias, history, type )
