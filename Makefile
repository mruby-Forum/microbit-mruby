curdir=$(shell pwd)

all:
	mruby/bin/mrbc -o source/mrbapp.c -Bappbin mrbapp.rb
	docker run -v $(curdir):/app --rm microbit-tools

docker:
	docker build -t microbit-tools .

clean:
	docker run -v $(curdir):/app --rm microbit-tools --clean
