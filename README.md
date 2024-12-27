A simple demonstration of using ImGUI as a web assembly application <br>
<br>
<br>
#########################REQUIREMENTS##################################<br>
<br>
OpenGL<br>
```bash
sudo dnf install mesa-libGL mesa-libGL-devel <br>
```
GLFW<br>
``` bash
sudo dnf install glfw glfw-devel<br>
```
Python3 <br>
```
sudo dnf install python3(insert version here)<br>
```
emsdk (emscripten)<br>
```bash
git clone https://github.com/emscripten-core/emsdk.git <br>
cd emsdk<br>
./emsdk install latest<br>
./emsdk activate latest<br>
source ./emsdk_env.sh<br>
```
<br>
<br>
#########################HOW TO BUILD##################################<br>
make sure you have the emsdk_env sourced before doing this <br>
if you have cmake:<br>
```
mkdir build<br>
cd build<br>
cmake ..<br>
```
<br>
if you dont have cmake: <br>
```bash
make<br>
```
<br>
<br>
#########################HOW TO RUN##################################<br>
<br>
start a python simple server at the root of your repo<br>
```bash
python3 -m http.server<br>
```
<br>
open your preferred browser and navigate to <br>
```browser
localhost:8000<br>
```
<br>
click on "imgui.html"<br>
<br>
wah lah<br>
<br>
<br>
refer to "Notes.txt" for more details
