# micro:bit mruby

**micro:bit mruby**（以下microbit-mruby）は [BBC micro:bit](https://microbit.org/) で動作する [mruby](https://mruby.org/) 実行環境です。  
mrubyは、人気の開発言語 [Ruby](https://www.ruby-lang.org/) を軽量化したプログラミング言語で、組込みシステムや様々なソフトウェアに組み込むことができる軽量なプログラミング言語です。

microbit-mruby は mruby 3.0 を搭載しており、micro:bit v2 マイコン上でmrubyアプリケーションを実行できる環境を提供します。

microbit-mruby は、ランカスター大学が公開している [microbit-v2-samples](https://github.com/lancaster-university/microbit-v2-samples) をベースにして作成しました。

# microbit-mruby必要なもの

- micro:bit v2  
  ※ v1では動作しません
- Windows PC または Mac

# microbit-mruby環境構築

## ビルド環境のセットアップ

microbit-mrubyでは、以下のセットアップが必要です。

- C言語開発環境 (gccなど)
- Ruby (2.7以降を推奨)
- git
- Docker

## ソースコードの取得とビルド

以下のコマンドを実行して、microbit-mrubyのソースコードを取得します。

```bash
cd $WORKING_DIR
git clone https://github.com/mruby-Forum/microbit-mruby.git
cd microbit-mruby
git clone https://github.com/mruby/mruby.git -b 3.0.0 --depth 1
```

- **$WORKING_DIR**には任意のディレクトリを指定してください。

## Docker imageの作成（初回のみ）

microbit-mrubyディレクトリ内で以下のコマンドを実行することで、micro:bitのビルド用のDockerイメージが作成されます。（Dockerイメージの作成には数分程度時間がかかります）  
Dockerイメージの作成は一度実行するだけでOKです。

```bash
make docker
```

## mrubyのビルド

以下のコマンドでmrubyをビルドします。  
mrubyのビルドはmrubyのライブラリ(mrbgems)を追加する場合には再度実行が必要ですが、通常は一度実行するだけでOKです。

```bash
cd mruby
rake MRUBY_CONFIG=../microbit.rb
```

## microbit-mrubyのビルド

microbit-mrubyディレクトリ内で、以下のコマンドを実行します。

```bash
make
```

ビルドが終了すると **MICROBIT.hex** が生成されます。

## アプリケーションの実行

USBケーブルで micro:bit を接続します。（micro:bitがディスクドライブとして認識されます）  
micro:bitのドライブのルートディレクトリに MICROBIT.hex をコピーすると、microbit-mruby が起動され、mrubyアプリケーションが実行されます。  
Githubリポジトリの[サンプルコード](mrbapp.rb)は、5x5のLEDの真ん中のLEDを使用したＬチカのソースコードになっています。

### サンプルプログラムの実行イメージ

![L-chika](https://github.com/mruby-Forum/microbit-mruby/blob/image/img/microbit-mruby-lchika.jpg)

---

## mrubyでアプリケーションを作ってみよう

microbit-mrubyで実行されるmrubyアプリケーションのソースコードは **mrbapp.rb** に記述します。  
**mrbapp.rb** の内容を変更した場合は、上記の **microbit-mrubyのビルド** と **アプリケーションの実行** を実施してください。

現状は、mrubyアプリケーションを変更した場合には、micro:bitのファームウェアと合わせてビルドされます。  
今後、mrubyアプリケーションだけを変更できるように改善していく予定です。

## 利用できるRuby(mruby)のクラス

- Rubyの組み込みクラス
- LEDクラス

※ クラス仕様の詳細は準備中です

micro:bitに搭載されているデバイスを利用するためのクラスは順次追加予定ですのでご期待ください。

---

# ライセンス

本ソフトウェアはMITライセンスのもとで公開しています。[LICENSE](LICENSE)を参照してください。
