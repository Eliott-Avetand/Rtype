
# R-Type

<p align="center">
    <img src="./Config/Assets/RTypePresentation.gif_" alt="R-Type présentation" />
</p>

The purpose of the project is to implement a multithreaded server
and a graphical client for the famous game R-Type, using an engine of our own design.

This project is also here to introduce us to networked video game development,
to explore in-depth advanced development topics as well as to learn good software engineering practices.
## Technical Part

### Stack

Language: C++

VSC Workflow: Git Workflow

Software Compilation: CMake


### Dependencies

### Requirements

### Supported Platforms

Windows / Linux

<img src="./Config/Assets/WindowsLogo.png" alt="Linux" width="80" />
<img src="./Config/Assets/LinuxLogo.jpg" alt="Windows" width="80" />

## Build and Deployment

To build the project you need to run, at the root of the project:

Windows (as administrator)
```bash
  .\install.bat
```

Linux 
```bash  
  sudo ./install.sh
```

To run the project you can launch

```bash
  ./r-type_server
```

to launch the server and host the games, and

```bash
  ./r-type_client
```

to launch a instance of the game.

WARNING: It may be possible that the network isn't working and you can't receive informations due to the firewall.
To fix it, you need to disable your firewall like so:

On linux
```bash
  sudo systemctl stop firewalld
```

On Windows
You need to disabled it by hand in the settings

2ND WARNING: It may also be possible that you face a problem with the compiler version. We set it to 12 in the file '.conan/profile' but if your compiler's version is older it won't compile.
You just have to change the version of the compiler in the file '.conan/profile' by your (gcc -v for your version)

## License

All the code is the property of ©Loustik Studio

<img src="./Config/Assets/LoustikStudioLogo.png" alt="Loustik Studio" width="200" />


## Authors

If you have any problems building and deploying the game, you can contact one of us !

- [Eliott](https://www.github.com/Eliott-Avetand)
- [Anthony](https://www.github.com/Abecarne)
- [Simon](https://www.github.com/Saymon24)
- [Grégoire](https://www.github.com/gregoirebiendine)
- [Alexis](https://www.github.com/AlexisDevloo)
- [Eléonore](https://www.github.com/Eleonoresycz)

