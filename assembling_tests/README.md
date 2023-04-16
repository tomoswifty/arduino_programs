# PCBボードテストプログラムの使い方
<img src="/Users/tomo/arduino_programs/photo/pcb/02_pcb_top.JPG" width="500">

## Arduino準備
## 1. プログラムを書き込む
Arduinoを接続して，ボードを選択．
<img src="../photo/pcb/03_arduino_upload.png" width="800">

## 2. シリアルモニタ(Serial Monitor)の起動

<img src="../photo/pcb/04_arduino_openSerialMonitor.png" width="800">

## 3. コマンドを入力する
シリアルモニタのコマンド窓にコマンドを入力して，`Enter`で送信．
<img src="../photo/pcb/05_arduino_sendCommand.png" width="800">

## コマンド

- `o` : 全てのリレーをLOWにする
- `h` : 全てのリレーをHIGHにする
- `l` : 左側のリレー（CR1〜6）をHIGHにする
- `r` : 右側のリレー（CR7〜12）をHIGHにする
以下，各リレーをHIGHにする
- `1` : CR1
- `2` : CR2
- `3` : CR3
- `4` : CR4
- `5` : CR5
- `6` : CR6
- `7` : CR7
- `8` : CR8
- `8` : CR8
- `9` : CR9
- `q` : CR10
- `w` : CR11
- `e` : CR12

## リレー対応表
<img src="../photo/pcb/022_pcb_top_marked.png" width="500">