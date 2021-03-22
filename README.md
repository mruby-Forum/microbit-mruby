# micro:bit mruby

<div align="right">
<a href="README-ja.md">日本語</a>
</div>

# What's micro:bit mruby

**micro:bit mruby** (microbit-mruby) aims to put an implementation of mruby 3.0 on [BBC micro:bit v2](https://microbit.org/)

(micro:bit mruby is available on microbic:bit v2 only.)

microbit-mruby is developed by using [microbit-v2-samples](https://github.com/lancaster-university/microbit-v2-samples) 

# Prerequisites
To compile microbit-mruby out of the source code you need the following tools:

- C Compiler (e.g. gcc or clang)
- Ruby 2.7 or later (e.g. ruby or jruby)
- Docker
- git

## How to get microbit-mruby

```bash
cd $WORKING_DIR
git clone https://github.com/mruby-Forum/microbit-mruby.git
cd microbit-mruby
git clone https://github.com/mruby/mruby.git -b 3.0.0 --depth 1
```
- Please set **$WORKING_DIR** according to your environmnent

## Generate the Docker image

```bash
make docker
```
You need to genarate the docker image for building microbit -mruby and it will take a few minutes.

##  Build mruby

```bash
cd mruby
rake MRUBY_CONFIG=../microbit.rb
```
Note: The mruby build needs to be executed again when adding the mruby library (mrbgems)

## Build microbit-mruby
 just invoke make inside of the microvit-mruby directory.

```bash
make
```
When the build is completed successfully , **MICROBIT.hex**  will be generated.

## Run the application
Connect to the Micro:bit via USB cable ,then Micro:bit will be appeared as a folder.
Copy the  MICROBIT.hex to the micro:bit folder, microbit-mruby will be launched and runs the LED lightning application  [Lightnig LED sample cord](mrbapp.rb)

### Picture of lightnig LED application

![L-chika](https://github.com/mruby-Forum/microbit-mruby/blob/image/img/microbit-mruby-lchika.jpg)


## Create mruby application

The source code of the mruby application executed by microbit-mruby should be described in ** mrbapp.rb **.
If you have changed the contents of ** mrbapp.rb **, please execute the above ** build microbit-mruby ** and ** run the application **.

Note:If you change the mruby application, the micro: bit firmware will be built again

## Available Ruby(mruby) class
- Ruby embedded class
- LED class

Note: We are preparing the details of the calss specification and planning to add more classes.

# License

microbit-mruby is released under the MIT License. Please see [LICENSE](LICENSE)
