# memo

# ビルド手順

## ファーム作成

1. [README.md](README.md) に従って `microbit-mruby` のファームウェア `MICROBIT.hex` を作成する

## HEXファイルへのmrubyアプリ書き込み

1. mrubyコードを作成する

作成したコードは `<mruby-code>.rb` とする

2. mrubyコードをコンパイルする

```sh
mrbc <mruby-code>.rb
```

mrubyアプリバイナリ `<mruby-code>.mrb` が生成される

3. mrubyアプリバイナリをファームウェアにマージする

```sh
./embed-mrb.rb MICROBIT.hex <mruby-code>.mrb 
```

アプリバイナリがマージされたファームウェア `MICROBIT-mruby.hex` が生成される

4. micro:bitにファームウェアを書き込む

- `micro:bit v2` を開発環境にUSB接続し、ドライブとして認識させる
- `MICROBIT-mruby.hex` を `micro:bit` ドライブのルートディレクトにコピーする

書き込み完了後、micro:bitがリセットされ、mrubyアプリケーションが開始される

