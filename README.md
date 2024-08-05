# rainbow-text
A little program I made in my free time

<img src="https://github.com/ARTEZON/rainbow-text/blob/main/images/preview.gif?raw=true" width="635" alt="Preview GIF">

### Update 2024!
- Linux support
- Multilingual Unicode support
- Auto adjustment for terminal size
- Code improvements
- To exit the program press <kbd>CTRL</kbd> + <kbd>C</kbd>

<img src="https://github.com/ARTEZON/rainbow-text/blob/main/images/Screenshot_20240805_211959.png?raw=true" width="635" alt="2024 New Version on Linux Terminal">

### Compile
```
g++ ./rainbow.cpp -o rainbow -O2
```

### Dependencies
- [utfcpp](https://github.com/nemtrif/utfcpp) library licensed under BSL-1.0

### Known issues:
- Some asian characters are misaligned because they are wider than other characters
- Command line arguments haven't been added, you need to edit the code to change the text, animation speed and direction
- The number of lines is still hardcoded
