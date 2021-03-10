curdir=$(shell pwd)

all:
	docker run -v $(curdir):/app --rm microbit-tools

docker:
	docker build -t microbit-tools .

clean:
	docker run -v $(curdir):/app --rm microbit-tools --clean
