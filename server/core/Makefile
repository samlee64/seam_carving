build-docker:
	docker build -t seam-carving -f ./devops/Dockerfile devops

run-docker:
	docker run -it --rm seam-carving bash

setup:
	- make proxygen

proxygen:
	#git clone https://github.com/facebook/proxygen.git
	cd proxygen/proxygen && ./build.sh && ./install.sh

