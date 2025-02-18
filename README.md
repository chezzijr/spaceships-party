# Spaceships party
An inspiration from astro party game

### Requirements
- SDL2: 2.30.6
- SDL2_image: 2.8.4
- SDL2_ttf: 2.24.0
- gcc: 14.2.1

### How to run

###### Linux
- `make all` to build, output at `dist/`
- `make run` to run the built executable

###### Windows
- Install [MSYS2](https://www.msys2.org/)
- Enter MSYS2 MINGW64 terminal
- Install packages
```
pacman -S git mingw-w64-x86_64-toolchain mingw64/mingw-w64-x86_64-SDL2 mingw64/mingw-w64-x86_64-SDL2_mixer mingw64/mingw-w64-x86_64-SDL2_image mingw64/mingw-w64-x86_64-SDL2_ttf mingw64/mingw-w64-x86_64-SDL2_net mingw64/mingw-w64-x86_64-cmake make
```
- Use the terminal to navigate to the project directory
- `make all` to build, output at `dist/`
- `make run` to run the built executable

### Development
- If you are using VSCode, you can add this to `settings.json` to activate the terminal by default
```json
{
    ...
    "terminal.integrated.profiles.windows": {
        "PowerShell": {
            "source": "PowerShell",
            "icon": "terminal-powershell"
        },
        "Command Prompt": {
            "path": [
                "${env:windir}\\Sysnative\\cmd.exe",
                "${env:windir}\\System32\\cmd.exe"
            ],
            "args": [],
            "icon": "terminal-cmd"
        },
        "Git Bash": {
            "source": "Git Bash"
        },
        "MSYS2": {
            "path": "C:\\msys64\\usr\\bin\\bash.exe",
            "args": [
                "--login",
                "-i"
            ],
            "env": {
                "MSYSTEM": "MINGW64",
                "CHERE_INVOKING": "1"
            }
        },
    },
    "terminal.integrated.defaultProfile.windows": "MSYS2", 
    ...
}
```