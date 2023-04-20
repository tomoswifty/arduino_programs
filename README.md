[English](README.en.md) | [日本語](README.md)

# MSD700のArduinoコード管理

<div align="center">
<img src="./photo/100x.png" width="300" />
</div>

1000～の数字で始まるファイルは完成し，デモに出荷されたマシンを表しています．
それぞれの数字は各号機を示しています．
特に1000については，テンプレートとして書き直されたコードです．

tunnelフォルダ内のファイルは，西松建設との共同開発のトンネル検査に使用されています．
1004号機がこのトンネルモデルとなっているため，1004号機はナンバリングには含まれていません．

testフォルダ内には，pcbをテストするためのコードがあります．

このリポジトリには見えていませんが，oldディレクトリというフォルダが存在します．
これは，中山本社のサーバローカルのみに保存し，git管理からは除外しています．


## インストール
任意のディレクトリで`Download ZIP`でダウンロードするか，`git clone`してダウンロードしてください．
`.ino`はArduino IDEで開いてください．Arduino IDEのインストールについては，ここでは説明を省きます．
```
git clone https://github.com/nakayama-software/arduino_programs.git
```

## 使い方

### プログラムの書き込み
Arduino IDEで`Arduino Mega or Mega2560`を選び，ポートを指定して書き込んでください．
