# CloudOLight

CloudOLight is the control software for a dress filled with amorphous clouds of light. The lighting pattern is controlled wirelessly via OSC messages. Two OSC endpoints control the colours and intensity of LEDs at the top and bottom of an LED array. The dress automatically interpolates everything else.


## Compiling:

1. Install [PlatformIO](https://docs.platformio.org/en/latest/installation.html).
2. Download or git clone CloudOLight source code.
3. Open a terminal and navigate to the source code location.
4. Edit the configuration at the top of src/main.cpp
4. Build the project: `$platformio run --target upload`


## Hardware:

[Techniques for diffusing light within a dress.](https://reprage.com/post/how-to-make-a-dress-filled-with-clouds-of-light)


## License

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.